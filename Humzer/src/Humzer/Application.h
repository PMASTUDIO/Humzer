#pragma once

#include "Core.h"

namespace Humzer
{
    class HUMZER_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    Application* CreateApplication();

} // namespace Humzer
