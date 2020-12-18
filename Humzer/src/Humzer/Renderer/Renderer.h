#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Humzer {

	class Renderer {
	public:
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::mat4 transform);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		// inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; } // NEEDS TO BE CALLED BEFORE WINDOWS AND CONTEXT CREATION!

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}
