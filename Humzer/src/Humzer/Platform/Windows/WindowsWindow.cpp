#include "humpch.h"
#include "WindowsWindow.h"
#include "Humzer/Platform/OpenGL/OpenglContext.h"


#include "../../Renderer/Renderer.h"

namespace Humzer {

	void GLFWErrorCallback(int error, const char* description)
	{
		HUM_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(int width, int height, std::string title) {
		return new WindowsWindow(width, height, title);
	}

	WindowsWindow::WindowsWindow(int width, int height, std::string title)
	{
		m_Data.Width = width;
		m_Data.Height = height;
		m_Data.Title = title;

		if (Init())
			HUM_CORE_INFO("Window successfully initialized!");

	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
	}

	void Humzer::WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	int Humzer::WindowsWindow::GetWidth()
	{
		return m_Data.Width;
	}

	int Humzer::WindowsWindow::GetHeight()
	{
		return m_Data.Height;
	}

	void Humzer::WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool Humzer::WindowsWindow::GetVSync() const
	{
		return m_Data.VSync;
	}

	bool WindowsWindow::Init()
	{

		if (!glfwInit()) {
			HUM_CORE_FATAL("GLFW Couldn't be initialized!");
			return false;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (!m_Window) {
			HUM_CORE_FATAL("Could not intialize the window!");
			return false;
		}

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			Renderer::OnWindowResize(width, height);
		});

		glfwSetErrorCallback(static_cast<GLFWerrorfun>(GLFWErrorCallback));

		SetVSync(true);

		return true;
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}
