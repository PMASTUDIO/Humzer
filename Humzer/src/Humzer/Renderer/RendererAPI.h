#pragma once

#include <glm/glm.hpp>
#include "../Core/Core.h"
#include "VertexArray.h"

namespace Humzer {

	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1,
			OpenGLES = 2,
			Vulkan = 3,
			Direct3D12 = 4,
			Metal = 5
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		virtual void EnableDepthTesting() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}
