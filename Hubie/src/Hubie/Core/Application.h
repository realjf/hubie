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
		virtual void OnImGui();

		inline AppState GetState() const
		{
			return m_CurrentState;
		}

		inline EditorState GetEditorState() const
		{
			return m_EditorState;
		}

		inline void SetAppState(AppState state)
		{
			m_CurrentState = state;
		}

		inline void SetEditorState(EditorState state)
		{
			m_EditorState = state;
		}


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
		bool OnWindowClose(WindowCloseEvent& e);


		uint32_t Width, Height;
		bool Fullscreen;
		bool VSync;
		bool Borderless = false;
		bool ShowConsole = true;
		std::string Title;
		int RenderAPI;
		std::string FilePath;



		static Application* s_Instance;

		AppState m_CurrentState = AppState::Loading;
		EditorState m_EditorState = EditorState::Preview;
		AppType m_AppType = AppType::Editor;

		NONCOPYABLE(Application);
	};

	Application* CreateApplication();
}


