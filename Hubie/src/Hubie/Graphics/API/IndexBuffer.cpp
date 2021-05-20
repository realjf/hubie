#include "hbpch.h"
#include "IndexBuffer.h"

namespace Hubie
{
	namespace Graphics
	{
		IndexBuffer* (*IndexBuffer::Create16Func)(uint16_t*, uint32_t, BufferUsage) = nullptr;
		IndexBuffer* (*IndexBuffer::CreateFunc)(uint32_t*, uint32_t, BufferUsage) = nullptr;

		IndexBuffer* IndexBuffer::Create(uint16_t* data, uint32_t count, BufferUsage bufferUsage)
		{
			HB_ASSERT(CreateFunc, "No IndexBuffer Create Function");

			return Create16Func(data, count, bufferUsage);
		}

		IndexBuffer* IndexBuffer::Create(uint32_t* data, uint32_t count, BufferUsage bufferUsage)
		{
			HB_ASSERT(CreateFunc, "No IndexBuffer Create Function");

			return CreateFunc(data, count, bufferUsage);
		}
	}
}
