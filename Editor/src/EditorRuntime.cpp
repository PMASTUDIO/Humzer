#include "EditorRuntime.h"

namespace Humzer {

	EditorRuntime::EditorRuntime()
	{
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
	}

	void EditorRuntime::OnUpdate(Timestep ts)
	{
		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();

		mainScene->OnUpdate(ts);

		m_Framebuffer->Unbind();

		if (Input::IsKeyPressed(Key::Escape)) {
			Application::Get().Quit();
		}
	}

	void EditorRuntime::OnShutdown()
	{

	}

}

