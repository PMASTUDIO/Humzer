workspace "HumzerWorkspace"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Tester"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Includes relative to solution directory
IncludeDir = {}
IncludeDir["glfw"] = "Humzer/vendor/glfw/include"
IncludeDir["glm"] = "Humzer/vendor/GLM"
IncludeDir["glad"] = "Humzer/vendor/GLAD/include"
IncludeDir["stb_image"] = "Humzer/vendor/stb_image"
IncludeDir["assimp"] = "Humzer/vendor/assimp/include"
IncludeDir["entt"] = "Humzer/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Humzer/vendor/yaml-cpp/include"
IncludeDir["imgui"] = "Humzer/vendor/imgui"

IncludeDir["lib_ovr"] = "Humzer/vendor/LibOVR/Include"

project "Humzer"
    location "Humzer"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    pchheader "humpch.h"
    pchsource "Humzer/src/humpch.cpp"

    files { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp" ,
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    targetdir ("bin/" .. output_dir .. "/%{prj.name}")
    objdir ("bin-int/" .. output_dir .. "/${prj.name}")

    includedirs { 
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.lib_ovr}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.imgui}"
    }

    libdirs {
        "GLFW/bin",
        "Humzer/vendor/LibOVR/Lib/Windows/x64/Release/VS2017",
    }

    links {
        "GLFW.lib",
        "GLAD",
        "opengl32.lib",
        "yaml-cpp",
        "LibOVR.lib",
        "ImGui"
    }

    filter { "system:windows" }
        cppdialect "C++17"

        defines {
            "HUM_PLATFORM_WINDOWS",
            "HUM_BUILD_DLL"
        }

    filter { "configurations:Debug" }
        defines { "HUM_DEBUG" }
        symbols "on"

    filter { "configurations:Release" }
        defines { "HUM_RELEASE" }
        optimize "on"
    
    filter { "configurations:Dist" }
        defines { "HUM_DIST" }
        optimize "on"


project "Tester"
    location "Tester"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

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
        "%{IncludeDir.glm}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.imgui}"
    }

    links {
        "Humzer",
    }

    filter { "system:windows" }
        systemversion "latest"

        defines {
            "HUM_PLATFORM_WINDOWS",
        }


    filter { "configurations:Debug" }
        defines { "HUM_DEBUG" }
        symbols "on"

        links
		{
			"Humzer/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}

        postbuildcommands 
        {
            '{COPY} "../Humzer/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
        }


    filter { "configurations:Release" }
        defines { "HUM_RELEASE" }
        optimize "On"

        links
		{
			"Humzer/vendor/assimp/bin/Release/assimp-vc141-mtd.lib"
		}

        postbuildcommands 
        {
            '{COPY} "../Humzer/vendor/assimp/bin/Release/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
        }
    
    filter { "configurations:Dist" }
        defines { "HUM_DIST" }
        optimize "On"

        links
		{
			"Humzer/vendor/assimp/bin/Release/assimp-vc141-mtd.lib"
		}

        postbuildcommands 
        {
            '{COPY} "../Humzer/vendor/assimp/bin/Release/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
        }


group "Dependencies"
    include "Humzer/vendor/GLAD"
    include "Humzer/vendor/yaml-cpp"
    include "Humzer/vendor/imgui"
