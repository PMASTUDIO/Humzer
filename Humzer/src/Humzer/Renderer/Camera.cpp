#include "humpch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "../Core/Input.h"
#include "../Core/Application.h"
#include "../Events/Subscriber.h"
#include "../Events/Events.h"
#include "../Events/Dispatcher.h"

const int MOVE_SPEED = 50;

namespace Humzer {

	SceneCamera::SceneCamera()
	{
		// LISTEN FOR RESIZE EVENTS
		/*m_EventsSubscriber = new Subscriber(this);
		m_EventsSubscriber->m_Method = std::bind(&SceneCamera::OnResize, this, std::placeholders::_1);
		Dispatcher::AddSpecificEventSubscriber(m_EventsSubscriber, Events::WINDOWS_RESIZED);*/

		UpdateMatrices();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNearClip = nearClip;
		m_PerspectiveFarClip = farClip;
		UpdateMatrices();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		UpdateMatrices();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		UpdateMatrices();
	}

	void SceneCamera::OnResize(void* size)
	{
		glm::vec2& vec2Size = *(glm::vec2*)size;

		m_AspectRatio = vec2Size.x / vec2Size.y;
		UpdateMatrices();
	}

	void SceneCamera::ResizeBounds(uint32_t width, uint32_t height)
	{
		m_AspectRatio = width / height;
		UpdateMatrices();
	}

	void SceneCamera::UpdateMatrices()
	{
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFOV), m_AspectRatio, m_PerspectiveNearClip, m_PerspectiveFarClip);
		}
		else {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
		
	}

}

