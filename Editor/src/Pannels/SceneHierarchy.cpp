#include "SceneHierarchy.h"
#include <imgui/imgui.h>

#include "Humzer/Scene/Entity.h"
#include "Humzer/Scene/Components.h"

namespace Humzer {

	SceneHierarchyPannel::SceneHierarchyPannel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPannel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPannel::OnImGuiRender()
	{
		ImGui::Begin("Scene Tree");

		// Iterate through the scene entities
		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, m_Context.get() };

			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void SceneHierarchyPannel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ((m_Selected == entity) ? ImGuiTreeNodeFlags_Selected : 0); // Set selected if it is selected
		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked()) { // Previous item is clicked
			m_Selected = entity;
		}

		if (expanded) {
			ImGui::TreePop();
		}
	}

}
