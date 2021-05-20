#include <HubieEngine.h>
#include <Hubie/Core/EntryPoint.h>


using namespace Hubie;

class Game : public Application
{
public:
	explicit Game() {}

	~Game() {}

	void OnEvent()
	{

	}

	void Init() override
	{
		Application::Init();
		Application::SetEditorState(EditorState::Play);
	}

	void OnImGui() override
	{

	}
};


Hubie::Application* Hubie::CreateApplication()
{
	return new Game();
}
