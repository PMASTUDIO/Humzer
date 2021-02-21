#include "EditorRuntime.h"

#include <imgui/imgui.h>
#include "Pannels/SceneHierarchy.h"
#include "Helpers/IconsFontAwesome5.h"
#include "Humzer/Platform/PlatformUtils.h"

#include <ImGuizmo.h>
#include "glm/gtc/type_ptr.hpp"
#include "Humzer/Math/Math.h"
#include "Humzer/Scene/Entity.h"

namespace Humzer {

	EditorRuntime::EditorRuntime()
	{
		m_SceneHierarchyPannel = CreateRef<SceneHierarchyPannel>();

		m_CheckerboardTexture = Texture2D::Create("Resources/textures/Checkerboard.png");

		FramebufferSpecs fbSpecs;
		fbSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::R32I, FramebufferTextureFormat::Depth };
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;

		m_Framebuffer = Framebuffer::Create(fbSpecs);
	}

	EditorRuntime::~EditorRuntime()
	{
		
	}

	void EditorRuntime::OnStart()
	{
	
		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f); // Aspect ratio doesn't matter here

		//mainScene->SetSkybox(skyboxTexture);

		// Panels
		m_SceneHierarchyPannel->SetContext(m_ActiveScene);
	}

	void EditorRuntime::OnUpdate(Timestep ts)
	{
		KeyPressed();

		FramebufferSpecs spec = m_Framebuffer->GetSpecs();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			 (m_ViewportSize.x != spec.Width || m_ViewportSize.y != spec.Height)) {

			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			// Editor camera update
			m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			// m_Camera->ResizeBounds((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		
		m_EditorCamera.OnUpdate(ts);

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();

		// Clear entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update editor scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);


		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		my = m_ViewportSize.y - my;

		int mouseX = (int)mx, mouseY = (int)my;

		m_Framebuffer->Bind();
		if (mouseX >= 0 && mouseY >= 0 && mouseX < m_ViewportSize.x && mouseY < m_ViewportSize.y) {
			int pixel = m_Framebuffer->ReadPixel(1, mx, my);

			m_HoveredEntity = pixel == -1 ? Entity() : Entity((entt::entity)pixel, m_ActiveScene.get());
		
			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft) && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)) {
				m_SceneHierarchyPannel->SetSelectedEntity(m_HoveredEntity);
			}
		
		}
		
		m_Framebuffer->Unbind();

		ImGuiRender();
		
		if (Input::IsKeyPressed(Key::Escape)) {
			Application::Get().Quit();
		}
	}

	void EditorRuntime::OnShutdown()
	{

	}

	void EditorRuntime::ImGuiRender()
	{
		
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWinSizeX = style.WindowMinSize.x;
		
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem(ICON_FA_FOLDER_PLUS" New", "Ctrl+N")) {
					NewScene();
				}

				if (ImGui::MenuItem(ICON_FA_FILE" Open...", "Ctrl+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem(ICON_FA_SAVE" Save As...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) { Application::Get().Quit(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Begin(ICON_FA_INFO_CIRCLE" Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("2D Renderer Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		m_SceneHierarchyPannel->OnImGuiRender();

		ImGui::Begin("Toolbox");
		RenderTools();
		ImGui::End();

		// Viewport
		ImGui::Begin("Viewport");
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		auto viewportOffset = ImGui::GetCursorPos();

		// Draw engine image from framebuffer
		uint32_t textureId = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = {minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		GuizmosRender();
		
		ImGui::End();

		ImGui::End(); // Ending of dock space

	}

	void EditorRuntime::KeyPressed()
	{
		if (Input::IsKeyPressed(Key::LeftControl)) {
			if (Input::IsKeyPressed(Key::O)) {
				OpenScene();
			}
			if (Input::IsKeyPressed(Key::N)) {
				NewScene();
			}
			if (Input::IsKeyPressed(Key::LeftShift)) {
				if (Input::IsKeyPressed(Key::S)) {
					SaveSceneAs();
				}
			}
		}

	}

	void EditorRuntime::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>(); // Create empty scene
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPannel->SetContext(m_ActiveScene);
	}

	void EditorRuntime::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Scene file (*.humscene)\0*.humscene\0");
		if (!filepath.empty()) {
			m_ActiveScene = CreateRef<Scene>(); // Create empty scene
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPannel->SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
		else {
			HUM_CORE_ERROR("Scene could not be opened!");
		}
	}

	void EditorRuntime::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Scene file (*.humscene)\0*.humscene\0");

		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
		else {
			HUM_CORE_ERROR("Scene could not be saved!");
		}
	}

	void EditorRuntime::RenderTools()
	{
		std::string name = "Null";
		if ((entt::entity)m_HoveredEntity != entt::null) {
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		}
		ImGui::Text("Hovered entity: %s", name.c_str());

		if (ImGui::Button(ICON_FA_MOUSE_POINTER)) {
			m_GuizmoOperation = -1;
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_ARROWS_ALT)) {
			m_GuizmoOperation = (int)ImGuizmo::OPERATION::TRANSLATE;
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_SYNC_ALT)) {
			m_GuizmoOperation = (int)ImGuizmo::OPERATION::ROTATE;
		}
		ImGui::SameLine();

		if (ImGui::Button(ICON_FA_EXPAND_ARROWS_ALT)) {
			m_GuizmoOperation = (int)ImGuizmo::OPERATION::SCALE;
		}

		ImGui::Checkbox("Snapping Enabled", &m_SnappingEnabled);
		
		ImGui::Text("Mode"); ImGui::SameLine();
		ImGui::RadioButton("Local", &m_TransformMode, 0); ImGui::SameLine();
		ImGui::RadioButton("World", &m_TransformMode, 1);
	}

	void EditorRuntime::GuizmosRender()
	{
		// Guizmos
		Entity selectedEntity = m_SceneHierarchyPannel->GetSelectedEntity();

		if (selectedEntity && m_GuizmoOperation != -1) {
			// Runtime camera
			/*auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;*/

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Runtime Camera
			/*const glm::mat4& proj = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			// EDITOR CAMERA
			const glm::mat4& proj = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			if (!m_SnappingEnabled)
				m_SnappingEnabled = Input::IsKeyPressed(Key::LeftControl);

			float snapValue = 0.5f; // 0.5 Meters
			if (m_GuizmoOperation == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue }; // Same value for each axis

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(proj), (ImGuizmo::OPERATION)m_GuizmoOperation, (ImGuizmo::MODE)m_TransformMode, glm::value_ptr(transform)
				, nullptr, m_SnappingEnabled ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) { // If there are changes

				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				tc.Translation = translation;
				tc.Scale = scale;

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Rotation += deltaRotation; // Rotation should work as a delta (adding) or else we can end up with a gimball lock (limited rotation).
			}
		}

	}

	std::pair<float, float> EditorRuntime::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;

		//return { (mx / m_ViewportSize.x) * 2.0f - 1.0f, ((my / m_ViewportSize.y) * 2.0f - 1.0f) * -1.0f };
		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

}

