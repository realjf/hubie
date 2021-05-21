#include "hbpch.h"
#include "Sprite.h"
#include "Hubie/Graphics/Material.h"
#include "Hubie/Graphics/API/Texture.h"
#include "Hubie/Graphics/API/GraphicsContext.h"
#include "Mesh.h"
#include "Hubie/Core/Application.h"
#include "MeshFactory.h"

namespace Hubie
{
    namespace Graphics
    {
        Sprite::Sprite(const Maths::Vector2& position, const Maths::Vector2& scale, const Maths::Vector4& colour)
        {
            m_Position = position;
            m_Scale = scale;
            m_Colour = colour;
            m_UVs = GetDefaultUVs();
            m_Texture = nullptr;
        }

        Sprite::Sprite(const Ref<Texture2D>& texture, const Maths::Vector2& position, const Maths::Vector2& scale, const Maths::Vector4& colour)
        {
            m_Texture = texture;
            m_Position = position;
            m_Scale = scale;
            m_Colour = colour;
            m_UVs = GetDefaultUVs();
        }

        Sprite::~Sprite()
        {
        }

        void Sprite::SetSpriteSheet(const Ref<Texture2D>& texture, const Maths::Vector2& index, const Maths::Vector2& cellSize, const Maths::Vector2& spriteSize)
        {
            m_Texture = texture;
            Maths::Vector2 min = { (index.x * cellSize.x) / texture->GetWidth(), (index.y * cellSize.y) / texture->GetHeight() };
            Maths::Vector2 max = { ((index.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((index.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };

            m_UVs = GetUVs(min, max);
        }

        void Sprite::SetTextureFromFile(const std::string& filePath)
        {
            auto tex = Ref<Graphics::Texture2D>(Graphics::Texture2D::CreateFromFile(filePath, filePath));
            if(tex)
            {
                m_Texture = tex;
            }
        }
    }
}
