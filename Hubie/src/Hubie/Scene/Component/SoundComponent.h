#pragma once

#include "Hubie/Audio/SoundNode.h"
#include <cereal/cereal.hpp>

namespace Hubie
{
    class HB_EXPORT SoundComponent
    {
    public:
        SoundComponent();
        explicit SoundComponent(Ref<SoundNode>& sound);

        void Init();

        void OnImGui();

        SoundNode* GetSoundNode() const { return m_SoundNode.get(); }

        template <typename Archive>
        void save(Archive& archive) const
        {
            archive(*m_SoundNode.get());
        }

        template <typename Archive>
        void load(Archive& archive)
        {
            auto* node = SoundNode::Create();
            archive(*node);
            m_SoundNode = Ref<SoundNode>(node);
        }

    private:
        Ref<SoundNode> m_SoundNode;
    };
}
