
#include "humpch.h"

#include "OpenglContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

Humzer::OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Window(window)
{

}

bool Humzer::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		HUM_CORE_FATAL("Could not intialize GLAD!");
		return false;
	}
}

void Humzer::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}
