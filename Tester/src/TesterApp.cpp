#include <Humzer/Humzer.h>

class Tester : public Humzer::Application {
public:
    Tester(){

    }

    ~Tester(){

    }

    void ClientUpdate(Humzer::Timestep dt) {
        // HUM_CLIENT_INFO("Mouse position - X: {0} - Y: {1}", Humzer::Input::GetMouseX(), Humzer::Input::GetMouseY());
    }
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
