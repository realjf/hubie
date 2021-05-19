#include "Editor.h"

namespace Hubie 
{
    Editor* Editor::s_Editor = nullptr;

    Editor::Editor()
    {
        s_Editor = this;
    }

    Editor::~Editor()
    {

    }

    void Editor::Init()
    {
        Application::Init();
        
    }
}