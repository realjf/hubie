#pragma once

namespace Hubie
{
    class AINode
    {
    public:
        AINode() = default;
        virtual ~AINode() = default;

        void Update(float dt) {};
    };
}
