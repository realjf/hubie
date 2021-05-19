#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Scene/SceneManager.h"
#include "Hubie/Events/ApplicationEvent.h"
#include "Hubie/Utilities/TimeStep.h"

namespace Hubie
{
	enum class AppState
	{
		Running,
		Loading,
		Closing,
	};

	enum class EditorState
	{
		Paused,
		Play,
		Next,
		Preview
	};

	enum class AppType
	{
		Game,
		Editor
	};

	class HB_EXPORT Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		bool OnFrame();

		virtual void Quit();
		virtual void Init();
		virtual void OnEvent(Event& e);
		virtual void OnUpdate(const TimeStep& dt);


		static Application& Get()
		{
			return *s_Instance;
		}

		static void Release()
		{
			if (s_Instance)
				delete s_Instance;
			s_Instance = nullptr;
		}

	private:
		static Application* s_Instance;

		AppState m_CurrentState = AppState::Loading;
		EditorState m_EditorState = EditorState::Preview;

		NONCOPYABLE(Application);
	};

	Application* CreateApplication();
}


