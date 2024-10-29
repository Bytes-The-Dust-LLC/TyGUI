
include "../BTDSTD3"

IMGUI_INCLUDE = "Venders/ImGUI"
IMGUI_BACKEND_INCLUDE = "Venders/ImGUI/backends"

---The ImGUI implementation for handling ImGUI widgets in BTD projects
project "TyGUI"
kind "StaticLib"
language "C++"

files 
{
---base code
"./includes/**.h",
"./src/**.c",
"./includes/**.hpp",
"./src/**.cpp",

"Venders/ImGUI/imconfig.h",
"Venders/ImGUI/imgui.h",
"Venders/ImGUI/imgui.cpp",
"Venders/ImGUI/imgui_draw.cpp",
"Venders/ImGUI/imgui_internal.h",
"Venders/ImGUI/imgui_tables.cpp",
"Venders/ImGUI/imgui_widgets.cpp",
"Venders/ImGUI/imstb_rectpack.h",
"Venders/ImGUI/imstb_textedit.h",
"Venders/ImGUI/imstb_truetype.h",
"Venders/ImGUI/imgui_demo.cpp",
"Venders/ImGUI/backends/imgui_impl_sdl2.cpp",
"Venders/ImGUI/backends/imgui_impl_vulkan.cpp",
}

includedirs 
{
---base code
"./includes",

"../" .. BTD_INCLUDE,
"../BTDSTD3/" .. GLM_INCLUDE,
"../BTDSTD3/" .. FMT_INCLUDE,
"../BTDSTD3/" .. SDL_INCLUDE,

"../BTDSTD3/" .. VK_BOOTSTRAP_INCLUDE,
"../BTDSTD3/" .. STB_INCLUDE,
"../BTDSTD3/" .. VOLK_INCLUDE,
"../BTDSTD3/" .. VMA_INCLUDE,
VULKAN_SDK_MANUAL_OVERRIDE,

"Venders/ImGUI",
}

links
{
"BTDSTD",
}


defines
{
"IMGUI_IMPL_VULKAN_USE_VOLK",
"GLM_ENABLE_EXPERIMENTAL",
"VK_NO_PROTOTYPES",
"GLM_FORCE_DEPTH_ZERO_TO_ONE",
"GLM_FORCE_RADIANS",
}


flags
{
"MultiProcessorCompile",
"NoRuntimeChecks",
}


buildoptions
{
"/utf-8",
}


--platforms
filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"Window_Build",
"VK_USE_PLATFORM_WIN32_KHR",
"Desktop_Build",
}

filter "system:linux"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"Linux_Build",
"VK_USE_PLATFORM_XLIB_KHR",
"Desktop_Build",
}


    filter "system:mac"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"


defines
{
"MacOS_Build",
"VK_USE_PLATFORM_MACOS_MVK",
"Desktop_Build",
}

--configs
filter "configurations:Debug"
    defines "BTD_DEBUG"
    symbols "On"

filter "configurations:Release"
    defines "BTD_RELEASE"
    optimize "On"


filter "configurations:Dist"
    defines "BTD_DIST"
    optimize "On"


defines
{
"NDEBUG",
}


flags
{
"LinkTimeOptimization",
}