#include "hbpch.h"
#include "Application.h"

namespace Hubie
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
	}

	Application::~Application()
	{

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

		if (m_CurrentState == AppState::Closing)
			return false;
		

		return m_CurrentState != AppState::Closing;
	}

	void Application::Quit()
	{

	}

	void Application::Init()
	{

	}

	void Application::OnEvent(Event& e)
	{

	}

	void Application::OnUpdate(const TimeStep& dt)
	{

	}

}