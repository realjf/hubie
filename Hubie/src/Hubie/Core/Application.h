#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Scene/SceneManager.h"
#include "Hubie/Scene/SystemManager.h"
#include "Hubie/Events/ApplicationEvent.h"
#include "Hubie/Utilities/TimeStep.h"
#include "Hubie/Core/Reference.h"

namespace Hubie
{
	class Timer;
	class Window;
	struct WindowProperties;
	class AudioManager;
	class SystemManager;
	class Editor;
	class Scene;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class ImGuiManager;

	namespace Graphics
	{
		class RenderGraph;
		enum class RenderAPI : uint32_t;
	}

	namespace Maths
	{
		class Vector2;
	}

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


		Window* GetWindow() const
		{
			return m_Window.get();
		}

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

		uint32_t m_Frames = 0;
		uint32_t m_Updates = 0;
		float m_SecondTimer = 0.0f;
		bool m_Minimized = false;
		bool m_SceneActive = true;

		uint32_t m_SceneViewWidth = 0;
		uint32_t m_SceneViewHeight = 0;
		bool m_SceneViewSizeUpdated = false;


		UniqueRef<Window> m_Window;
		UniqueRef<SceneManager> m_SceneManager;
		

		static Application* s_Instance;

		AppState m_CurrentState = AppState::Loading;
		EditorState m_EditorState = EditorState::Preview;
		AppType m_AppType = AppType::Editor;

		NONCOPYABLE(Application);
	};

	Application* CreateApplication();
}


