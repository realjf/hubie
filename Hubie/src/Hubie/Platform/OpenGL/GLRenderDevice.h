#pragma once
#include "Hubie/Graphics/API/RenderDevice.h"

namespace Hubie
{
    namespace Graphics
    {
        class GLRenderDevice : public RenderDevice
        {
        public:
            GLRenderDevice() = default;
            ~GLRenderDevice() = default;

            void Init() override;

            static void MakeDefault();

        protected:
            static RenderDevice* CreateFuncGL();
        };
    }
}
