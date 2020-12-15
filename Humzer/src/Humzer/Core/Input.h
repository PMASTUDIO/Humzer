#pragma once

#include <glm/glm.hpp>

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace Humzer {
	class HUMZER_API Input {
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
