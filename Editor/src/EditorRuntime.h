#pragma once

#include <Humzer/Humzer.h>
#include "Humzer/Renderer/EditorCamera.h"

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

		void RenderTools();

		void GuizmosRender();

		std::pair<float, float> GetMouseViewportSpace();
	private:
		Ref<Scene> m_ActiveScene;
		Ref<Texture2D> m_CheckerboardTexture;

		Ref<Framebuffer> m_Framebuffer;

		int m_GuizmoOperation = -1; // No Guizmos
		bool m_SnappingEnabled = false;
		int m_TransformMode = 0;

		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportBounds[2] = { {0.0f, 0.0f}, {0.0f, 0.0f} };

		// Panels
		Ref<SceneHierarchyPannel> m_SceneHierarchyPannel;
	};

}
