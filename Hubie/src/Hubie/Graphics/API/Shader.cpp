#include "hbpch.h"
#include "Shader.h"

#include "Hubie/Graphics/API/GraphicsContext.h"

namespace Hubie
{
	namespace Graphics
	{
		Shader* (*Shader::CreateFunc)(const std::string&) = nullptr;

		const Shader* Shader::s_CurrentlyBound = nullptr;

		Shader* Shader::CreateFromFile(const std::string& filepath)
		{
			HB_ASSERT(CreateFunc, "No Shader Create Function");
			return CreateFunc(filepath);
		}
	}
}
