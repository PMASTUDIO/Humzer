#include <Humzer/Humzer.h>
#include "glm/ext/matrix_transform.hpp"

// TEMP
#include "Humzer/Scene/SceneSerializer.h"

using namespace Humzer;

class Tester : public Humzer::Application {
public:
	Tester() /*: basicCam(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 4.0f, 20.0f), true)*/ {
		/*std::vector<std::string> faces{
			 "Resources/textures/sky/right.jpg",
			 "Resources/textures/sky/left.jpg",
			 "Resources/textures/sky/top.jpg",
			 "Resources/textures/sky/bottom.jpg",
			 "Resources/textures/sky/front.jpg",
			 "Resources/textures/sky/back.jpg",
		 };
        skyboxTexture = TextureCube::Create(faces);*/
    }

    ~Tester(){

    }

    void ClientOnStart() {
		mainScene = CreateRef<Scene>();

		//mainScene->SetSkybox(skyboxTexture);

		auto mainCamera = mainScene->CreateEntity("camera");
		mainCamera.GetComponent<TransformComponent>().Translation = glm::vec3{ 0.0f, 4.0f, 20.0f };
		mainCamera.AddComponent<CameraComponent>().Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

		auto cube = mainScene->CreateEntity("square");
		cube.GetComponent<TransformComponent>().Translation = glm::vec3{ 5.0, 0.0, 0.0 };
		cube.AddComponent<PrimitiveRendererComponent>(PrimitiveShape::CUBE, glm::vec4{ 0.0, 1.0, 0.0, 1.0 });

		auto testMesh = MeshLibrary::Load("Cerberus", "Resources/meshes/CerberusMaterials.fbx");

		auto gun = mainScene->CreateEntity("gun");
		gun.AddComponent<MeshRendererComponent>().Mesh = testMesh;
		gun.GetComponent<TransformComponent>().Translation = { 2.0, 0.0, 0.0 };

        // TEMP TEST
		SceneSerializer serializer(mainScene);
        //serializer.Serialize("Resources/scenes/first.humscene");
        //serializer.Deserialize("Resources/scenes/first.humscene");
    }

    void ClientUpdate(Humzer::Timestep dt) {

        mainScene->OnUpdate(dt);

        if (Input::IsKeyPressed(Key::Escape)) {
            Quit();
        }
    }
private:
    //PerspectiveCamera basicCam;
    Ref<Scene> mainScene;
    Ref<TextureCube> skyboxTexture;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
