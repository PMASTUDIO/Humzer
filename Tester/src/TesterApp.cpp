#include <Humzer/Humzer.h>

using namespace Humzer;

class Tester : public Humzer::Application {
public:
    Tester(){
        testMesh = CreateRef<Mesh>("Resources/meshes/CerberusMaterials.fbx");
    }

    ~Tester(){

    }

    void ClientUpdate(Humzer::Timestep dt) {
		//Renderer3D::DrawPlane({ 0.0, 0.0, 0.0 }, { 1.0, 1.0 }, CheckerboardTexture);
		Renderer3D::DrawCube({ 5.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 });
        Renderer3D::DrawMesh(testMesh, { 2.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 });

        // testMesh->Render(dt, *basicCam);
    }
private:
    Ref<Mesh> testMesh;
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
