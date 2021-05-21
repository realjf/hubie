#include "hbpch.h"
#include "RenderDevice.h"

namespace Hubie
{
	namespace Graphics
	{
		RenderDevice* (*RenderDevice::CreateFunc)() = nullptr;

		RenderDevice* RenderDevice::s_Instance = nullptr;

		void RenderDevice::Create()
		{
			HB_ASSERT(CreateFunc, "No RenderDevice Create Function");

			s_Instance = CreateFunc();
		}

		void RenderDevice::Release()
		{
			delete s_Instance;
		};
	}
}
