#include <HubieEngine.h>
#include <Hubie/Core/EntryPoint.h>


using namespace Hubie;

class Game : public Application
{
public:
	explicit Game() 
	{

	}

	~Game() {}

	void OnEvent(Event& e) override
	{
		Application::OnEvent(e);
	}

	void Init() override
	{
		Application::Init();
		Application::SetEditorState(EditorState::Play);
	}

	void OnImGui() override
	{
		ImGui::Begin("Metrics");
		ImGui::Text("FPS : %.2f", (float)Hubie::Engine::Get().Statistics().FramesPerSecond);
		ImGui::End();
		Application::OnImGui();
	}
};


Hubie::Application* Hubie::CreateApplication()
{
	return new Game();
}
