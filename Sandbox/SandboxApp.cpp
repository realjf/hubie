#include <HubieEngine.h>
#include <Hubie/Core/EntryPoint.h>
#include "Scenes/Scene3D.h"
#include "Scenes/GraphicsScene.h"
#include "Scenes/SceneModelViewer.h"
#include "Scenes/MaterialTest.h"

using namespace Hubie;

class Game : public Application
{
    public:
	explicit Game()
		: Application(std::string("/Sandbox/"), std::string("Sandbox"))
	{
        Application::Get().GetWindow()->SetWindowTitle("Sandbox");
	}

	~Game()
	{
	}
    
    void OnEvent(Event& e) override
    {
        Application::OnEvent(e);
    }

	void Init() override
	{
		Application::Init();
        Application::SetEditorState(EditorState::Play);
        Application::Get().GetWindow()->SetEventCallback(BIND_EVENT_FN(Game::OnEvent));
        
        //GetSceneManager()->EnqueueScene<Scene3D>(std::string("Physics"));
		//GetSceneManager()->EnqueueScene<SceneModelViewer>(std::string("SceneModelViewer"));
		//GetSceneManager()->EnqueueScene<GraphicsScene>(std::string("Terrain"));
		//GetSceneManager()->EnqueueScene<MaterialTest>(std::string("Material"));
	}
    
    void OnImGui() override
    {
        ImGui::Begin("Metrics");
        ImGui::Text("FPS : %.2f",(float)Hubie::Engine::Get().Statistics().FramesPerSecond );
        ImGui::End();
        Application::OnImGui();
    }
};

Hubie::Application* Hubie::CreateApplication()
{
	return new Game();
}
