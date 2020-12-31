#include <Humzer/Humzer.h>
#include "glm/ext/matrix_transform.hpp"

using namespace Humzer;

class Tester : public Humzer::Application {
public:
    Tester() : basicCam(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 4.0f, 20.0f), true) {
        std::vector<std::string> faces{
            "Resources/textures/sky/right.jpg",
            "Resources/textures/sky/left.jpg",
            "Resources/textures/sky/top.jpg",
            "Resources/textures/sky/bottom.jpg",
            "Resources/textures/sky/front.jpg",
            "Resources/textures/sky/back.jpg",
        };
        skyboxTexture = TextureCube::Create(faces);
    }

    ~Tester(){

    }

    void ClientOnStart() {
        mainScene = CreateRef<Scene>();

        auto cube = mainScene->CreateEntity("square");
        glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0), { 5.0, 0.0, 0.0 });		
        cube.GetComponent<TransformComponent>().Transform = cubeTransform;
        cube.AddComponent<PrimitiveRendererComponent>(PrimitiveShape::CUBE, glm::vec4{ 0.0, 1.0, 0.0, 1.0 });


        testMesh = CreateRef<Mesh>("Resources/meshes/CerberusMaterials.fbx");

		auto gun = mainScene->CreateEntity("gun");
		glm::mat4 gunTransform = glm::translate(glm::mat4(1.0), { 2.0, 0.0, 0.0 });
		gun.AddComponent<MeshRendererComponent>().Mesh = testMesh;
		gun.GetComponent<TransformComponent>().Transform = gunTransform;
    }

    void ClientUpdate(Humzer::Timestep dt) {

        basicCam.OnUpdate(dt);

        Renderer3D::BeginScene(basicCam);
        Renderer3D::DrawSkybox(skyboxTexture);
		
        mainScene->OnUpdate(dt);

        Renderer3D::EndScene();
    }
private:
    PerspectiveCamera basicCam;
    Ref<Mesh> testMesh;
    Ref<TextureCube> skyboxTexture;
    Ref<Scene> mainScene;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
