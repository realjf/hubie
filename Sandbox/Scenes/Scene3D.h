#pragma once
#include <HubieEngine.h>

class Scene3D : public Hubie::Scene
{
public:
	Scene3D(const std::string& SceneName);
	virtual ~Scene3D();

	virtual void OnInit() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdate(const Hubie::TimeStep& timeStep) override;
	virtual void Render2D() override;
	virtual void OnImGui() override;
};
