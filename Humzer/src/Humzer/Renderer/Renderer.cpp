#include "humpch.h"
#include "Renderer.h"

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

}
