#include "hbpch.h"
#include "SkyboxRenderer.h"
#include "Hubie/Graphics/API/Shader.h"
#include "Hubie/Graphics/API/Framebuffer.h"
#include "Hubie/Graphics/API/Texture.h"
#include "Hubie/Graphics/API/UniformBuffer.h"
#include "Hubie/Graphics/API/Renderer.h"
#include "Hubie/Graphics/API/CommandBuffer.h"
#include "Hubie/Graphics/API/Swapchain.h"
#include "Hubie/Graphics/API/RenderPass.h"
#include "Hubie/Graphics/API/GraphicsContext.h"
#include "Hubie/Graphics/API/Pipeline.h"
#include "Hubie/Graphics/GBuffer.h"
#include "Hubie/Graphics/Mesh.h"
#include "Hubie/Graphics/MeshFactory.h"
#include "RenderGraph.h"
#include "Hubie/Scene/Scene.h"
#include "Hubie/Core/Application.h"
#include "Hubie/Graphics/Camera/Camera.h"
#include "Hubie/Graphics/Environment.h"

#include <imgui/imgui.h>

namespace Hubie
{
    namespace Graphics
    {
        SkyboxRenderer::SkyboxRenderer(uint32_t width, uint32_t height)
            : m_UniformBuffer(nullptr)
            , m_CubeMap(nullptr)
        {
            m_Pipeline = nullptr;

            SetScreenBufferSize(width, height);
            Init();
        }

        SkyboxRenderer::~SkyboxRenderer()
        {
            delete m_UniformBuffer;
            delete m_Skybox;
            delete[] m_VSSystemUniformBuffer;

            m_Framebuffers.clear();
        }

        void SkyboxRenderer::RenderScene()
        {
            HB_PROFILE_FUNCTION();
            if(!m_CubeMap)
                return;

            m_CurrentBufferID = 0;
            if(!m_RenderTexture)
                m_CurrentBufferID = Renderer::GetSwapchain()->GetCurrentBufferIndex();

            Begin();
            SetSystemUniforms(m_Shader.get());
            m_Pipeline->Bind(Renderer::GetSwapchain()->GetCurrentCommandBuffer());

            m_CurrentDescriptorSets[0] = m_Pipeline->GetDescriptorSet();

            m_Skybox->GetVertexBuffer()->Bind(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), m_Pipeline.get());
            m_Skybox->GetIndexBuffer()->Bind(Renderer::GetSwapchain()->GetCurrentCommandBuffer());

            Renderer::BindDescriptorSets(m_Pipeline.get(), Renderer::GetSwapchain()->GetCurrentCommandBuffer(), 0, m_CurrentDescriptorSets);
            Renderer::DrawIndexed(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), DrawType::TRIANGLE, m_Skybox->GetIndexBuffer()->GetCount());

            m_Skybox->GetVertexBuffer()->Unbind();
            m_Skybox->GetIndexBuffer()->Unbind();

            End();

            //if(!m_RenderTexture)
            //Renderer::Present((m_CommandBuffers[m_CurrentBufferID].get()));
        }

        enum VSSystemUniformIndices : int32_t
        {
            VSSystemUniformIndex_InverseProjectionViewMatrix = 0,
            VSSystemUniformIndex_Size
        };

        void SkyboxRenderer::Init()
        {
            HB_PROFILE_FUNCTION();
            m_Shader = Application::Get().GetShaderLibrary()->GetResource("/CoreShaders/Skybox.shader");
            m_Skybox = Graphics::CreateQuad();

            // Vertex shader System uniforms
            m_VSSystemUniformBufferSize = sizeof(Maths::Matrix4);
            m_VSSystemUniformBuffer = new uint8_t[m_VSSystemUniformBufferSize];
            memset(m_VSSystemUniformBuffer, 0, m_VSSystemUniformBufferSize);
            m_VSSystemUniformBufferOffsets.resize(VSSystemUniformIndex_Size);

            // Per Scene System Uniforms
            m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_InverseProjectionViewMatrix] = 0;

            AttachmentInfo textureTypes[2] = {
                { TextureType::COLOUR, TextureFormat::RGBA8 },
                { TextureType::DEPTH, TextureFormat::DEPTH }
            };

            Graphics::RenderPassInfo renderpassCI {};
            renderpassCI.attachmentCount = 2;
            renderpassCI.textureType = textureTypes;
            renderpassCI.clear = false;

            m_RenderPass = Graphics::RenderPass::Get(renderpassCI);

            CreateGraphicsPipeline();
            UpdateUniformBuffer();
            CreateFramebuffers();

            m_CurrentDescriptorSets.resize(1);
        }

        void SkyboxRenderer::Begin()
        {
            HB_PROFILE_FUNCTION();
            m_RenderPass->BeginRenderpass(Renderer::GetSwapchain()->GetCurrentCommandBuffer(), Maths::Vector4(0.0f), m_Framebuffers[m_CurrentBufferID].get(), Graphics::INLINE, m_ScreenBufferWidth, m_ScreenBufferHeight);
        }

        void SkyboxRenderer::BeginScene(Scene* scene, Camera* overrideCamera, Maths::Transform* overrideCameraTransform)
        {
            HB_PROFILE_FUNCTION();
            auto& registry = scene->GetRegistry();

            auto view = registry.view<Graphics::Environment>();

            if(view.size() != 0)
            {
                //Just use first
                const auto& env = view.get<Graphics::Environment>(view.front());

                if(m_CubeMap != env.GetEnvironmentMap())
                {
                    m_CubeMap = env.GetEnvironmentMap();
                    UpdateUniformBuffer();
                }
            }
            else
            {
                m_CubeMap = nullptr;
                return;
            }

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

            auto invViewProj = Maths::Matrix4::Inverse(m_Camera->GetProjectionMatrix() * m_CameraTransform->GetWorldMatrix().Inverse());
            memcpy(m_VSSystemUniformBuffer + m_VSSystemUniformBufferOffsets[VSSystemUniformIndex_InverseProjectionViewMatrix], &invViewProj, sizeof(Maths::Matrix4));
        }

        void SkyboxRenderer::End()
        {
            HB_PROFILE_FUNCTION();
            m_RenderPass->EndRenderpass(Renderer::GetSwapchain()->GetCurrentCommandBuffer());
        }

        void SkyboxRenderer::SetSystemUniforms(Shader* shader) const
        {
            HB_PROFILE_FUNCTION();
            m_UniformBuffer->SetData(sizeof(UniformBufferObject), *&m_VSSystemUniformBuffer);
        }

        void SkyboxRenderer::OnResize(uint32_t width, uint32_t height)
        {
            HB_PROFILE_FUNCTION();
            m_Framebuffers.clear();

            SetScreenBufferSize(width, height);

            m_CubeMap = nullptr;
            UpdateUniformBuffer();
            CreateFramebuffers();
        }

        void SkyboxRenderer::CreateGraphicsPipeline()
        {
            HB_PROFILE_FUNCTION();
            Graphics::PipelineInfo pipelineCreateInfo {};
            pipelineCreateInfo.shader = m_Shader;
            pipelineCreateInfo.renderpass = m_RenderPass;
            pipelineCreateInfo.polygonMode = Graphics::PolygonMode::FILL;
            pipelineCreateInfo.cullMode = Graphics::CullMode::NONE;
            pipelineCreateInfo.transparencyEnabled = false;
            pipelineCreateInfo.depthBiasEnabled = false;

            m_Pipeline = Graphics::Pipeline::Get(pipelineCreateInfo);
        }

        void SkyboxRenderer::UpdateUniformBuffer()
        {
            HB_PROFILE_FUNCTION();
            if(m_UniformBuffer == nullptr)
            {
                m_UniformBuffer = Graphics::UniformBuffer::Create();
                uint32_t bufferSize = static_cast<uint32_t>(sizeof(UniformBufferObject));
                m_UniformBuffer->Init(bufferSize, nullptr);
            }

            std::vector<Graphics::BufferInfo> bufferInfos;

            Graphics::BufferInfo bufferInfo = {};
            bufferInfo.buffer = m_UniformBuffer;
            bufferInfo.offset = 0;
            bufferInfo.size = sizeof(UniformBufferObject);
            bufferInfo.type = Graphics::DescriptorType::UNIFORM_BUFFER;
            bufferInfo.binding = 0;
            bufferInfo.shaderType = ShaderType::VERTEX;

            bufferInfos.push_back(bufferInfo);

            std::vector<Graphics::ImageInfo> imageInfos;

            if(m_CubeMap)
            {
                Graphics::ImageInfo imageInfo = {};
                imageInfo.texture = m_CubeMap;
                imageInfo.name = "u_CubeMap";
                imageInfo.binding = 1;
                imageInfo.type = TextureType::CUBE;

                imageInfos.push_back(imageInfo);
            }

            if(m_Pipeline != nullptr)
                m_Pipeline->GetDescriptorSet()->Update(imageInfos, bufferInfos);
        }

        void SkyboxRenderer::SetCubeMap(Texture* cubeMap)
        {
            HB_PROFILE_FUNCTION();
            m_CubeMap = cubeMap;
            UpdateUniformBuffer();
        }

        void SkyboxRenderer::SetRenderTarget(Texture* texture, bool rebuildFramebuffer)
        {
            HB_PROFILE_FUNCTION();
            m_RenderTexture = texture;

            if(rebuildFramebuffer)
            {
                m_Framebuffers.clear();

                CreateFramebuffers();
            }
        }

        void SkyboxRenderer::CreateFramebuffers()
        {
            HB_PROFILE_FUNCTION();
            TextureType attachmentTypes[2];
            attachmentTypes[0] = TextureType::COLOUR;
            attachmentTypes[1] = TextureType::DEPTH;

            Texture* attachments[2];
            FramebufferInfo bufferInfo {};
            bufferInfo.width = m_ScreenBufferWidth;
            bufferInfo.height = m_ScreenBufferHeight;
            bufferInfo.attachmentCount = 2;
            bufferInfo.renderPass = m_RenderPass.get();
            bufferInfo.attachmentTypes = attachmentTypes;

            attachments[1] = dynamic_cast<Texture*>(Application::Get().GetRenderGraph()->GetGBuffer()->GetDepthTexture());

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

        void SkyboxRenderer::OnImGui()
        {
            HB_PROFILE_FUNCTION();
            ImGui::TextUnformatted("Skybox Renderer");
            if(ImGui::TreeNode("CubeMap"))
            {
                bool flipImage = Graphics::GraphicsContext::GetContext()->FlipImGUITexture();

                ImGui::Image(m_CubeMap->GetHandle(), ImVec2(128, 128), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));

                if(ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Image(m_CubeMap->GetHandle(), ImVec2(256, 256), ImVec2(0.0f, flipImage ? 1.0f : 0.0f), ImVec2(1.0f, flipImage ? 0.0f : 1.0f));
                    ImGui::EndTooltip();
                }

                ImGui::TreePop();
            }
        }
    }
}
