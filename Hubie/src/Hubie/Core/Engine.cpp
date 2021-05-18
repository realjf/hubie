#include "hbpch.h"
#include "Engine.h"



namespace Hubie
{

	Engine::Engine()
		: m_MaxFramesPerSecond(1000.0f / 60.0f),
		m_TimeStep(TimeStep(0.0f))
	{

	}

	Engine::~Engine()
	{

	}

}

