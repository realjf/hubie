#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Maths/Maths.h"

namespace Hubie
{
    namespace Graphics
    {
        enum class HB_EXPORT LightType
        {
            DirectionalLight = 0,
            SpotLight = 1,
            PointLight = 2
        };

        struct HB_EXPORT MEM_ALIGN Light
        {
            Light(const Maths::Vector3& direction = Maths::Vector3(0.0f), const Maths::Vector4& colour = Maths::Vector4(1.0f), float intensity = 1.0f, const LightType& type = LightType::DirectionalLight, const Maths::Vector3& position = Maths::Vector3(), float radius = 1.0f, float angle = 0.0f);

            void OnImGui();
            static std::string LightTypeToString(Graphics::LightType type);
            static float StringToLightType(const std::string& type);

            Maths::Vector4 Colour;
            Maths::Vector4 Position;
            Maths::Vector4 Direction;
            float Intensity;
            float Radius;
            float Type;
            float Angle;
        };

        template <class Archive>
        void serialize(Archive& archive, Graphics::Light& light)
        {
            archive(light.Position, light.Colour, light.Type, light.Angle, light.Direction, light.Intensity, light.Radius);
        }

    }
}
