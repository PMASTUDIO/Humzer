#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

namespace Humzer { class Texture2D; }

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

	class Renderer3D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		// PRIMITIVES
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);
	};

}
