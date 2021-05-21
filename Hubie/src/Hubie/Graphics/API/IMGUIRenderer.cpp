#include "hbpch.h"
#include "IMGUIRenderer.h"
#include "GraphicsContext.h"

#ifdef HB_IMGUI
#ifdef HB_RENDER_API_OPENGL
#include "Hubie/Platform/OpenGL/GLIMGUIRenderer.h"
#endif

#ifdef HB_RENDER_API_VULKAN
#include "Hubie/Platform/Vulkan/VKIMGUIRenderer.h"
#endif
#endif

namespace Hubie
{
	namespace Graphics
	{
		IMGUIRenderer* (*IMGUIRenderer::CreateFunc)(uint32_t, uint32_t, bool) = nullptr;

		IMGUIRenderer* IMGUIRenderer::Create(uint32_t width, uint32_t height, bool clearScreen)
		{
			HB_ASSERT(CreateFunc, "No IMGUIRenderer Create Function");

			return CreateFunc(width, height, clearScreen);
		}
	}
}
