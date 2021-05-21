#include "Precompiled.h"
#include "GLRenderDevice.h"

namespace Hubie::Graphics
{
    void GLRenderDevice::Init()
    {
    }

    void GLRenderDevice::MakeDefault()
    {
        CreateFunc = CreateFuncGL;
    }

    RenderDevice* GLRenderDevice::CreateFuncGL()
    {
        return new GLRenderDevice();
    }
}
