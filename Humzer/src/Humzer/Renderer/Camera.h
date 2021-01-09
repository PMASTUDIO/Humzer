#pragma once

#include "../Core/Timestep.h"

#include <glm/glm.hpp>

namespace Humzer {

	class Subscriber;

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

		void OnResize(void* windowsData);
		void ResizeBounds(uint32_t width, uint32_t height);

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; UpdateMatrices(); }
		float GetPerspectiveNear() const { return m_PerspectiveNearClip; }
		void SetPerspectiveNear(float nearClip) { m_PerspectiveNearClip = nearClip; UpdateMatrices(); }
		float GetPerspectiveFar() const { return m_PerspectiveFarClip; }
		void SetPerspectiveFar(float farClip) { m_PerspectiveFarClip = farClip; UpdateMatrices(); }

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
	private:
		ProjectionType m_ProjectionType = ProjectionType::Perspective;

		float m_PerspectiveFOV = 45.0f, m_AspectRatio = 0.0f, m_PerspectiveNearClip = 0.01f, m_PerspectiveFarClip = 1000.0f;

		float m_OrthographicSize = 1.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		//Subscriber* m_EventsSubscriber;
	};
}
