#pragma once

namespace Humzer {

	class ImGuiRuntime {
	public:
		ImGuiRuntime();
		~ImGuiRuntime();

		void OnStart();
		void OnShutdown();
		
		void Begin();
		void End();
	};

}
