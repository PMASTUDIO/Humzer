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
		static void Init();

		static void BeginScene(Camera& camera, const glm::mat4& transform);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::mat4 transform);

		static Ref<ShaderLibrary> GetShaderLibrary();

		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		// inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; } // NEEDS TO BE CALLED BEFORE WINDOWS AND CONTEXT CREATION!

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Ref<ShaderLibrary> s_ShaderLibrary;
		static SceneData* s_SceneData;
	};

	class HUMZER_API Renderer3D {
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Camera& camera, const glm::mat4& transform, const glm::vec3 cameraPos);
		static void EndScene();

		// IMMEDIATE RENDERER
		static void DrawPlane(const glm::mat4 transform, const glm::vec4& color);
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawPlane(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

		static void DrawCube(const glm::mat4 transform, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);

		static void DrawSkybox(const Ref<TextureCube>& texture);

		static void DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh);

		[[deprecated("Transforms should be passed in mat4 form")]]
		static void DrawMesh(Ref<Mesh> mesh, const glm::vec3& position, const glm::vec3& scale);

	private:
		static Camera* s_SceneCamera;
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& camTransform);
		static void EndScene();
		
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
	};

}
