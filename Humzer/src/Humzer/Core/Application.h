#pragma once

#include "Core.h"
#include "Window.h"

// TESTING
#include <Humzer\Renderer\Buffer.h>
#include "..\Renderer\VertexArray.h"
#include "..\Renderer\Shader.h"
#include "..\Renderer\Texture.h"

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

        // TESTING ONLY
        Ref<VertexBuffer> VBO;
        Ref<IndexBuffer> EBO;
        Ref<VertexArray> VAO;
        Ref<Shader> BasicShader;
        Ref<Texture2D> CheckerboardTexture;
    };

    // DEFINED IN CLIENT
    Application* CreateApplication();

} // namespace Humzer
