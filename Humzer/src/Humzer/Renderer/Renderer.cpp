#include "humpch.h"
#include "Renderer.h"
#include "PrimitiveData.h"

namespace Humzer {

		Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

		void Renderer::BeginScene(PerspectiveCamera& camera)
		{
			s_SceneData->ViewProjectionMatrix = camera.GetViewProjection();
		}

		void Renderer::EndScene()
		{
		}

		void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::mat4 transform)
		{
			shader->Bind();
			shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
			shader->SetMat4("u_Transform", transform);

			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}

		// RENDERER 3D

		struct Renderer3DStorage {
			Ref<VertexArray> PlaneVAO;
			Ref<VertexArray> CubeVAO;

			Ref<Shader> FlatColorShader;
		};

		static Renderer3DStorage* s_Data;

		void Renderer3D::Init()
		{
			s_Data = new Renderer3DStorage();

			BufferLayout base_layout = {
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float3, "a_Normals"},
				{ ShaderDataType::Float4, "a_Color"},
				{ ShaderDataType::Float2, "a_TexCoord"},
			};

			std::vector<float> vertices = {};
			std::vector<uint32_t> indices = {};

			// --- PLANE SET UP ---
			s_Data->PlaneVAO = VertexArray::Create();

			Primitive::getQuadData(vertices, indices);
			Ref<VertexBuffer> PlaneVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
			PlaneVBO->SetLayout(base_layout);
			s_Data->PlaneVAO->AddVertexBuffer(PlaneVBO);

			Ref<IndexBuffer> PlaneEBO = IndexBuffer::Create(&indices[0], indices.size());
			s_Data->PlaneVAO->SetIndexBuffer(PlaneEBO);

			// --- CUBE SET UP ---
			s_Data->CubeVAO = VertexArray::Create();

			Primitive::getCubeData(vertices, indices);
			Ref<VertexBuffer> CubeVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
			CubeVBO->SetLayout(base_layout);
			s_Data->CubeVAO->AddVertexBuffer(CubeVBO);

			Ref<IndexBuffer> CubeEBO = IndexBuffer::Create(&indices[0], indices.size());
			s_Data->CubeVAO->SetIndexBuffer(CubeEBO);

			// --- SHADERS SET UP ---
			s_Data->FlatColorShader = Shader::Create("Resources/shaders/flat_colored.vs", "Resources/shaders/flat_colored.fs");
		}

		void Renderer3D::Shutdown()
		{
			delete s_Data;
		}

		void Renderer3D::BeginScene(PerspectiveCamera& camera)
		{
			s_Data->FlatColorShader->Bind();
			s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		}

		void Renderer3D::EndScene()
		{

		}

		void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{
			s_Data->FlatColorShader->Bind();
			s_Data->FlatColorShader->SetFloat4("u_Color", color);
			s_Data->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0));

			s_Data->PlaneVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->PlaneVAO);
		}

		void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
		{
			s_Data->FlatColorShader->Bind();
			s_Data->FlatColorShader->SetFloat4("u_Color", color);
			s_Data->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0));

			s_Data->CubeVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->CubeVAO);
		}

}
