#pragma once
#include "Hubie/Core/Core.h"
#include "Hubie/Scene/ISystem.h"
#include <vector>
namespace Hubie
{
    class Camera;
    class SoundNode;

    class HB_EXPORT AudioManager : public ISystem
    {
    public:
        static AudioManager* Create();

        virtual ~AudioManager() = default;
        virtual void OnInit() override = 0;
        virtual void OnUpdate(const TimeStep& dt, Scene* scene) override = 0;

        Camera* GetListener() const
        {
            return m_Listener;
        }

        void AddSoundNode(SoundNode* node)
        {
            m_SoundNodes.emplace_back(node);
        }
        void OnDebugDraw() override {};

        void ClearNodes()
        {
            m_SoundNodes.clear();
        }

        bool GetPaused() const { return m_Paused; }
        void SetPaused(bool paused);

    protected:
        Camera* m_Listener;
        std::vector<SoundNode*> m_SoundNodes;
        bool m_Paused;
    };
}
