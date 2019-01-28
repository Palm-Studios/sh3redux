-- premake5.lua for SILENT HILL 3: Redux
-- Based on the example one found on Github

workspace "silenthill3"
    architecture "x86_64"
    configurations {"Debug", "Release", "Distro"}
    language "C++"

project "sh3r"
    kind "ConsoleApp"
    targetdir ("build/sh3r/%{cfg.buildcfg}_%{cfg.archtecture}")
    objdir ("obj/sh3r/%{cfg.buildcfg}_%{cfg.archtecture}")
    files {"source/SH3/**.cpp", "source/main.cpp", "include/SH3/**.hpp"}

    
    includedirs {"include", "third_party/debugbreak"}

    
    filter {"system:windows"}
        libdirs {"libs/SDL2-2.0.9/build", "libs/zlib-1.2.11/build", "libs/glew-2.1.0/build/lib"}
        links {"zlibstatic", "glew32", "mingw32", "SDL2main", "SDL2"}
        sysincludedirs {"libs/SDL2-2.0.9/include", "libs/glew-2.1.0/include", "libs/boost_1_69_0", "libs/glm/include"}
        links{"OpenGL32"}

    filter {"system:not windows"}
        libdirs {"libs/SDL2-2.0.9/build", "libs/zlib-1.2.11/build", "libs/glew-2.1.0/build/lib"}
        sysincludedirs {"libs/SDL2-2.0.9/include", "libs/glew-2.1.0/include", "libs/boost_1_69_0", "libs/glm/include"}
        links {"z", "SDL2", "GLEW"}
        links {"GL"}

    filter "configurations:Debug"
        defines {"SH3_DEBUG"}
        symbols "On"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}   

    filter "configurations:Release"
        defines {""}
        symbols "Off"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}

    filter "configurations:Distro"
        defines {""}
        symbols "Off"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}

project "tex"
    kind "ConsoleApp"
    targetdir ("build/sh3r/%{cfg.buildcfg}_%{cfg.architecture}")
    objdir ("obj/sh3r/%{cfg.buildcfg}_%{cfg.architecture}")
    files {"source/SH3/**.cpp", "tests/tex.cpp", "include/SH3/**.hpp"}

    
    includedirs {"include", "third_party/debugbreak"}

    
    filter {"system:windows"}
        libdirs {"libs/SDL2-2.0.9/build", "libs/zlib-1.2.11/build", "libs/glew-2.1.0/build/lib", }
        links {"zlibstatic", "glew32", "mingw32", "SDL2main", "SDL2"}
        sysincludedirs {"libs/SDL2-2.0.9/include", "libs/glew-2.1.0/include", "libs/boost_1_69_0", "libs/glm/include"}
        links{"OpenGL32"}

    filter {"system:not windows"}
        libdirs {"libs/SDL2-2.0.9/build", "libs/zlib-1.2.11/build", "libs/glew-2.1.0/build/lib"}
        sysincludedirs {"libs/SDL2-2.0.9/include", "libs/glew-2.1.0/include", "libs/boost_1_69_0", "libs/glm/include"}
        links {"z", "SDL2", "GLEW"}
        links {"GL"}

    filter "configurations:Debug"
        defines {"SH3_DEBUG"}
        symbols "On"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}   

    filter "configurations:Release"
        defines {""}
        symbols "Off"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}

    filter "configurations:Distro"
        defines {""}
        symbols "Off"
        cppdialect "C++17"
        staticruntime "On"
        buildoptions {"-Wall", "-Wextra", "-pedantic", "-Wsign-compare", "-Wold-style-cast", "-Wdeprecated", "-Wconversion", "-Wnon-virtual-dtor", "-Wundef", "-Wfloat-equal", "-Wunreachable-code"}
