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
		void KeyPressed();

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Ref<Scene> m_ActiveScene;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize;

		// Panels
		Ref<SceneHierarchyPannel> m_SceneHierarchyPannel;
	};

}
