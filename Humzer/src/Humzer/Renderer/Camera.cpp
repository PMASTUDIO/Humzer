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
		/*glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_CameraRight = { 1.0f, 0.0f, 0.0f };*/

		Application::Get().GetWindow().CaptureMouse();

		// LISTEN FOR RESIZE EVENTS
		m_EventsSubscriber = new Subscriber(this);
		m_EventsSubscriber->m_Method = std::bind(&SceneCamera::OnResize, this, std::placeholders::_1);
		Dispatcher::AddSpecificEventSubscriber(m_EventsSubscriber, Events::WINDOWS_RESIZED);

		UpdateMatrices();
		UpdateCameraVectors();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_FOV = verticalFOV;
		m_NearClip = nearClip;
		m_FarClip = farClip;
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		UpdateMatrices();
	}

	void SceneCamera::OnUpdate(Timestep ts)
	{
		/*if (m_MoveControlsEnabled) {
			float speed = MOVE_SPEED * ts;
			if (Input::IsKeyPressed(Key::W)) {
				m_Position += m_CameraFront * speed;
			}
			else if (Input::IsKeyPressed(Key::S)) {
				m_Position -= m_CameraFront * speed;
			}

			if (Input::IsKeyPressed(Key::A)) {
				m_Position -= m_CameraRight * speed;
			}
			else if (Input::IsKeyPressed(Key::D)) {
				m_Position += m_CameraRight * speed;
			}

			if (Input::IsKeyPressed(Key::LeftShift)) {
				m_Position.y -= MOVE_SPEED * ts;
			}
			else if (Input::IsKeyPressed(Key::LeftControl)) {
				m_Position.y += MOVE_SPEED * ts;
			}
			

			if (m_FirstFrame) {
				m_MouseLastPos.x = Input::GetMouseX();
				m_MouseLastPos.y = Input::GetMouseY();
				m_FirstFrame = false;
			}

			glm::vec2 mouseOffset = { Input::GetMouseX() - m_MouseLastPos.x, m_MouseLastPos.y - Input::GetMouseY() };

			m_MouseLastPos.x = Input::GetMouseX();
			m_MouseLastPos.y = Input::GetMouseY();

			ProcessMouseMovent(mouseOffset);

			UpdateMatrices();
		}*/
	}

	void SceneCamera::OnResize(void* size)
	{
		glm::vec2& vec2Size = *(glm::vec2*)size;

		m_AspectRatio = vec2Size.x / vec2Size.y;
		UpdateMatrices();
	}

	void SceneCamera::UpdateMatrices()
	{
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		}
		else {
			// #TODO Orthographic Cam projection set up 

			/*float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);*/
		}

		/*m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);*/
		
	}


	void SceneCamera::UpdateCameraVectors()
	{
		/*glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(front);
		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));*/
	}

	void SceneCamera::ProcessMouseMovent(glm::vec2 offset, bool constrainPitch)
	{
		//offset *= m_MouseSensitivity;

		//m_Yaw += offset.x;
		//m_Pitch += offset.y;

		//// make sure that when pitch is out of bounds, screen doesn't get flipped
		//if (constrainPitch) {
		//	if (m_Pitch > 89.0f)
		//		m_Pitch = 89.0f;
		//	if (m_Pitch < -89.0f)
		//		m_Pitch = -89.0f;
		//}

		//UpdateCameraVectors();
		
	}

}

