#pragma once

#include "humpch.h"

#include "Core.h"

namespace Humzer {

	class HUMZER_API Window {
	public:
		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool GetVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(int width, int height, std::string title);
	};

}
