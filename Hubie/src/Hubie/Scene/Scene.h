#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Utilities/AssetManager.h"

#include <sol/forward.hpp>
#include <cereal/cereal.hpp>

#include <entt/entity/registry.hpp>

#include <string>


namespace Hubie
{
	class TimeStep;
	class Event;
	class WindowResizeEvent;

	class HB_EXPORT Scene
	{
	public:
		explicit Scene(CONST std::string& SceneName);
		virtual ~Scene();

		virtual void OnInit();

		virtual void OnCleanupScene();

		virtual void Render3D()
		{}

		virtual void Render2D()
		{}

		virtual void OnUpdate(const TimeStep& timeStep);
		virtual void OnImGui() {}
		virtual void OnEvent(Event& e);



	protected:
		std::string m_SceneName;
		int m_SceneSerialisationVersion = 0;

		uint32_t m_ScreenWidth;
		uint32_t m_ScreenHeight;

		bool m_HasCppClass = true;

	private:
		NONCOPYABLE(Scene);

		bool OnWindowResize(WindowResizeEvent& e);

		friend class Entity;
	};
}