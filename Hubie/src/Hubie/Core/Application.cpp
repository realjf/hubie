#include "hbpch.h"
#include "Application.h"

#include "Engine.h"

#include <imgui.h>

namespace Hubie
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Frames(0)
		, m_Updates(0)
		, m_SceneViewWidth(800)
		, m_SceneViewHeight(600)
	{
		HB_PROFILE_FUNCTION();
		HB_ASSERT(!s_Instance, "Application already exists!");

		s_Instance = this;

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
	}

	Application::~Application()
	{
		ImGui::DestroyContext();
	}

	void Application::Run()
	{
		while (OnFrame())
		{

		}
		Quit();
	}

	bool Application::OnFrame()
	{

		auto& stats = Engine::Get().Statistics();
		auto& ts = Engine::GetTimeStep();

		{
			ImGuiIO& io = ImGui::GetIO();
			io.DeltaTime = ts.GetSeconds();

			stats.FrameTime = ts.GetMillis();
		}

		if (m_CurrentState == AppState::Closing)
			return false;
		
		{
			ImGui::NewFrame();
		}

		{
			OnUpdate(ts);
		}

		return m_CurrentState != AppState::Closing;
	}

	void Application::Quit()
	{

	}

	void Application::Init()
	{

	}

	void Application::OnImGui()
	{
		
	}

	void Application::OnEvent(Event& e)
	{

	}

	void Application::OnUpdate(const TimeStep& dt)
	{

	}

}