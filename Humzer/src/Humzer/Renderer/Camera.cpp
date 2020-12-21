#include "humpch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "../Core/Input.h"

const int MOVE_SPEED = 50;

namespace Humzer {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 pos, bool moveControlsEnabled) :
		m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Position(pos), m_MoveControlsEnabled(moveControlsEnabled)
	{
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		UpdateMatrices();
	}

	void PerspectiveCamera::OnUpdate(Timestep ts)
	{
		if (m_MoveControlsEnabled) {
			if (Input::IsKeyPressed(Key::W)) {
				m_Position.z -= MOVE_SPEED * ts;
			}
			else if (Input::IsKeyPressed(Key::S)) {
				m_Position.z += MOVE_SPEED * ts;
			}

			if (Input::IsKeyPressed(Key::A)) {
				m_Position.x -= MOVE_SPEED * ts;
			}
			else if (Input::IsKeyPressed(Key::D)) {
				m_Position.x += MOVE_SPEED * ts;
			}

			if (Input::IsKeyPressed(Key::LeftShift)) {
				m_Position.y -= MOVE_SPEED * ts;
			}
			else if (Input::IsKeyPressed(Key::LeftControl)) {
				m_Position.y += MOVE_SPEED * ts;
			}

			UpdateMatrices();
		}
	}

	void PerspectiveCamera::UpdateMatrices()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_ViewportWidth / m_ViewportHeight, m_NearClip, m_FarClip);
	}

}

