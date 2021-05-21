#include "hbpch.h"
#include "LineRenderer.h"
#include "Hubie/Core/OS/Window.h"
#include "Hubie/Graphics/API/Shader.h"
#include "Hubie/Graphics/API/Framebuffer.h"
#include "Hubie/Graphics/API/UniformBuffer.h"
#include "Hubie/Graphics/API/Renderer.h"
#include "Hubie/Graphics/API/CommandBuffer.h"
#include "Hubie/Graphics/API/Swapchain.h"
#include "Hubie/Graphics/API/RenderPass.h"
#include "Hubie/Graphics/API/Pipeline.h"
#include "Hubie/Graphics/API/IndexBuffer.h"
#include "Hubie/Graphics/API/Texture.h"
#include "Hubie/Graphics/API/GraphicsContext.h"
#include "Hubie/Graphics/GBuffer.h"
#include "Hubie/Graphics/Sprite.h"
#include "Hubie/Scene/Scene.h"
#include "Hubie/Core/Application.h"
#include "Hubie/Platform/OpenGL/GLDescriptorSet.h"
#include "Hubie/Graphics/Renderable2D.h"
#include "Hubie/Graphics/Camera/Camera.h"
#include "Hubie/Graphics/Renderers/Renderer2D.h"
#include "Hubie/Maths/Transform.h"

namespace Hubie
{
    using namespace Graphics;

    static const uint32_t MaxLines = 10000;
    static const uint32_t MaxLineVertices = MaxLines * 2;
    static const uint32_t MaxLineIndices = MaxLines * 6;
    static const uint32_t MAX_BATCH_DRAW_CALLS = 100;
    static const uint32_t RENDERER_LINE_SIZE = sizeof(LineVertexData) * 4;
    static const uint32_t RENDERER_BUFFER_SIZE = RENDERER_LINE_SIZE * MaxLineVertices;

    LineRenderer::LineRenderer(uint32_t width, uint32_t height, bool clear)
        : m_Buffer(nullptr)
        , m_Clear(clear)
    {
        m_RenderTexture = nullptr;
        m_BatchDrawCallIndex = 0;
        LineIndexCount = 0;

        LineRenderer::SetScreenBufferSize(width, height);
        LineRenderer::Init();
    }

    LineRenderer::~LineRenderer()
    {
        delete m_IndexBuffer;
        delete m_UniformBuffer;

        delete[] m_VSSystemUniformBuffer;

        m_Framebuffers.clear();

        for(int i = 0; i < MAX_BATCH_DRAW_CALLS; i++)
            delete m_VertexBuffers[i];
    }

    void LineRenderer::Init()
    {
        HB_PROFILE_FUNCTION();

        m_Shader = Application::Get().GetShaderLibrary()->GetResource("/CoreShaders/Batch2DLine.shader");

        m_VSSystemUniformBufferSize = sizeof(Maths::Matrix4);
        m_VSSystemUniformBuffer = new uint8_t[m_VSSystemUniformBufferSize];

        m_UniformBuffer = Graphics::UniformBuffer::Create();

        AttachmentInfo textureTypes[2] = {
            { TextureType::COLOUR, TextureFormat::RGBA8 }
        };

        Graphics::RenderPassInfo renderpassCI;
        renderpassCI.attachmentCount = 1;
        renderpassCI.textureType = textureTypes;
        renderpassCI.clear = false;

        m_RenderPass = Graphics::RenderPass::Get(renderpassCI);

        CreateFramebuffers();

        CreateGraphicsPipeline();

        uint32_t bufferSize = static_cast<uint32_t>(sizeof(UniformBufferObject));
        m_UniformBuffer->Init(bufferSize, nullptr);

        std::vector<Graphics::BufferInfo> bufferInfos;

        Graphics::BufferInfo bufferInfo;
        bufferInfo.buffer = m_UniformBuffer;
        bufferInfo.offset = 0;
        bufferInfo.size = sizeof(UniformBufferObject);
        bufferInfo.type = Graphics::DescriptorType::UNIFORM_BUFFER;
        bufferInfo.shaderType = Graphics::ShaderType::VERTEX;
        bufferInfo.name = "UniformBufferObject";
        bufferInfo.binding = 0;

        bufferInfos.push_back(bufferInfo);

        m_Pipeline->GetDescriptorSet()->Update(bufferInfos);

        m_VertexBuffers.resize(MAX_BATCH_DRAW_CALLS);

        for(auto& vertexBuffer : m_VertexBuffers)
        {
            vertexBuffer = Graphics::VertexBuffer::Create(BufferUsage::DYNAMIC);
            vertexBuffer->Resize(RENDERER_BUFFER_SIZE);
        }

        uint32_t* indices = new uint32_t[MaxLineIndices];

        for(int32_t i = 0; i < MaxLineIndices; i++)
        {
            indices[i] = i;
        }

        m_IndexBuffer = IndexBuffer::Create(indices, MaxLineIndices);

        delete[] indices;

        m_ClearColour = Maths::Vector4(0.2f, 0.7f, 0.2f, 1.0f);
        m_CurrentDescriptorSets.resize(1);
    }

    void LineRenderer::Submit(const Maths::Vector3& p1, const Maths::Vector3& p2, const Maths::Vector4& colour)
    {
        m_Lines.emplace_back(p1, p2, colour);
    }

    void LineRenderer::SubmitInternal(const LineInfo& info)
    {
        if(LineIndexCount >= MaxLineIndices)
            FlushAndResetLines();

        m_Buffer->vertex = info.p1;
        m_Buffer->colour = info.col;
        m_Buffer++;

        m_Buffer->vertex = info.p2;
        m_Buffer->colour = info.col;
        m_Buffer++;

        LineIndexCount += 2;
    }

    struct PointVertex
    {
        Maths::Vector4 pos;
        Maths::Vector4 col;
    };

    struct LineVertex
    {
        PointVertex p0;
        PointVertex p1;
    };

    struct TriVertex
    {
        PointVertex p0;
        PointVertex p1;
        PointVertex p2;
    };

    void LineRenderer::Begin()
    {
        m_CurrentBufferID = 0;
        LineIndexCount = 0;

        m_Lines.clear();

        //m_RenderPass->BeginRenderpass(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), m_ClearColour, m_Framebuffers[m_CurrentBufferID].get(), Graphics::SECONDARY, m_ScreenBufferWidth, m_ScreenBufferHeight);
    }

    void LineRenderer::SetSystemUniforms(Shader* shader) const
    {
        m_UniformBuffer->SetData(sizeof(UniformBufferObject), *&m_VSSystemUniformBuffer);
    }

    void LineRenderer::BeginScene(Scene* scene, Camera* overrideCamera, Maths::Transform* overrideCameraTransform)
    {
        auto& registry = scene->GetRegistry();

        if(overrideCamera)
        {
            m_Camera = overrideCamera;
            m_CameraTransform = overrideCameraTransform;
        }
        else
        {
            auto cameraView = registry.view<Camera>();
            if(!cameraView.empty())
            {
                m_Camera = &cameraView.get<Camera>(cameraView.front());
                m_CameraTransform = registry.try_get<Maths::Transform>(cameraView.front());
            }
        }

        if(!m_Camera || !m_CameraTransform)
            return;

        auto projView = m_Camera->GetProjectionMatrix() * m_CameraTransform->GetWorldMatrix().Inverse();

        memcpy(m_VSSystemUniformBuffer, &projView, sizeof(Maths::Matrix4));
    }

    void LineRenderer::Present()
    {
        Graphics::CommandBuffer* currentCMDBuffer = Renderer::GetSwapchain()->GetCurrentCommandBuffer();

        m_Pipeline->Bind(currentCMDBuffer);

        m_VertexBuffers[m_BatchDrawCallIndex]->ReleasePointer();
        m_VertexBuffers[m_BatchDrawCallIndex]->Unbind();

        m_IndexBuffer->SetCount(LineIndexCount);

        m_CurrentDescriptorSets[0] = m_Pipeline->GetDescriptorSet();

        m_VertexBuffers[m_BatchDrawCallIndex]->Bind(currentCMDBuffer, m_Pipeline.get());
        m_IndexBuffer->Bind(currentCMDBuffer);

        Renderer::BindDescriptorSets(m_Pipeline.get(), currentCMDBuffer, 0, m_CurrentDescriptorSets);
        Renderer::DrawIndexed(currentCMDBuffer, DrawType::LINES, LineIndexCount);

        m_VertexBuffers[m_BatchDrawCallIndex]->Unbind();
        m_IndexBuffer->Unbind();

        LineIndexCount = 0;

        m_BatchDrawCallIndex++;
    }

    void LineRenderer::End()
    {
        m_RenderPass->EndRenderpass(Renderer::GetSwapchain()->GetCurrentCommandBuffer());

        m_BatchDrawCallIndex = 0;
    }

    void LineRenderer::RenderInternal()
    {
        HB_PROFILE_FUNCTION();

        if(!m_RenderTexture)
            m_CurrentBufferID = Renderer::GetSwapchain()->GetCurrentBufferIndex();

        m_Pipeline->Bind(Renderer::GetSwapchain()->GetCurrentCommandBuffer());

        m_RenderPass->BeginRenderpass(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), m_ClearColour, m_Framebuffers[m_CurrentBufferID].get(), Graphics::INLINE, m_ScreenBufferWidth, m_ScreenBufferHeight);

        m_VertexBuffers[m_BatchDrawCallIndex]->Bind(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), m_Pipeline.get());
        m_Buffer = m_VertexBuffers[m_BatchDrawCallIndex]->GetPointer<LineVertexData>();

        SetSystemUniforms(m_Shader.get());

        for(auto& line : m_Lines)
        {
            SubmitInternal(line);
        }

        Present();

        End();
    }

    void LineRenderer::OnResize(uint32_t width, uint32_t height)
    {
        m_Framebuffers.clear();

        SetScreenBufferSize(width, height);

        CreateFramebuffers();
    }

    void LineRenderer::PresentToScreen()
    {
        //Renderer::Present((m_CommandBuffers[Renderer::GetSwapchain()->GetCurrentBufferIndex()].get()));
    }

    void LineRenderer::SetScreenBufferSize(uint32_t width, uint32_t height)
    {
        if(width == 0)
        {
            width = 1;
            HB_CRITICAL("Width 0");
        }
        if(height == 0)
        {
            height = 1;
            HB_CRITICAL("Height 0");
        }
        m_ScreenBufferWidth = width;
        m_ScreenBufferHeight = height;
    }

    void LineRenderer::CreateGraphicsPipeline()
    {
        Graphics::PipelineInfo pipelineCreateInfo;
        pipelineCreateInfo.shader = m_Shader;
        pipelineCreateInfo.renderpass = m_RenderPass;
        pipelineCreateInfo.polygonMode = Graphics::PolygonMode::FILL;
        pipelineCreateInfo.cullMode = Graphics::CullMode::BACK;
        pipelineCreateInfo.transparencyEnabled = false;
        pipelineCreateInfo.depthBiasEnabled = true;
        pipelineCreateInfo.drawType = DrawType::LINES;

        m_Pipeline = Graphics::Pipeline::Get(pipelineCreateInfo);
    }

    void LineRenderer::CreateFramebuffers()
    {
        TextureType attachmentTypes[1];
        attachmentTypes[0] = TextureType::COLOUR;

        Texture* attachments[1];
        FramebufferInfo bufferInfo {};
        bufferInfo.width = m_ScreenBufferWidth;
        bufferInfo.height = m_ScreenBufferHeight;
        bufferInfo.attachmentCount = 1;
        bufferInfo.renderPass = m_RenderPass.get();
        bufferInfo.attachmentTypes = attachmentTypes;

        if(m_RenderTexture)
        {
            attachments[0] = m_RenderTexture;
            bufferInfo.attachments = attachments;
            bufferInfo.screenFBO = false;
            m_Framebuffers.emplace_back(Framebuffer::Get(bufferInfo));
        }
        else
        {
            for(uint32_t i = 0; i < Renderer::GetSwapchain()->GetSwapchainBufferCount(); i++)
            {
                bufferInfo.screenFBO = true;
                attachments[0] = Renderer::GetSwapchain()->GetImage(i);
                bufferInfo.attachments = attachments;

                m_Framebuffers.emplace_back(Framebuffer::Get(bufferInfo));
            }
        }
    }

    void LineRenderer::SetRenderTarget(Graphics::Texture* texture, bool rebuildFramebuffer)
    {
        m_RenderTexture = texture;

        if(!rebuildFramebuffer)
            return;

        m_Framebuffers.clear();

        CreateFramebuffers();
    }

    void LineRenderer::FlushAndResetLines()
    {
        Present();

        m_Lines.clear();

        m_VertexBuffers[m_BatchDrawCallIndex]->Bind(nullptr, nullptr);
        m_Buffer = m_VertexBuffers[m_BatchDrawCallIndex]->GetPointer<LineVertexData>();
    }
}
