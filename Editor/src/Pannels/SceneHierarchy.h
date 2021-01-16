#pragma once
#include "Humzer/Core/Core.h"
#include "Humzer/Scene/Scene.h"
#include "Humzer/Scene/Entity.h"

namespace Humzer {
	
	class SceneHierarchyPannel {
	public:
		SceneHierarchyPannel() = default;
		SceneHierarchyPannel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		void SetSelectedEntity(Entity entity) { m_Selected = entity; }
		Entity GetSelectedEntity() const { return m_Selected; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;

		Entity m_Selected;
		
	};

}
