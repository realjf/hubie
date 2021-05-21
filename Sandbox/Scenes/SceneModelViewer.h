#pragma once

#include <HubieEngine.h>

class SceneModelViewer : public Hubie::Scene
{
public:
	explicit SceneModelViewer(const std::string& SceneName);
	virtual ~SceneModelViewer();

	virtual void OnInit() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdate(const Hubie::TimeStep& timeStep) override;
	virtual void OnImGui() override;
	void LoadModels();
};
