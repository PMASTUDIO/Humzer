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

		void SetupTheme(bool bStyleDark_, float alpha_);
	};

}
