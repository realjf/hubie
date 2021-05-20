#include "hbpch.h"

#include "GLFWWindow.h"

#include "GLFWKeyCodes.h"

#include "Hubie/Core/OS/Input.h"
#include "Hubie/Core/Application.h"

#include "Hubie/Events/ApplicationEvent.h"
#include "Hubie/Events/MouseEvent.h"
#include "Hubie/Events/KeyEvent.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <imgui.h>


namespace Hubie
{
	static bool s_GLFWInitialized = false;
	static int s_NumGLFWWindows = 0;

	GLFWWindow::GLFWWindow(const WindowProperties& properties)
	{
		m_Init = false;
		m_VSync = properties.VSync;

		HB_INFO("VSync : {0}", m_VSync ? "True" : "False");
		m_HasResized = true;
		m_Data.m_RenderAPI = static_cast<Graphics::RenderAPI>(properties.RenderAPI);

		m_Init = Init(properties);

		//Setting fullscreen overrides width and heigh in Init
		auto propCopy = properties;
		propCopy.Width = m_Data.Width;
		propCopy.Height = m_Data.Height;
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_Handle);

		s_NumGLFWWindows--;

		if (s_NumGLFWWindows < 1)
		{
			s_GLFWInitialized = false;
			glfwTerminate();
		}
	}

	void GLFWWindow::ToggleVSync()
	{
		HB_PROFILE_FUNCTION();
		if (m_VSync)
		{
			SetVSync(false);
		}
		else
		{
			SetVSync(true);
		}

		HB_INFO("VSync : {0}", m_VSync ? "True" : "False");
	}

	void GLFWWindow::SetVSync(bool set)
	{
		HB_PROFILE_FUNCTION();
		if (set)
		{
			m_VSync = true;
			glfwSwapInterval(1);
		}
		else
		{
			m_VSync = false;
			glfwSwapInterval(0);
		}
		HB_INFO("VSync : {0}", m_VSync ? "True" : "False");
	}

	void GLFWWindow::SetWindowTitle(const std::string& title)
	{

	}

	void GLFWWindow::SetBorderlessWindow(bool borderless)
	{

	}

	void GLFWWindow::OnUpdate()
	{

	}

	void GLFWWindow::HideMouse(bool hide)
	{

	}

	void GLFWWindow::SetMousePosition(const Maths::Vector2& pos)
	{

	}

	void GLFWWindow::UpdateCursorImGui()
	{

	}

	void GLFWWindow::ProcessInput()
	{
		HB_PROFILE_SCOPE("GLFW PollEvents");
		glfwPollEvents();
	}

	bool GLFWWindow::Init(const WindowProperties& properties)
	{
		HB_PROFILE_FUNCTION();
		HB_INFO("Creating window - Title : {0}, Width : {1}, Height : {2}", properties.Title, properties.Width, properties.Height);
	}

	void GLFWWindow::SetIcon(const std::string& file, const std::string& smallIconFilePath /*= ""*/)
	{

	}

	void GLFWWindow::MakeDefault()
	{
		CreateFunc = CreateFuncGLFW;
	}

	Hubie::Window* GLFWWindow::CreateFuncGLFW(const WindowProperties& properties)
	{
		return new GLFWWindow(properties);
	}

}
