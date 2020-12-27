#include <Humzer/Humzer.h>

using namespace Humzer;

class Tester : public Humzer::Application {
public:
    Tester() : basicCam(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f, glm::vec3(0.0f, 4.0f, 20.0f), true) {
        testMesh = CreateRef<Mesh>("Resources/meshes/CerberusMaterials.fbx");
    }

    ~Tester(){

    }

    void ClientUpdate(Humzer::Timestep dt) {

        basicCam.OnUpdate(dt);

        Renderer3D::BeginScene(basicCam);
		//Renderer3D::DrawPlane({ 0.0, 0.0, 0.0 }, { 1.0, 1.0 }, CheckerboardTexture);
		Renderer3D::DrawCube({ 5.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 });
        Renderer3D::DrawMesh(testMesh, { 2.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 });
        Renderer3D::EndScene();

        // testMesh->Render(dt, *basicCam);
    }
private:
    PerspectiveCamera basicCam;
    Ref<Mesh> testMesh;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
