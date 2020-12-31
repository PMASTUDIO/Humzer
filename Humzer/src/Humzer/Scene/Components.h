#pragma once
#include "glm/gtc/quaternion.hpp"
#include <string>

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
		glm::mat4 Transform = glm::mat4(1.0);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform){}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
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

}
