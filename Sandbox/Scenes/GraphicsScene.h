#pragma once
#include <HubieEngine.h>

class GraphicsScene : public Hubie::Scene
{
public:
	GraphicsScene(const std::string& SceneName);
	virtual ~GraphicsScene();

	virtual void OnInit() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdate(const Hubie::TimeStep& timeStep) override;

	void LoadModels();
    
private:
};
