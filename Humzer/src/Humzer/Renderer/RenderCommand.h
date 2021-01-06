#pragma once

#include <glm/glm.hpp>
#include "RendererAPI.h"

namespace Humzer {

	class RenderCommand {
	public:
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void EnableDepthTesting()
		{
			s_RendererAPI->EnableDepthTesting();
		}

		inline static void DisableDepthTesting()
		{
			s_RendererAPI->DisableDepthTesting();
		}

		inline static void EnableDepthMask()
		{
			s_RendererAPI->EnableDepthMask();
		}

		inline static void DisableDepthMask()
		{
			s_RendererAPI->DisableDepthMask();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawArrays(const int num_of_vertices)
		{
			s_RendererAPI->DrawArrays(num_of_vertices);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}
