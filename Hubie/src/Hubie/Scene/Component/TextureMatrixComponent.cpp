#include "Precompiled.h"
#include "TextureMatrixComponent.h"

namespace Hubie
{
    TextureMatrixComponent::TextureMatrixComponent(const Maths::Matrix4& matrix)
        : m_TextureMatrix(matrix)
    {
    }

    void TextureMatrixComponent::OnImGui()
    {
    }
}
