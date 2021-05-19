#pragma once

#include <Hubie/Core/Application.h>

namespace Hubie
{
    class Editor : public Application
    {
    public:
        Editor();
        virtual ~Editor();

        void Init() override;

    protected:
        static Editor* s_Editor;
    };
}