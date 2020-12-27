#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

namespace Humzer { class Texture2D; }

namespace Humzer {

	class HUMZER_API Renderer {
	public:
		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::mat4 transform);

		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		// inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; } // NEEDS TO BE CALLED BEFORE WINDOWS AND CONTEXT CREATION!

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

	class HUMZER_API Renderer3D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		// IMMEDIATE RENDERER
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);

		static void DrawMesh(Ref<Mesh> mesh, const glm::vec3& position, const glm::vec3& scale);

	private:
		static PerspectiveCamera* s_SceneCamera;
	};

}
