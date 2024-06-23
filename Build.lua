-- premake5.lua
workspace "SoftwarePathTracer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SoftwarePathTracer"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "SoftwarePathTracer-Core"
	include "Core/Build-Core.lua"
group ""

include "SoftwarePathTracer/Build-SoftwarePathTracer.lua"