#include "SceneHierarchy.h"
#include <imgui/imgui.h>

#include "Humzer/Scene/Entity.h"
#include "Humzer/Scene/Components.h"
#include "glm/gtc/type_ptr.inl"

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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selected = {};

		ImGui::End();

		ImGui::Begin("Contextual Menu");

		if (m_Selected)
			DrawComponents(m_Selected);

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

	void SceneHierarchyPannel::DrawComponents(Entity entity)
	{
		ImGui::AlignTextToFramePadding();

		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, 256);
			memcpy(buffer, tag.c_str(), tag.length());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
				tag = std::string(buffer);
			}

			ImGui::Separator();
		}

		if (entity.HasComponent<TransformComponent>()) {
			auto& position = entity.GetComponent<TransformComponent>().Translation;
			auto& scale = entity.GetComponent<TransformComponent>().Scale;
			auto& rotation = entity.GetComponent<TransformComponent>().Rotation;

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::Columns(2);

				// TRANSLATION
				ImGui::Text("Translation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::DragFloat3("##Translation", glm::value_ptr(position), 0.1f);

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				// SCALE
				ImGui::Text("Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.1f);

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				// ROTATION
				ImGui::Text("Rotation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::DragFloat3("##Rotation", glm::value_ptr(rotation), 0.1f);

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Columns(1); // Reset to 1 column

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(CameraComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				const char* projectionTypeStr[] = { "Perspective", "Orthographic" }; // Available options
				const char* currentProjectionTypeString = projectionTypeStr[(int)camera.GetProjectionType()]; // Current value

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {

						bool isSelected = projectionTypeStr[i] == currentProjectionTypeString;
						if (ImGui::Selectable(projectionTypeStr[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStr[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i); // Set projection
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus(); // Set UI to selected
					}
					
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					
					ImGui::Columns(2);

					// SIZE
					ImGui::Text("Size");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("##Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);
					

					ImGui::PopItemWidth();
					ImGui::NextColumn();

					// NEAR
					ImGui::Text("Near Clip");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("##Near", &nearClip))
						camera.SetOrthographicNearClip(nearClip);


					ImGui::PopItemWidth();
					ImGui::NextColumn();

					// FAR
					ImGui::Text("Far Clip");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("##Far", &farClip))
						camera.SetOrthographicFarClip(farClip);


					ImGui::PopItemWidth();
					ImGui::NextColumn();

					ImGui::Columns(1); // Reset columns
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {

					ImGui::Columns(2);

					// SIZE
					ImGui::Text("FOV");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float fov = camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("##FOV", &fov))
						camera.SetPerspectiveFOV(fov);


					ImGui::PopItemWidth();
					ImGui::NextColumn();

					// NEAR
					ImGui::Text("Near Clip");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float nearClip = camera.GetPerspectiveNear();
					if (ImGui::DragFloat("##Near", &nearClip))
						camera.SetPerspectiveNear(nearClip);


					ImGui::PopItemWidth();
					ImGui::NextColumn();

					// FAR
					ImGui::Text("Far Clip");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					float farClip = camera.GetPerspectiveFar();
					if (ImGui::DragFloat("##Far", &farClip))
						camera.SetPerspectiveFar(farClip);


					ImGui::PopItemWidth();
					ImGui::NextColumn();

					ImGui::Columns(1); // Reset columns
				
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		/*

		if (entity.HasComponent<TagComponent>()) {

		}

		if (entity.HasComponent<TagComponent>()) {

		}

		if (entity.HasComponent<TagComponent>()) {

		}*/

	}

}
