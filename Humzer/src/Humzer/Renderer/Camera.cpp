#include "humpch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/ext/matrix_transform.hpp"

namespace Humzer {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 pos) : 
		m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_Position(pos)
	{
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_ViewMatrix = glm::lookAt(m_Position, cameraFront, cameraUp);
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_ViewportWidth / m_ViewportHeight, m_NearClip, m_FarClip);
	}

	void PerspectiveCamera::OnUpdate(Timestep ts)
	{
		
	}

}

