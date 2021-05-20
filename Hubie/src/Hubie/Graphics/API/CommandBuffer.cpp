#include "hbpch.h"
#include "CommandBuffer.h"

namespace Hubie
{
	namespace Graphics
	{
		CommandBuffer* (*CommandBuffer::CreateFunc)() = nullptr;

		CommandBuffer* CommandBuffer::Create()
		{
			HB_ASSERT(CreateFunc, "No CommandBuffer Create Function");

			return CreateFunc();
		}
	}
}
