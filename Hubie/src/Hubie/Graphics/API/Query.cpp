#include "hbpch.h"
#include "Query.h"

#ifdef HB_RENDER_API_OPENGL
#include "Hubie/Platform/OpenGL/GLQuery.h"
#endif

#ifdef HB_RENDER_API_DIRECT3D
#include "Hubie/Graphics/directx/DXQuery.h"
#endif

#include "Hubie/Graphics/API/GraphicsContext.h"

namespace Hubie
{
	namespace Graphics
	{
		Query* (*Query::CreateFunc)(QueryType) = nullptr;

		Query* Query::Create(QueryType type)
		{
			HB_ASSERT(CreateFunc, "No Query Create Function");

			return CreateFunc(type);
		}
	}
}
