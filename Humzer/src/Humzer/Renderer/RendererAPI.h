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
		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawArrays(const int num_of_vertices) = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void EnableDepthTesting() = 0;
		virtual void DisableDepthTesting() = 0;

		virtual void EnableDepthMask() = 0;
		virtual void DisableDepthMask() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}
