#include <Humzer/Humzer.h>
#include "glm/ext/matrix_transform.hpp"

#include <Humzer/Core/EntryPoint.h>

using namespace Humzer;

class Tester : public Humzer::Application {
public:
	Tester() {
		/*std::vector<std::string> faces{
			 "Resources/textures/sky/right.jpg",
			 "Resources/textures/sky/left.jpg",
			 "Resources/textures/sky/top.jpg",
			 "Resources/textures/sky/bottom.jpg",
			 "Resources/textures/sky/front.jpg",
			 "Resources/textures/sky/back.jpg",
		 };
        skyboxTexture = TextureCube::Create(faces);*/
		m_CheckerboardTexture = Texture2D::Create("Resources/textures/Checkerboard.png");
    }

    ~Tester(){

    }

    void ClientOnStart() {

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

		/*auto cube = mainScene->CreateEntity("square");
		cube.GetComponent<TransformComponent>().Translation = glm::vec3{ 5.0, 0.0, 0.0 };
		cube.AddComponent<PrimitiveRendererComponent>(PrimitiveShape::CUBE, glm::vec4{ 0.0, 1.0, 0.0, 1.0 });

		auto testMesh = MeshLibrary::Load("Cerberus", "Resources/meshes/CerberusMaterials.fbx");

		auto gun = mainScene->CreateEntity("gun");
		gun.AddComponent<MeshRendererComponent>().Mesh = testMesh;
		gun.GetComponent<TransformComponent>().Translation = { 2.0, 0.0, 0.0 };*/
    }

    void ClientUpdate(Humzer::Timestep dt) {
		RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		RenderCommand::Clear();

		mainScene->OnUpdateRuntime(dt);

        if (Input::IsKeyPressed(Key::Escape)) {
            Quit();
        }
    }
private:
    Ref<Scene> mainScene;
   /* Ref<TextureCube> skyboxTexture;*/
	Ref<Texture2D> m_CheckerboardTexture;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
