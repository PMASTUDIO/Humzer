#pragma once
#include "glm/gtc/quaternion.hpp"
#include <string>

#include "Humzer/Renderer/Camera.h"
#include "../Renderer/Mesh.h"

namespace Humzer {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name) : Tag(name) {}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }

		operator const char*() const { return Tag.c_str(); }
	};

	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : Translation(translation){}

		glm::mat4 GetTransform() const {
			glm::mat4 rotation =
				glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 }) *
				glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return
				glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	enum class PrimitiveShape {
		CUBE,
		QUAD,
	};

	struct PrimitiveRendererComponent {
		PrimitiveShape Shape = PrimitiveShape::CUBE;
		glm::vec4 Color = glm::vec4{ 1.0, 0.0, 0.0, 1.0 };

		PrimitiveRendererComponent() = default;
		PrimitiveRendererComponent(const PrimitiveRendererComponent&) = default;
		PrimitiveRendererComponent(const PrimitiveShape& shape, const glm::vec4& color) : Shape(shape), Color(color) {}

	};

	struct MeshRendererComponent {
		Ref<Humzer::Mesh> Mesh;

		operator Ref<Humzer::Mesh>() { return Mesh; }
		operator const std::string& () { return Mesh->GetName(); }
	};

	struct CameraComponent {
		PerspectiveCamera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float fov, float nearClip, float farClip, glm::vec3 pos) 
			: Camera(fov, 1280.0f / 720.0f, nearClip, farClip, pos, true) {}

		operator PerspectiveCamera& () { return Camera; }
		operator const PerspectiveCamera& () const { return Camera; }
	};

}
