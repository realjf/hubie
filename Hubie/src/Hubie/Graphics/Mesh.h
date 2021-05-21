#pragma once

#include "Hubie/Core/Core.h"
#include "API/IndexBuffer.h"
#include "API/VertexBuffer.h"
#include "Hubie/Graphics/API/CommandBuffer.h"
#include "Hubie/Graphics/API/DescriptorSet.h"
#include "Hubie/Maths/Maths.h"
#include "Material.h"

#include <array>

namespace Hubie
{
    namespace Graphics
    {
        class Texture2D;

        struct HB_EXPORT BasicVertex
        {
            Maths::Vector3 Position;
            Maths::Vector2 TexCoords;
        };

        struct HB_EXPORT Vertex
        {
            Vertex()
                : Position(Maths::Vector3(0.0f))
                , Colours(Maths::Vector4(0.0f))
                , TexCoords(Maths::Vector2(0.0f))
                , Normal(Maths::Vector3(0.0f))
                , Tangent(Maths::Vector3(0.0f))
            {
            }

            Maths::Vector3 Position;
            Maths::Vector4 Colours;
            Maths::Vector2 TexCoords;
            Maths::Vector3 Normal;
            Maths::Vector3 Tangent;

            bool operator==(const Vertex& other) const
            {
                return Position == other.Position && TexCoords == other.TexCoords && Colours == other.Colours && Normal == other.Normal && Tangent == other.Tangent;
            }
        };

        class HB_EXPORT Mesh
        {
        public:
            Mesh();
            Mesh(const Mesh& mesh);
            Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices, float optimiseThreshold = 1.0f);
            Mesh(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, const Ref<Maths::BoundingBox>& boundingBox);

            virtual ~Mesh();

            const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
            const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
            const Ref<Material>& GetMaterial() const { return m_Material; }
            const Ref<Maths::BoundingBox>& GetBoundingBox() const { return m_BoundingBox; }

            void SetMaterial(const Ref<Material>& material) { m_Material = material; }

            bool& GetActive() { return m_Active; }
            void SetName(const std::string& name) { m_Name = name; }

            static void GenerateNormals(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);
            static void GenerateTangents(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount);

        protected:
            static Maths::Vector3 GenerateTangent(const Maths::Vector3& a, const Maths::Vector3& b, const Maths::Vector3& c, const Maths::Vector2& ta, const Maths::Vector2& tb, const Maths::Vector2& tc);
            static Maths::Vector3* GenerateNormals(uint32_t numVertices, Maths::Vector3* vertices, uint32_t* indices, uint32_t numIndices);
            static Maths::Vector3* GenerateTangents(uint32_t numVertices, Maths::Vector3* vertices, uint32_t* indices, uint32_t numIndices, Maths::Vector2* texCoords);

            Ref<VertexBuffer> m_VertexBuffer;
            Ref<IndexBuffer> m_IndexBuffer;
            Ref<Material> m_Material;
            Ref<Maths::BoundingBox> m_BoundingBox;

            std::string m_Name;

            bool m_Active = true;
            std::vector<uint32_t> m_Indices;
            std::vector<Vertex> m_Vertices;
        };
    }
}

namespace std
{
    template <>
    struct hash<Hubie::Graphics::Vertex>
    {
        size_t operator()(Hubie::Graphics::Vertex const& vertex) const
        {
            return ((hash<Hubie::Maths::Vector3>()(vertex.Position) ^ (hash<Hubie::Maths::Vector2>()(vertex.TexCoords) << 1) ^ (hash<Hubie::Maths::Vector4>()(vertex.Colours) << 1) ^ (hash<Hubie::Maths::Vector3>()(vertex.Normal) << 1) ^ (hash<Hubie::Maths::Vector3>()(vertex.Tangent) << 1)));
        }
    };
}
