#include "hbpch.h"

#define NOMINMAX
#undef NOGDI
#include <Windows.h>
#include <Windowsx.h>
#define NOGDI

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam) ((short)HIWORD(wParam))
#endif

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
		//int ipdf = GetPixelFormat(hDc);
		PIXELFORMATDESCRIPTOR pfd;
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

	// callback

	void ResizeCallback(Window* window, int32_t width, int32_t height)
	{
		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;

		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	}

	void FocusCallback(Window* window, bool focused)
	{
		Input::Get().SetWindowFocus(focused);
	}

	void MouseButtonCallback(Window* window, int32_t button, int32_t x, int32_t y)
	{
		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;
		HWND hWnd = static_cast<WindowsWindow*>(window)->GetHWND();

		bool down = false;
		Hubie::InputCode::MouseKey mouseKey;
		switch (button)
		{
		case WM_LBUTTONDOWN:
			SetCapture(hWnd);
			mouseKey = Hubie::InputCode::MouseKey::ButtonLeft;
			down = true;
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
			mouseKey = Hubie::InputCode::MouseKey::ButtonLeft;
			down = false;
			break;
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			mouseKey = Hubie::InputCode::MouseKey::ButtonRight;
			down = true;
			break;
		case WM_RBUTTONUP:
			ReleaseCapture();
			mouseKey = Hubie::InputCode::MouseKey::ButtonRight;
			down = false;
			break;
		case WM_MBUTTONDOWN:
			SetCapture(hWnd);
			mouseKey = Hubie::InputCode::MouseKey::ButtonMiddle;
			down = true;
			break;
		case WM_MBUTTONUP:
			ReleaseCapture();
			mouseKey = Hubie::InputCode::MouseKey::ButtonMiddle;
			down = false;
			break;
		}

		if (down)
		{
			MouseButtonPressedEvent event(mouseKey);
			data.EventCallback(event);
		}
		else
		{
			MouseButtonReleasedEvent event(mouseKey);
			data.EventCallback(event);
		}
	}

	void MouseScrollCallback(Window* window, int inSw, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;

		MouseScrolledEvent event(0.0f, static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA));
		data.EventCallback(event);
	}

	void MouseMoveCallback(Window* window, int32_t x, int32_t y)
	{
		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;
		MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
		data.EventCallback(event);
	}

	void CharCallback(Window* window, int32_t key, int32_t flags, UINT message)
	{
		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;

		KeyTypedEvent event(Hubie::InputCode::Key {}, char(key));
		data.EventCallback(event);
	}

	void KeyCallback(Window* window, int32_t key, int32_t flags, UINT message)
	{
		bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
		bool repeat = (flags >> 30) & 1;

		WindowsWindow::WindowData& data = static_cast<WindowsWindow*>(window)->m_Data;

		if (pressed)
		{
			KeyPressedEvent event(WindowsKeyCodes::WindowsKeyToHubie(key), repeat ? 1 : 0);
			data.EventCallback(event);
		}
		else
		{
			KeyReleasedEvent event(WindowsKeyCodes::WindowsKeyToHubie(key));
			data.EventCallback(event);
		}
	}

	static void ImGuiUpdateMousePos(HWND hWnd)
	{
		ImGuiIO& io = ImGui::GetIO();

		// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
		if (io.WantSetMousePos)
		{
			POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
			::ClientToScreen(hWnd, &pos);
			::SetCursorPos(pos.x, pos.y);
		}

		// Set mouse position
		io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
		POINT pos;
		if (HWND active_window = ::GetForegroundWindow())
			if (active_window == hWnd || ::IsChild(active_window, hWnd))
				if (::GetCursorPos(&pos) && ::ScreenToClient(hWnd, &pos))
					io.MousePos = ImVec2((float)pos.x, (float)pos.y);
	}

	// wndProc
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = NULL;
		Window* window = Application::Get().GetWindow();
		if (window == nullptr)
			return DefWindowProc(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_ACTIVATE:
		{
			if (!HIWORD(wParam)) // Is minimized
			{
				// active
			}
			else
			{
				// inactive
			}

			return 0;
		}
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		case WM_SETFOCUS:
			FocusCallback(window, true);
			break;
		case WM_KILLFOCUS:
			FocusCallback(window, false);
			break;
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			KeyCallback(window, int32_t(wParam), int32_t(lParam), message);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			MouseButtonCallback(window, message, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEMOVE:
			MouseMoveCallback(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEWHEEL:
			MouseScrollCallback(window, WM_VSCROLL, wParam, lParam);
			break;
		case WM_SIZE:
			ResizeCallback(window, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_CHAR:
		case WM_SYSCHAR:
		case WM_UNICHAR:
			CharCallback(window, int32_t(wParam), int32_t(lParam), message);
			break;
		case WM_SETCURSOR:
			ImGuiUpdateMousePos(hWnd);
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}
		return result;
	}
}