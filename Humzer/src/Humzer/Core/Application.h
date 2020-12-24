#pragma once

#include "Core.h"
#include "Window.h"

// TESTING
#include <Humzer\Renderer\Buffer.h>
#include "..\Renderer\Camera.h"

namespace Humzer
{
    class HUMZER_API Application {
    public:
        Application();
        virtual ~Application();

        virtual void ClientUpdate(Timestep dt){}

        void Run();

        Window& GetWindow() { return *m_Window; }

        static Application& Get() { return *s_Instance; }

    private:
        std::unique_ptr<Window> m_Window;

        bool m_Running = true;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;

    protected:
        PerspectiveCamera* basicCam;
    };

    // DEFINED IN CLIENT
    Application* CreateApplication();

} // namespace Humzer
