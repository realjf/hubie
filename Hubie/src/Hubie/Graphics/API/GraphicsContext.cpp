#include "hbpch.h"
#include "GraphicsContext.h"

#ifdef HB_RENDER_API_OPENGL
#include "Hubie/Platform/OpenGL/GLContext.h"
#include "Hubie/Platform/OpenGL/GLFunctions.h"
#endif
#ifdef HB_RENDER_API_VULKAN
#include "Hubie/Platform/Vulkan/VKContext.h"
#include "Hubie/Platform/Vulkan/VKFunctions.h"
#endif
#ifdef HB_RENDER_API_DIRECT3D
#include "Hubie/Graphics/DirectX/DXContext.h"
#include "Hubie/Graphics/DirectX/DXFunctions.h"
#endif

namespace Hubie
{
	namespace Graphics
	{
		GraphicsContext* (*GraphicsContext::CreateFunc)(const WindowProperties&, Window*) = nullptr;

		GraphicsContext* GraphicsContext::s_Context = nullptr;
		RenderAPI GraphicsContext::s_RenderAPI;

		void GraphicsContext::Create(const WindowProperties& properties, Window* window)
		{
			HB_ASSERT(CreateFunc, "No GraphicsContext Create Function");

			s_Context = CreateFunc(properties, window);
		}

		void GraphicsContext::Release()
		{
			delete s_Context;
		}

		GraphicsContext::~GraphicsContext()
		{
		}

		void GraphicsContext::SetRenderAPI(RenderAPI api)
		{
			s_RenderAPI = api;

			switch (s_RenderAPI)
			{
#ifdef HB_RENDER_API_VULKAN
			case RenderAPI::VULKAN:
				Graphics::Vulkan::MakeDefault();
				break;
#endif

#ifdef HB_RENDER_API_OPENGL
			case RenderAPI::OPENGL:
				Graphics::GL::MakeDefault();
				break;
#endif

#ifdef HB_RENDER_API_DIRECT3D
			case RenderAPI::DIRECT3D:
				Graphics::DIRECT3D::MakeDefault();
				break;
#endif
			default:
				break;
			}
		}
	}
}
