#pragma once

#include "../Core/Timestep.h"

#include <glm/glm.hpp>

namespace Humzer {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip, glm::vec3 pos, bool moveControlsEnabled = false);

		void OnUpdate(Timestep ts);

		void OnResize(float width, float height);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetProjection() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }
		const glm::vec3& GetPosition() const { return m_Position; }
		
	private:
		void UpdateMatrices();
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		bool m_MoveControlsEnabled;
	
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}
