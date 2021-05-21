#pragma once
#include "Hubie/Graphics/API/RenderDevice.h"

namespace Hubie
{
    namespace Graphics
    {
        class VKRenderDevice : public RenderDevice
        {
        public:
            VKRenderDevice() = default;
            ~VKRenderDevice() = default;

            void Init() override;

            static void MakeDefault();

        protected:
            static RenderDevice* CreateFuncVulkan();
        };
    }
}
