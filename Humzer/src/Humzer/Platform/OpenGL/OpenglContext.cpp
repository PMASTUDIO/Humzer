
#include "humpch.h"

#include "OpenglContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

Humzer::OpenGLContext::OpenGLContext(GLFWwindow* window) : m_Window(window)
{

}

static void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

	HUM_CORE_ERROR("---------------");
	HUM_CORE_ERROR("Debug message ({0}) : {1}", id, message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             HUM_CORE_ERROR("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   HUM_CORE_ERROR("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: HUM_CORE_ERROR("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     HUM_CORE_ERROR("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     HUM_CORE_ERROR("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           HUM_CORE_ERROR("Source: Other"); break;
	};

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               HUM_CORE_ERROR("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: HUM_CORE_ERROR("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  HUM_CORE_ERROR("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         HUM_CORE_ERROR("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         HUM_CORE_ERROR("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              HUM_CORE_ERROR("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          HUM_CORE_ERROR("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           HUM_CORE_ERROR("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               HUM_CORE_ERROR("Type: Other"); break;
	};

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         HUM_CORE_ERROR("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       HUM_CORE_ERROR("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          HUM_CORE_ERROR("Severity: low"); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: HUM_CORE_ERROR("Severity: notification"); return;
	};
}

bool Humzer::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		HUM_CORE_FATAL("Could not intialize GLAD!");
		return false;
	}

#ifdef HUM_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

	return true;
}

void Humzer::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_Window);
}
