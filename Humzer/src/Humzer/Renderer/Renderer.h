#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"

namespace Humzer {

	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		// inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; } // NEEDS TO BE CALLED BEFORE WINDOWS AND CONTEXT CREATION!
	};

}
