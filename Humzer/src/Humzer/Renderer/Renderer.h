#pragma once

namespace Humzer {

	enum class RendererAPI {
		None = 0,
		OpenGL = 1,
		OpenGLES = 2,
		Vulkan = 3,
		Direct3D12 = 4,
		Metal = 5
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
		inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; } // NEEDS TO BE CALLED BEFORE WINDOWS AND CONTEXT CREATION!
	private:
		static RendererAPI s_RendererAPI;
	};

}
