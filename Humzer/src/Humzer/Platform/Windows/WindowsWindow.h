#pragma once

#include "Humzer/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Humzer {

	class HUMZER_API WindowsWindow : public Window {
	public:
		WindowsWindow(int width, int height, std::string title);
		~WindowsWindow();

		void OnUpdate() override;

		int GetWidth() override;
		int GetHeight() override;

		void SetVSync(bool enabled) override;
		bool GetVSync() const override;

		void* GetNativeWindow() const override;

	private:
		bool Init();
		void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;

	};

}
