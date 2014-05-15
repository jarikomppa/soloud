-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

local sdl_root       = "/libraries/sdl"
local portmidi_root  = "/libraries/portmidi"
local dxsdk_root     = "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)"
local portaudio_root = "/libraries/portaudio"
local openal_root    = "/libraries/openal"

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

local sdl_include       = sdl_root .. "/include"      
local sdl_lib           = sdl_root .. "/lib"      
local portmidi_include  = portmidi_root .. "/pm_common"
local portmidi_debug    = portmidi_root .. "/debug"
local portmidi_release  = portmidi_root .. "/release"
local dxsdk_include     = dxsdk_root .. "/include"
local portaudio_include = portaudio_root .. "/include"
local openal_include    = openal_root .. "/include"

local buildroot = ""
if _ACTION then buildroot = _ACTION end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

newoption {
	trigger		  = "with-portmidi",
	description = "Use portmidi to drive midi keyboard in the piano demo"
}

newoption {
	trigger		  = "with-xaudio2",
	description = "Include xaudio2 in build"
}

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

solution "SoLoud"
  location(buildroot)
	configurations { "Debug", "Release" }
	targetdir "../bin"
	if _PREMAKE_VERSION ~= "4.3" then
		debugdir "../bin"
	end
	flags { "NoExceptions", "NoRTTI", "NoPCH" }		
	if (os.is("Windows")) then defines { "_CRT_SECURE_NO_WARNINGS" } end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "simplest"
    kind "ConsoleApp"
    language "C++"
    files {
      "../demos/simplest/**.c*"
      }
    includedirs {
      "../include"
    }

		links {"StaticLib"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "simplest_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "simplest"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "mixbusses"
    kind "WindowedApp"
    language "C++"
    files {
      "../demos/mixbusses/**.c*"
      }
    includedirs {
      "../include",
      sdl_include
    }
    libdirs {
      sdl_lib
    }
    

		links {"StaticLib", "sdlmain", "sdl"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "mixbusses_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "mixbusses"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "piano"
    kind "WindowedApp"
    language "C++"
    files {
      "../demos/piano/**.c*"
      }
    includedirs {
      "../include",
      sdl_include
    }
    libdirs {
      sdl_lib
    }
    
    if _OPTIONS["with-portmidi"] then
    	includedirs {
    	portmidi_include
    	}
    	defines {"USE_PORTMIDI"}
    	links { "portmidi" }
    end

		links {"StaticLib", "sdlmain", "sdl"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "piano_d"
			flags { "Symbols" }
	    if _OPTIONS["with-portmidi"] then
	    	libdirs { portmidi_debug }
	    end
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "piano"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
	    if _OPTIONS["with-portmidi"] then
	    	libdirs { portmidi_release }
	    end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "env"
    kind "WindowedApp"
    language "C++"
    files {
      "../demos/env/**.c*"
      }
    includedirs {
      "../include",
      sdl_include   
    }
    libdirs {
      sdl_lib
    }

		links {"StaticLib", "sdlmain", "sdl"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "env_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "env"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "multimusic"
    kind "WindowedApp"
    language "C++"
    files {
      "../demos/multimusic/**.c*"
      }
    includedirs {
      "../include",
      sdl_include 
    }
    libdirs {
      sdl_lib
    }

		links {"StaticLib", "sdlmain", "sdl"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "multimusic_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "multimusic"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "StaticLib"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"
		files 
		{ 
	    "../src/audiosource/**.c*",
	    "../src/filter/**.c*",
	    "../src/core/**.c*"	    
	  }

		includedirs 
		{
		  "../src/**",
      "../include"
		}

    files {
      "../src/backend/openal/**.c*"
      }
    includedirs {
      "../include",
      openal_include
    }

if (not os.is("Windows")) then 
    files {
      "../src/backend/oss/**.c*"
      }
    includedirs {
      "../include"
    }    
end

    files {
      "../src/backend/portaudio/**.c*"
      }
    includedirs {
      "../include",
      portaudio_include
    }

    files {
      "../src/backend/sdl/**.c*"
      }
    includedirs {
      "../include",
      sdl_include
    }
    
if (os.is("Windows")) then 
    files {
      "../src/backend/wasapi/**.c*"
      }
    includedirs {
      "../include"
    }

if _OPTIONS["with-xaudio2"] then
  	defines {"USE_XAUDIO2"}
end    
    files {
      "../src/backend/xaudio2/**.c*"
      }
    includedirs {
      "../include",
      dxsdk_include
    }
    files {
      "../src/backend/winmm/**.c*"
      }
    includedirs {
      "../include"
    }
    
    
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "codegen"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/codegen/**.cpp"
		}
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "codegen_d"
			flags { "Symbols" }			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "codegen"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "c_test"
    kind "ConsoleApp"
    language "C++"
    files {
      "../demos/c_test/**.c*",
      "../src/c_api/soloud_c.cpp"
      }
    includedirs {
      "../include"
    }

		links {"StaticLib"}
		

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "c_test_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "c_test"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "SharedLib"
		kind "SharedLib"
		targetdir "../lib"
		language "C++"
		files 
		{ 
	      "../src/c_api/**.c*"
  	    }

		includedirs 
		{
		  "../src/**",
          "../include"
		}

		links {"StaticLib"}

if (os.is("Windows")) then 
	linkoptions { "/DEF:\"../../src/c_api/soloud.def\"" }
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_x86_d"
			implibdir("../lib")
			implibname "soloud_dll_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")			
			targetname "soloud_x86"
			implibdir("../lib")
			implibname("soloud_dll_x86")
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
