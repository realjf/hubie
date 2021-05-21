#pragma once
#include <HubieEngine.h>

class MaterialTest : public Hubie::Scene
{
public:
	MaterialTest(const std::string& SceneName);
	virtual ~MaterialTest();

	virtual void OnInit() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdate(const Hubie::TimeStep& timeStep) override;
	virtual void Render2D() override;
	virtual void OnImGui() override;
	void LoadModels();
};
