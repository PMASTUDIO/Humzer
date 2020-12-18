#include "humpch.h"
#include "Renderer.h"

namespace Humzer {

		void Renderer::BeginScene()
		{

		}

		void Renderer::EndScene()
		{

		}

		void Renderer::Submit(const Ref<VertexArray>& vertexArray)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}

}
