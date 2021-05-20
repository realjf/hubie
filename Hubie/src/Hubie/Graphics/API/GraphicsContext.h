#pragma once

#include "Hubie/Core/OS/Window.h"

namespace Hubie
{
	namespace Graphics
	{

		enum class RenderAPI : uint32_t
		{
			OPENGL = 0,
			VULKAN,
			DIRECT3D, //Unsupported
			METAL, //Unsupported
			NONE, //Unsupported
		};

		class CommandBuffer;

		class HB_EXPORT GraphicsContext
		{
		public:
			virtual ~GraphicsContext();

			static void Create(const WindowProperties& properties, Window* window);
			static void Release();

			static RenderAPI GetRenderAPI() { return s_RenderAPI; }
			static void SetRenderAPI(RenderAPI api);

			virtual void Init() = 0;
			virtual void Present() = 0;
			virtual float GetGPUMemoryUsed() = 0;
			virtual float GetTotalGPUMemory() = 0;

			virtual size_t GetMinUniformBufferOffsetAlignment() const = 0;

			static GraphicsContext* GetContext() { return s_Context; }
			virtual bool FlipImGUITexture() const = 0;
			virtual void WaitIdle() const = 0;
			virtual void OnImGui() = 0;

		protected:
			static GraphicsContext* (*CreateFunc)(const WindowProperties&, Window*);

			static GraphicsContext* s_Context;
			static RenderAPI s_RenderAPI;
		};
	}
}
