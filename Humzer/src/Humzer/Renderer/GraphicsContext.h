#pragma once

namespace Humzer {

	class GraphicsContext {
	public:
		virtual bool Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}