#include "humpch.h"

#include "WindowsWindow.h"


namespace Humzer {

	Window* Window::Create(int width, int height, std::string title) {
		return new WindowsWindow(width, height, title);
	}

	WindowsWindow::WindowsWindow(int width, int height, std::string title)
	{
	}

	void Humzer::WindowsWindow::OnUpdate()
	{

	}

	int Humzer::WindowsWindow::GetWidth()
	{
		return 0;
	}

	int Humzer::WindowsWindow::GetHeight()
	{
		return 0;
	}

	void Humzer::WindowsWindow::SetVSync(bool enabled)
	{

	}

	bool Humzer::WindowsWindow::GetVSync() const
	{
		return false;
	}

}
