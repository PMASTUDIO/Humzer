#include <Humzer/Humzer.h>

class Tester : public Humzer::Application {
public:
    Tester(){

    }

    ~Tester(){

    }
};

Humzer::Application* Humzer::CreateApplication() {
    return new Tester();
}
