#pragma once

#include <Humzer/Humzer.h>

namespace Humzer {

	class EditorRuntime {
	public:
		EditorRuntime();
		~EditorRuntime();

		void OnStart();
		void OnUpdate(Timestep ts);
		void OnShutdown();
	private:
		Ref<Scene> mainScene;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;
	};

}
