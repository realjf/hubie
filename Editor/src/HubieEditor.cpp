#include <HubieEngine.h>
#include <Hubie/Core/EntryPoint.h>
#include "Editor.h"

Hubie::Application* Hubie::CreateApplication()
{
    return new Editor();
}