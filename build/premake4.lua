local WITH_SDL = 0
local WITH_SDL_NONDYN = 0
local WITH_PORTAUDIO = 0
local WITH_OPENAL = 0
local WITH_XAUDIO2 = 0
local WITH_WINMM = 0
local WITH_WASAPI = 0
local WITH_OSS = 0

if (os.is("Windows")) then
	WITH_WINMM = 1
else
	WITH_OSS = 1
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

local sdl_root       = "/libraries/sdl"
local portmidi_root  = "/libraries/portmidi"
local dxsdk_root     = os.getenv("DXSDK_DIR") and os.getenv("DXSDK_DIR") or "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)"
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
    trigger       = "with-common-backends",
    description   = "Includes common backends in build"
}

newoption {
	trigger		  = "with-portmidi",
	description = "Use PortMidi to drive midi keyboard in the piano demo"
}

newoption {
	trigger		  = "with-openal",
	description = "Include OpenAL backend in build"
}

newoption {
	trigger		  = "with-portaudio",
	description = "Include PortAudio backend in build"
}

newoption {
	trigger		  = "with-wasapi",
	description = "Include WASAPI backend in build"
}

newoption {
	trigger		  = "with-xaudio2",
	description = "Include XAudio2 backend in build"
}

newoption {
	trigger		  = "with-native-only",
	description = "Only native backends (winmm/oss) in build (default)"
}

newoption {
	trigger		  = "with-sdl-only",
	description = "Only include sdl in build"
}

newoption {
	trigger		  = "with-sdlnondyn-only",
	description = "Only include sdl that doesn't use dyndll in build"
}

newoption {
	trigger		  = "with-libmodplug",
	description = "Include libmodplug in build"
}

newoption {
	trigger		  = "with-tools",
	description = "Include (optional) tools in build"
}

if _OPTIONS["with-common-backends"] then
    WITH_SDL = 1
    WITH_SDL_NONDYN = 0
    WITH_PORTAUDIO = 1
    WITH_OPENAL = 1
    WITH_XAUDIO2 = 0
    WITH_WINMM = 0
    WITH_WASAPI = 0
    WITH_OSS = 1
    
    if (os.is("Windows")) then
    	WITH_XAUDIO2 = 0
    	WITH_WINMM = 1
    	WITH_WASAPI = 1
    	WITH_OSS = 0
    end
end

if _OPTIONS["with-xaudio2"] then
	WITH_XAUDIO2 = 1  	
end

if _OPTIONS["with-openal"] then
	WITH_OPENAL = 1  	
end

if _OPTIONS["with-portaudio"] then
	WITH_PORTAUDIO = 1  	
end

if _OPTIONS["with-wasapi"] then
	WITH_WASAPI = 1
end

if _OPTIONS["with-sdl-only"] then
	WITH_SDL = 1
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-sdlnondyn-only"] then
	WITH_SDL = 0
	WITH_SDL_NONDYN = 1
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-native-only"] then
	WITH_SDL = 0
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
	if (os.is("Windows")) then
		WITH_WINMM = 1
	else
	  WITH_OSS = 1
	end
end

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
if _OPTIONS["with-libmodplug"] then
		links {"libmodplug"}
end		
		
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

  project "welcome"
	kind "ConsoleApp"
	language "C++"
	files {
	  "../demos/welcome/**.c*"
	  }
	includedirs {
	  "../include"
	}

		links {"StaticLib"}
if _OPTIONS["with-libmodplug"] then
		links {"libmodplug"}
end		
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "welcome_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "welcome"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if _OPTIONS["with-libmodplug"] then
	project "libmodplug"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"

		defines { "MODPLUG_STATIC" }

		files 
		{ 
		"../ext/libmodplug/src/**.cpp*"
	  }

		includedirs 
		{
		"../ext/libmodplug/src/**"
		}

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "libmodplug_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "libmodplug"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end    

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "StaticLib"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"

		defines { "MODPLUG_STATIC" }
if _OPTIONS["with-libmodplug"] then
		defines { "WITH_MODPLUG" }
end		
		
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
if (WITH_OPENAL == 1) then
	defines {"WITH_OPENAL"}
	files {
	  "../src/backend/openal/**.c*"
	  }
	includedirs {
	  "../include",
	  openal_include
	}
end    

if (WITH_OSS == 1) then 
	defines {"WITH_OSS"}
	files {
	  "../src/backend/oss/**.c*"
	  }
	includedirs {
	  "../include"
	}    
end

if (WITH_PORTAUDIO == 1) then
	defines {"WITH_PORTAUDIO"}
	
	files {
	  "../src/backend/portaudio/**.c*"
	  }
	includedirs {
	  "../include",
	  portaudio_include
	}
end

if (WITH_SDL == 1) then
		defines { "WITH_SDL" }
	files {
	  "../src/backend/sdl/**.c*"
	  }
	includedirs {
	  "../include",
	  sdl_include
	}
end

if (WITH_SDL_NONDYN == 1) then
		defines { "WITH_SDL_NONDYN" }
	files {
	  "../src/backend/sdl_nondyn/**.c*"
	  }
	includedirs {
	  "../include",
	  sdl_include
	}
end
	
if (WITH_WASAPI == 1) then 
		defines { "WITH_WASAPI" }
	files {
	  "../src/backend/wasapi/**.c*"
	  }
	includedirs {
	  "../include"
	}
end

if (WITH_XAUDIO2 == 1) then
	defines {"WITH_XAUDIO2"}
	files {
	  "../src/backend/xaudio2/**.c*"
	  }
	includedirs {
	  "../include",
	  dxsdk_include
	}
end
	
if (WITH_WINMM == 1) then
		defines { "WITH_WINMM" }
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
if _OPTIONS["with-tools"] then

	project "codegen"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/codegen/**.cpp"
		}
if _OPTIONS["with-libmodplug"] then
		defines { "WITH_MODPLUG" }
end		
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
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if _OPTIONS["with-tools"] then

	project "resamplerlab"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/resamplerlab/**.c*"
		}

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "resamplerlab_d"
			flags { "Symbols" }			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "resamplerlab"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

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
if _OPTIONS["with-libmodplug"] then
		links {"libmodplug"}
end		
	

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
if _OPTIONS["with-libmodplug"] then
		links {"libmodplug"}
end

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

-- 
--  The rest of the projects require SDL
-- 

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (WITH_SDL == 1) then

  project "3dtest"
	kind "WindowedApp"
	language "C++"
	files {
	  "../demos/3dtest/**.c*"
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
			targetname "3dtest_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "3dtest"
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

  project "pewpew"
	kind "WindowedApp"
	language "C++"
	files {
	  "../demos/pewpew/**.c*"
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
			targetname "pewpew_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "pewpew"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "space"
	kind "WindowedApp"
	language "C++"
	files {
	  "../demos/space/**.c*"
	  }
	includedirs {
	  "../include",
	  sdl_include   
	}
	libdirs {
	  sdl_lib
	}

		links {"StaticLib", "sdlmain", "sdl"}
if _OPTIONS["with-libmodplug"] then
		links {"libmodplug"}
end		
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "space_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "space"
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

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
