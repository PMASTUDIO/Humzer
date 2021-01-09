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
		void ImGuiRender();
	private:
		Ref<Scene> mainScene;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;

		SceneCamera* m_Camera;

		glm::vec2 m_ViewportSize;

		// Panels
		Ref<SceneHierarchyPannel> m_SceneHierarchyPannel;
	};

}
