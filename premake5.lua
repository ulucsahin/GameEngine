workspace "GameEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GameEngine"
	location "GameEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src/vendor",
		"%{prj.name}/src",
		"%{prj.name}/Dependencies/GLFW_x64/include",
		"%{prj.name}/Dependencies/GLEW/include"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"GENGINE_PLATFORM_WINDOWS",
			"GENGINE_BUILD_DLL",
			"GLEW_STATIC",
			"_CONSOLE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "GENGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GENGINE_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("intermediate/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"GameEngine/src";
	}

	links
	{
		"GameEngine"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"GENGINE_PLATFORM_WINDOWS",
			"GLEW_STATIC",
			"_CONSOLE"
		}

	filter "configurations:Debug"
		defines "GENGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GENGINE_DIST"
		optimize "On"