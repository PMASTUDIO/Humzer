#include <Humzer/Humzer.h>

using namespace Humzer;

class Tester : public Humzer::Application {
public:
    Tester(){
        GunTexture = Humzer::Texture2D::Create("Resources/textures/Cerberus/cerberus_A.png");
        testMesh = CreateRef<Mesh>("Resources/meshes/CerberusMaterials.fbx");
    }

    ~Tester(){

    }

    void ClientUpdate(Humzer::Timestep dt) {
		//Renderer3D::DrawPlane({ 0.0, 0.0, 0.0 }, { 1.0, 1.0 }, CheckerboardTexture);
		//Renderer3D::DrawCube({ 5.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 });

        testMesh->Render(dt, *basicCam, GunTexture);
    }
private:
	Humzer::Ref<Humzer::Texture2D> GunTexture;
    Ref<Mesh> testMesh;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
