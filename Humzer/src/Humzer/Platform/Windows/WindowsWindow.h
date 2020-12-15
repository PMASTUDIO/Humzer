#pragma once

#include "../../Core/Window.h"

namespace Humzer {

	class HUMZER_API WindowsWindow : public Window {
	public:
		WindowsWindow(int width, int height, std::string title);

		void OnUpdate() override;

		int GetWidth() override;
		int GetHeight() override;

		void SetVSync(bool enabled) override;
		bool GetVSync() const override;

	};

}
