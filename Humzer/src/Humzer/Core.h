#pragma once

#ifdef HUM_PLATFORM_WINDOWS
    #ifdef HUM_BUILD_DLL
        #define HUMZER_API __declspec(dllexport)
    #else
        #define HUMZER_API __declspec(dllimport)
    #endif
#else
    #error Humzel Only supports Windows x64!
#endif
