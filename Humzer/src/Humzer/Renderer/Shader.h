#pragma once

#include "humpch.h"

#include <glm/glm.hpp>

namespace Humzer {

	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<Shader> Create(const std::string & vertexPath, const std::string & fragmentPath);
	};

}
