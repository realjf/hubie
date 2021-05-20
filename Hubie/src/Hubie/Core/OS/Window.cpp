#include "hbpch.h"
#include "Window.h"


namespace Hubie
{

	Hubie::Window* Window::Create(const WindowProperties& properties)
	{
		HB_ASSERT(CreateFunc, "No Windows Create Function");
		return CreateFunc(properties);
	}

	bool Window::Initialise(const WindowProperties& properties)
	{
		return HasInitialised();
	}

	Window* (*Window::CreateFunc)(const WindowProperties&) = NULL;

}