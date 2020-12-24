#include <Humzer/Humzer.h>

using namespace Humzer;

class Tester : public Humzer::Application {
public:
    Tester(){
		CheckerboardTexture = Humzer::Texture2D::Create("Resources/textures/Checkerboard.png");
        testMesh = CreateRef<Mesh>("Resources/meshes/cerberus.fbx");
    }

    ~Tester(){

    }

    void ClientUpdate(Humzer::Timestep dt) {
		Renderer3D::DrawPlane({ 0.0, 0.0, 0.0 }, { 1.0, 1.0 }, CheckerboardTexture);
		Renderer3D::DrawCube({ 5.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 });

        testMesh->Render(dt, *basicCam);
    }
private:
	Humzer::Ref<Humzer::Texture2D> CheckerboardTexture;
    Ref<Mesh> testMesh;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
