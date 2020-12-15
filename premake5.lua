workspace "HumzerWorkspace"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Tester"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Includes relative to solution directory
IncludeDir = {}
IncludeDir["glfw"] = "Humzer/vendor/glfw/include"
IncludeDir["glm"] = "Humzer/vendor/GLM"

project "Humzer"
    location "Humzer"
    kind "SharedLib"
    language "C++"

    pchheader "humpch.h"
    pchsource "Humzer/src/humpch.cpp"

    files { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp" ,
    }

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/${prj.name}")

    includedirs { 
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glm}"
    }

    libdirs {
        "GLFW/bin"
    }

    links {
        "GLFW.lib",
        "opengl32.lib"
    }

    filter { "system:windows" }
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "HUM_PLATFORM_WINDOWS",
            "HUM_BUILD_DLL"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Tester")
        }


    filter { "configurations:Debug" }
        defines { "HUM_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "HUM_RELEASE" }
        optimize "On"
    
    filter { "configurations:Dist" }
        defines { "HUM_DIST" }
        optimize "On"



project "Tester"
    location "Tester"
    kind "ConsoleApp"
    language "C++"

    files { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp" 
    }

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/${prj.name}")

    includedirs { 
        "%{prj.name}/include",
        "Humzer/vendor/spdlog/include",
        "Humzer/src",
        "%{IncludeDir.glm}"
    }

    links {
        "Humzer"
    }

    filter { "system:windows" }
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "HUM_PLATFORM_WINDOWS",
        }


    filter { "configurations:Debug" }
        defines { "HUM_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "HUM_RELEASE" }
        optimize "On"
    
    filter { "configurations:Dist" }
        defines { "HUM_DIST" }
        optimize "On"
    