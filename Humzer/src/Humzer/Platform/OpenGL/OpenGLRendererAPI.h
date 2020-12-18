#pragma once
#include "../../Renderer/RendererAPI.h"

namespace Humzer {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;


		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;


		void EnableDepthTesting() override;

	};

}