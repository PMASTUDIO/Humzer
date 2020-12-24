project "GLAD"
    kind "StaticLib"
    staticruntime "On"
    language "C"

    files { 
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs {
        "include"
    }

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/${prj.name}")

    filter { "system:windows" }
        systemversion "latest"

    