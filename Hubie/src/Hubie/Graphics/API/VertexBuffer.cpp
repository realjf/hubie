#include "hbpch.h"
#include "VertexBuffer.h"

namespace Hubie
{
	namespace Graphics
	{
		VertexBuffer* (*VertexBuffer::CreateFunc)(const BufferUsage&) = nullptr;

		VertexBuffer* VertexBuffer::Create(const BufferUsage& usage)
		{
			HB_ASSERT(CreateFunc, "No VertexBuffer Create Function");

			return CreateFunc(usage);
		}
	}
}
