#pragma once
#include "CameraController.h"

namespace Hubie
{

	class HB_EXPORT ThirdPersonCameraController : public CameraController
	{
	public:
		ThirdPersonCameraController();
		virtual ~ThirdPersonCameraController() override;

		virtual void HandleMouse(Maths::Transform& transform, float dt, float xpos, float ypos) override;
		virtual void HandleKeyboard(Maths::Transform& transform, float dt) override;

	private:
		bool m_Free;
	};
}
