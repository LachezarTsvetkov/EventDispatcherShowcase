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
	kind "StaticLib"
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
		optimize "Speed"

project "SandboxApp"
	location "SandboxApp"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs 
	{
		"EventDispatcher/src",
		"%{prj.name}/src"
	}
	links
	{
		"EventDispatcher"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
	filter "configurations:Debug"
		defines "ED_DEBUG"
		symbols "On"
	filter "configurations:Development"
		defines "ED_DEVELOPMENT"
		symbols "On"
	filter "configurations:Release"
		defines "ED_RELEASE"
		symbols "Off"
		optimize "Speed"

project "UnitTests"
	location "UnitTests"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"EventDispatcher/src",
		"%{prj.name}/src",
		"EventDispatcher/vendor/googletest/googletest/include"
	}

	links
	{
		"EventDispatcher",
		"GoogleTest"
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
		optimize "Speed"


project "GoogleTest"
	location "EventDispatcher/vendor/googletest/googletest"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
files
	{
		"EventDispatcher/vendor/googletest/googletest/src/gtest-all.cc",
		"EventDispatcher/vendor/googletest/googletest/include/gtest/**.h",
		"EventDispatcher/vendor/googletest/googletest/include/gtest/internal/**.h"
	}
	includedirs
	{
		"EventDispatcher/vendor/googletest/googletest/include",
		"EventDispatcher/vendor/googletest/googletest"
	}
	filter "system:windows"
		systemversion "latest"
	filter "configurations:Debug"
		defines "GTEST_DEBUG"
		symbols "On"
	filter "configurations:Development"
		defines "GTEST_DEVELOPMENT"
		symbols "On"
	filter "configurations:Release"
		defines "GTEST_RELEASE"
		symbols "Off"
		optimize "Speed"