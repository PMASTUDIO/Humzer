#include "humpch.h"
#include "ImGuiRuntime.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../Core/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "../../Editor/src/Helpers/IconsFontAwesome5.h"


namespace Humzer {

	ImGuiRuntime::ImGuiRuntime()
	{

	}

	ImGuiRuntime::~ImGuiRuntime()
	{

	}

	void ImGuiRuntime::OnStart()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		io.Fonts->AddFontFromFileTTF("Resources/fonts/Roboto/Roboto-Bold.ttf", 16.0f); // Font 0
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/fonts/Roboto/Roboto-Regular.ttf", 16.0f); // Font 1

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/fonts/Font-Awesome/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges); // Merged icon fonts

		ImGui::StyleColorsDark();

		// When view ports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetupTheme(true, 1.0f);

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiRuntime::OnShutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiRuntime::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiRuntime::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiRuntime::SetupTheme(bool bStyleDark_, float alpha_)
	{
		auto& colors = ImGui::GetStyle().Colors;

		// CAUTION: REMEMBER THAT THE SUMS BELOW COULD CAUSE A COLOR TO BE ABOVE 1.0F
		ImVec4 primary = ImVec4{ 0.1f, 0.09f, 0.09f, 1.0f };
		ImVec4 secondary = ImVec4{ 0.53f, 0.45f, 0.17f, 1.0f };
		ImVec4 terceary = ImVec4{ 0.0f, 0.87f, 0.5f, 1.0f };

		colors[ImGuiCol_WindowBg] = primary;

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ primary.x + 0.1f, primary.y + 0.1f, primary.z + 0.1f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ primary.x + 0.2f, primary.y + 0.2f, primary.z + 0.2f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ primary.x - 0.4f, primary.y - 0.4f, primary.z - 0.4f, 1.0f };
		
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ primary.x + 0.1f, primary.y + 0.1f, primary.z + 0.1f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ primary.x + 0.2f, primary.y + 0.2f, primary.z + 0.2f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ primary.x - 0.3f, primary.y - 0.3f, primary.z - 0.3f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ primary.x + 0.1f, primary.y + 0.1f, primary.z + 0.1f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ primary.x + 0.2f, primary.y + 0.2f, primary.z + 0.2f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ primary.x - 0.3f, primary.y - 0.3f, primary.z - 0.3f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = secondary;
		colors[ImGuiCol_TabHovered] = ImVec4{ secondary.x + 0.3f, secondary.y + 0.3f, secondary.z + 0.3f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ secondary.x + 0.2f, secondary.y + 0.2f, secondary.z + 0.2f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = secondary;
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ secondary.x + 0.05f, secondary.y + 0.05f, secondary.z + 0.05f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ secondary.x - 0.2f, secondary.y - 0.2f, secondary.z - 0.2f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ secondary.x - 0.2f, secondary.y - 0.2f, secondary.z - 0.2f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ secondary.x + 0.4f, secondary.y + 0.4f, secondary.z + 0.4f, 1.0f };

		// Separators
		colors[ImGuiCol_SeparatorHovered] = terceary;
		colors[ImGuiCol_SeparatorActive] = ImVec4{ terceary.x + 0.1f, terceary.y + 0.1f, terceary.z + 0.1f, 1.0f };

		// Checkbox
		colors[ImGuiCol_CheckMark] = terceary;
	}

}