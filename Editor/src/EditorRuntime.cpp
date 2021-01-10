#include "EditorRuntime.h"

#include <imgui/imgui.h>
#include "Pannels/SceneHierarchy.h"
#include "Helpers/IconsFontAwesome5.h"

namespace Humzer {

	EditorRuntime::EditorRuntime()
	{
		m_SceneHierarchyPannel = CreateRef<SceneHierarchyPannel>();

		m_CheckerboardTexture = Texture2D::Create("Resources/textures/Checkerboard.png");

		FramebufferSpecs fbSpecs;
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;

		m_Framebuffer = Framebuffer::Create(fbSpecs);
	}

	EditorRuntime::~EditorRuntime()
	{
		
	}

	void EditorRuntime::OnStart()
	{
	
		mainScene = CreateRef<Scene>();

		//mainScene->SetSkybox(skyboxTexture);

		auto camera = mainScene->CreateEntity("camera");
		camera.GetComponent<TransformComponent>().Translation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		camera.AddComponent<CameraComponent>().Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

		m_Camera = &camera.GetComponent<CameraComponent>().Camera;

		auto redSquare = mainScene->CreateEntity("square1");
		redSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ 0.7f, 0.0f, 0.0f };
		redSquare.GetComponent<TransformComponent>().Scale = glm::vec3{ 0.8f, 0.4f, 1.0f };
		redSquare.AddComponent<SpriteRendererComponent>().Color = { 0.8f, 0.05f, 0.05f, 1.0f };

		auto greenSquare = mainScene->CreateEntity("square2");
		greenSquare.GetComponent<TransformComponent>().Translation = glm::vec3{ -0.3f, 0.0f, 0.0f };
		greenSquare.GetComponent<TransformComponent>().Scale = glm::vec3{ 0.5f, 0.3f, 1.0f };
		greenSquare.AddComponent<SpriteRendererComponent>().Color = { 0.0f, 0.8f, 0.05f, 1.0f };

		auto checkerboard = mainScene->CreateEntity("checkerboard");
		checkerboard.GetComponent<TransformComponent>().Translation = glm::vec3{ 0.0f, 0.0f, -0.1f };
		checkerboard.GetComponent<TransformComponent>().Scale = glm::vec3{ 5.0f, 5.0f, 0.0f };
		checkerboard.AddComponent<SpriteRendererComponent>().Texture = m_CheckerboardTexture;
		checkerboard.GetComponent<SpriteRendererComponent>().TilingFactor = 10.0f;

		// Panels
		m_SceneHierarchyPannel->SetContext(mainScene);
	}

	void EditorRuntime::OnUpdate(Timestep ts)
	{
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();

		mainScene->OnUpdate(ts);

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

		ImGui::Begin("Viewport");
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y) {
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_Camera->ResizeBounds((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		uint32_t textureId = m_Framebuffer->GetColorAttachment();
		ImGui::Image((void*)textureId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::End(); // Ending of dock space

	}

}

