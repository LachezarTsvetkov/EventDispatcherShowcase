workspace "EventDispatcher"
	architecture "x64"

	configurations
	{
		"Debug",
		"Development",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder:

project "EventDispatcher"
	location "EventDispatcher"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "edpch.h"
	pchsource "EventDispatcher/src/edpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/src",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			
		}
		 
	filter "configurations:Debug"
		defines "ED_DEBUG"
		symbols "On"
		buildoptions { "/utf-8" }

	filter "configurations:Development"
		defines "ED_DEVELOPMENT"
		symbols "On"
		buildoptions { "/utf-8" }

	filter "configurations:Release"
		defines "ED_RELEASE"
		symbols "Off"
		buildoptions { "/utf-8" }