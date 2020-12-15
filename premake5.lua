workspace "HumzerWorkspace"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Tester"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Humzer"
    location "Humzer"
    kind "SharedLib"
    language "C++"

    pchheader "humpch.h"
    pchsource "Humzer/src/Humzer/humpch.cpp"

    files { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp" 
    }

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/${prj.name}")

    includedirs { 
        "%{prj.name}/include",
        "%{prj.name}/vendor/spdlog/include"
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
        "Humzer/src"
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
    