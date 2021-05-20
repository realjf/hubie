#include "hbpch.h"

#define NOMINMAX
#undef NOGDI
#include <Windows.h>
#include <Windowsx.h>
#define NOGDI

#include "WindowsWindow.h"
#include "WindowsKeyCodes.h"
#include "Hubie/Core/Application.h"
#include "Hubie/Core/OS/Input.h"
#include "Hubie/Events/ApplicationEvent.h"
#include "Hubie/Utilities/LoadImage.h"

#include <imgui.h>

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Hubie
{

	EXTERN_C IMAGE_DOS_HEADER __ImageBase;

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT)0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT)0x02)
#endif

#ifndef HID_USAGE_GENERIC_KEYBOARD
#define HID_USAGE_GENERIC_KEYBOARD ((USHORT)0x06)
#endif

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
		: hWnd(nullptr)
	{
		m_Init = false;
		m_VSync = properties.VSync;
		SetHasResized(true);
		m_Data.m_RenderAPI = static_cast<Graphics::RenderAPI>(properties.RenderAPI);

		m_Init = Init(properties);

	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::ToggleVSync()
	{
		if (m_VSync)
		{
			m_VSync = false;
			//glfwSwapInterval(0);
		}
		else
		{
			m_VSync = true;
			//glfwSwapInterval(1);
		}
	}

	void WindowsWindow::SetWindowTitle(const std::string& title)
	{
		SetWindowText(hWnd, (LPCWSTR)title.c_str());
	}

	void WindowsWindow::SetBorderlessWindow(bool borderless)
	{

	}

	void WindowsWindow::OnUpdate()
	{

	}

	void WindowsWindow::ProcessInput()
	{

	}

	void WindowsWindow::UpdateCursorImGui()
	{

	}

	void WindowsWindow::SetIcon(const std::string& filePath, const std::string& smallIconFilePath /*= ""*/)
	{

	}

	bool WindowsWindow::Init(const WindowProperties& properties)
	{
		m_Data.Title = properties.Title;
		m_Data.Width = properties.Width;
		m_Data.Height = properties.Height;
		m_Data.Exit = false;

		hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);

		WNDCLASSEXA winClass = {};
		winClass.cbSize = sizeof(WNDCLASSEX);
		winClass.hInstance = hInstance;
		winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		winClass.lpfnWndProc = static_cast<WNDPROC>(WndProc);
		winClass.lpszClassName = properties.Title.c_str();

		winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		winClass.hIcon = winClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		winClass.lpszMenuName = nullptr;
		winClass.style = CS_VREDRAW | CS_HREDRAW;

		DWORD style = WS_POPUP;

		if (!properties.Fullscreen)
			style = WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;

		if (!properties.Borderless)
			style |= WS_BORDER;

		if (!RegisterClassExA(&winClass))
		{
			HB_CRITICAL("Could not register Win32 class!");
			return false;
		}

		RECT size = { 0, 0, (LONG)properties.Width, (LONG)properties.Height };
		AdjustWindowRectEx(&size, style, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		m_Data.Width = size.right - size.left;
		m_Data.Height = size.bottom - size.top;

		int windowLeft = (GetSystemMetrics(SM_CXSCREEN) - m_Data.Width) / 2;
		int windowTop = (GetSystemMetrics(SM_CYSCREEN) - m_Data.Height) / 2;

		if (properties.Fullscreen)
		{
			windowLeft = 0;
			windowTop = 0;
		}

		hWnd = CreateWindow((LPCWSTR)winClass.lpszClassName, (LPCWSTR)properties.Title.c_str(), style, windowLeft, windowTop, m_Data.Width, m_Data.Height, NULL, NULL, hInstance, NULL);

		if (!hWnd)
		{
			HB_CRITICAL("Could not create window!");
			return false;
		}

		hDc = GetDC(hWnd);
		PIXELFORMATDESCRIPTOR pfd = GetPixelFormat(hDc);
		int32_t pixelFormat = ChoosePixelFormat(hDc, &pfd);
		if (pixelFormat)
		{
			if (!SetPixelFormat(hDc, pixelFormat, &pfd))
			{
				HB_CRITICAL("Failed setting pixel format!");
				return false;
			}
		}
		else
		{
			HB_CRITICAL("Failed choosing pixel format!");
			return false;
		}

		SetIcon("/Textures/icon.png", "/Textures/icon32.png");

		ShowWindow(hWnd, SW_SHOW);
		SetFocus(hWnd);
		SetWindowTitle(properties.Title);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		UpdateWindow(hWnd);

		MoveWindow(hWnd, windowLeft, windowTop, m_Data.Width, m_Data.Height, TRUE);

		GetClientRect(hWnd, &clientRect);
		int w = clientRect.right - clientRect.left;
		int h = clientRect.bottom - clientRect.top;

		m_Data.Height = h;
		m_Data.Width = w;

		if (!properties.ShowConsole)
		{
			HWND consoleWindow = GetConsoleWindow();

			ShowWindow(consoleWindow, SW_HIDE);

			SetActiveWindow(hWnd);
		}
		else
		{
			HWND consoleWindow = GetConsoleWindow();

			ShowWindow(consoleWindow, SW_SHOW);
		}

		if (properties.Fullscreen)
		{
			DEVMODE dm;
			memset(&dm, 0, sizeof(dm));
			dm.dmSize = sizeof(dm);
			// use default values from current setting
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
			/*m_data->m_oldScreenWidth = dm.dmPelsWidth;
			m_data->m_oldHeight = dm.dmPelsHeight;
			m_data->m_oldBitsPerPel = dm.dmBitsPerPel;*/

			dm.dmPelsWidth = m_Data.Width;
			dm.dmPelsHeight = m_Data.Height;
			/*if (colorBitsPerPixel)
			{
				dm.dmBitsPerPel = colorBitsPerPixel;
			}*/
			dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

			LONG res = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
			if (res != DISP_CHANGE_SUCCESSFUL)
			{ // try again without forcing display frequency
				dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				res = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
			}
		}

		//Input
		rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
		rid.dwFlags = RIDEV_INPUTSINK;
		rid.hwndTarget = hWnd;
		RegisterRawInputDevices(&rid, 1, sizeof(rid));

		return true;
	}

	void WindowsWindow::MakeDefault()
	{
		CreateFunc = CreateFuncWindows;
	}

	Hubie::Window* WindowsWindow::CreateFuncWindows(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

}