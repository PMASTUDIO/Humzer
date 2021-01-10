#include "SceneHierarchy.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

		if (ImGui::BeginPopupContextWindow(0, 1, false)) { // Right clicked on empty space
			if (ImGui::MenuItem("Create Entity"))
				m_Context->CreateEntity("New Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Contextual Menu");

		if (m_Selected) {
			DrawComponents(m_Selected);
		
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent")) {
				if (ImGui::MenuItem("Camera")) {
					m_Selected.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer")) {
					m_Selected.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				
				ImGui::EndPopup();
			}
		}

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

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) { // Right clicked on this item
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (expanded) {
			ImGui::TreePop();
		}

		// ONLY DELETE ENTITIES AFTER GUI RENDERING AND CLEAR SELECTION CONTEXT
		if (entityDeleted) {
			m_Context->DestroyEntity(entity);
			if (m_Selected == entity)
				m_Selected = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValues = 0.0f, float columnWidth = 100.0f) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 }); // Tight spacing

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValues;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.4f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValues;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValues;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>()) {
			auto& position = entity.GetComponent<TransformComponent>().Translation;
			auto& scale = entity.GetComponent<TransformComponent>().Scale;

			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");

			if (open)
			{
				DrawVec3Control("Translation", position);
				DrawVec3Control("Scale", scale, 1.0f);

				glm::vec3 degRotation = glm::degrees(entity.GetComponent<TransformComponent>().Rotation);
				DrawVec3Control("Rotation", degRotation);
				entity.GetComponent<TransformComponent>().Rotation = glm::radians(degRotation);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>()) {
			auto& cc = entity.GetComponent<CameraComponent>();
			auto& camera = cc.Camera;

			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(CameraComponent).hash_code()), treeNodeFlags, "Camera"))
			{
				const char* projectionTypeStr[] = { "Perspective", "Orthographic" }; // Available options
				const char* currentProjectionTypeString = projectionTypeStr[(int)camera.GetProjectionType()]; // Current value

				ImGui::Checkbox("Primary", &cc.Primary);

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

					// FOV
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

		if (entity.HasComponent<SpriteRendererComponent>()) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2, 2 });
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");

			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("~", ImVec2{ 20, 20 })) {
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			auto& src = entity.GetComponent<SpriteRendererComponent>();

			if (open)
			{
				ImGui::Columns(2);

				// COLOR
				ImGui::Text("Color");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::ColorEdit4("##Color", glm::value_ptr(src.Color), ImGuiColorEditFlags_DisplayRGB);

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Separator();

				// PATH
				ImGui::Text("Texture (WIP)");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::Text(src.Texture ? src.Texture->GetPath().c_str() : "No Texture");

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				// TILING
				ImGui::Text("Tiling Factor");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				ImGui::DragFloat("##Tiling Factor", &src.TilingFactor);

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Columns(1); // Reset to 1 column

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
		}
		/*
		if (entity.HasComponent<TagComponent>()) {

		}

		if (entity.HasComponent<TagComponent>()) {

		}*/

	}

}
