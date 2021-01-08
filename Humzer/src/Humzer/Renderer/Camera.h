#pragma once

#include "../Core/Timestep.h"

#include <glm/glm.hpp>

namespace Humzer {

	class Subscriber;

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_ProjectionMatrix(projection) {}

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0);
	};

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float verticalFOV, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		void OnUpdate(Timestep ts);

		void OnResize(void* windowsData);
		void ResizeBounds(uint32_t width, uint32_t height);

		/*const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }
		const glm::vec3& GetPosition() const { return m_Position; }*/

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; UpdateMatrices(); }
		float GetNear() const { return m_NearClip; }
		void SetNear(float nearClip) { m_NearClip = nearClip; UpdateMatrices(); }
		float GetFar() const { return m_FarClip; }
		void SetFar(float farClip) { m_FarClip = farClip; UpdateMatrices(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; UpdateMatrices(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; UpdateMatrices(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; UpdateMatrices(); }


		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; UpdateMatrices(); }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }

	private:
		void UpdateMatrices();
		void UpdateCameraVectors();
		void ProcessMouseMovent(glm::vec2 offset, bool constrainPitch = true);
	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		float m_OrthographicSize = 1.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		//glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		//glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

		/*glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraRight = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		glm::vec3 m_WorldUp = { 0.0f, 1.0f, 0.0f };

		glm::vec2 m_MouseLastPos = { 0.0f, 0.0f };*/

		/*float m_MouseSensitivity = SENSITIVITY;
		bool m_FirstFrame = true;

		float m_Yaw = YAW;
		float m_Pitch = PITCH;

		bool m_MoveControlsEnabled;*/

		Subscriber* m_EventsSubscriber;
	};
}
