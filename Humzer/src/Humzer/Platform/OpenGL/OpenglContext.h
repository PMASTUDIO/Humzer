#pragma once

#include "Humzer/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Humzer {
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window);

		virtual bool Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_Window;
	};
}
