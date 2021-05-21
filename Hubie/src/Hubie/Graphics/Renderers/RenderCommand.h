#pragma once

#include "Hubie/Graphics/Mesh.h"
#include "Hubie/Graphics/API/Shader.h"

namespace Hubie
{

    namespace Graphics
    {
        class Material;

        struct HB_EXPORT RenderCommand
        {
            Mesh* mesh = nullptr;
            Material* material = nullptr;
            Maths::Matrix4 transform;
            Maths::Matrix4 textureMatrix;
            bool animated = false;
        };
    }
}
