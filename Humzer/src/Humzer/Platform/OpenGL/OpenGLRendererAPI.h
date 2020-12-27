#pragma once
#include "../../Renderer/RendererAPI.h"

namespace Humzer {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;


		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;


		void EnableDepthTesting() override;


		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	};

}