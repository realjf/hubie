#pragma once

#include "Hubie/AI/AINode.h"

namespace Hubie
{
    class HB_EXPORT AIComponent
    {
    public:
        AIComponent();
        explicit AIComponent(Ref<AINode>& aiNode);

        void OnImGui();

    private:
        Ref<AINode> m_AINode;
    };
}
