local WITH_SDL = 0
local WITH_SDL_STATIC = 0
local WITH_SDL2_STATIC = 0
local WITH_PORTAUDIO = 0
local WITH_OPENAL = 0
local WITH_XAUDIO2 = 0
local WITH_WINMM = 0
local WITH_WASAPI = 0
local WITH_ALSA = 0
local WITH_OSS = 0
local WITH_NULL = 1
local WITH_LIBMODPLUG = 0
local WITH_PORTMIDI = 0
local WITH_TOOLS = 0

if (os.is("Windows")) then
	WITH_WINMM = 1
else
	WITH_ALSA = 1
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
	trigger		  = "with-sdl",
	description = "Include SDL backend in build"
}

newoption {
	trigger		  = "with-sdl2",
	description = "Include SDL2 backend in build"
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
	trigger		  = "with-sdlstatic-only",
	description = "Only include sdl that doesn't use dyndll in build"
}

newoption {
	trigger		  = "with-sdl2-only",
	description = "Only include sdl2 in build"
}

newoption {
	trigger		  = "with-sdl2static-only",
	description = "Only include sdl2 that doesn't use dyndll in build"
}

newoption {
	trigger		  = "with-libmodplug",
	description = "Include libmodplug in build"
}

newoption {
	trigger		  = "with-tools",
	description = "Include (optional) tools in build"
}

newoption {
	trigger		  = "soloud-devel",
	description = "Shorthand for options used while developing SoLoud"
}

if _OPTIONS["soloud-devel"] then
    WITH_SDL = 1
    WITH_SDL_STATIC = 0
    WITH_SDL2_STATIC = 0
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
    WITH_TOOLS = 1
    WITH_LIBMODPLUG = 1
    WITH_PORTMIDI = 1
end

if _OPTIONS["with-common-backends"] then
    WITH_SDL = 1
    WITH_SDL_STATIC = 0
    WITH_SDL2_STATIC = 0
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

if _OPTIONS["with-sdl"] then
	WITH_SDL = 1
end

if _OPTIONS["with-sdl2"] then
	WITH_SDL = 1
end

if _OPTIONS["with-wasapi"] then
	WITH_WASAPI = 1
end

if _OPTIONS["with-sdl-only"] then
	WITH_SDL = 1
	WITH_SDL_STATIC = 0
	WITH_SDL2_STATIC = 0
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-sdl2-only"] then
	WITH_SDL = 1
	WITH_SDL_STATIC = 0
	WITH_SDL2_STATIC = 0
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-sdlstatic-only"] then
	WITH_SDL = 0
	WITH_SDL_STATIC = 1
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-sdl2static-only"] then
	WITH_SDL = 0
	WITH_SDL_STATIC = 0
	WITH_SDL2_STATIC = 1
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-sdl2static-only"] then
	WITH_SDL = 0
	WITH_SDL_STATIC = 0
	WITH_SDL2_STATIC = 1
	WITH_PORTAUDIO = 0
	WITH_OPENAL = 0
	WITH_XAUDIO2 = 0
	WITH_WINMM = 0
	WITH_WASAPI = 0
	WITH_OSS = 0
end

if _OPTIONS["with-native-only"] then
	WITH_SDL = 0
	WITH_SDL_STATIC = 0
	WITH_SDL2_STATIC = 0
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

if _OPTIONS["with-libmodplug"] then
	WITH_LIBMODPLUG = 1
end

if _OPTIONS["with-portmidi"] then
	WITH_PORTMIDI = 1
end

if _OPTIONS["with-tools"] then
	WITH_TOOLS = 1
end

print ("")
print ("Active options:")
print ("WITH_SDL        = ", WITH_SDL)
print ("WITH_PORTAUDIO  = ", WITH_PORTAUDIO)
print ("WITH_OPENAL     = ", WITH_OPENAL)
print ("WITH_XAUDIO2    = ", WITH_XAUDIO2)
print ("WITH_WINMM      = ", WITH_WINMM)
print ("WITH_WASAPI     = ", WITH_WASAPI)
print ("WITH_ALSA       = ", WITH_ALSA)
print ("WITH_OSS        = ", WITH_OSS)
print ("WITH_LIBMODPLUG = ", WITH_LIBMODPLUG)
print ("WITH_PORTMIDI   = ", WITH_PORTMIDI)
print ("WITH_TOOLS      = ", WITH_TOOLS)
print ("")


-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

solution "SoLoud"
  location(buildroot)
	configurations { "Debug", "Release" }
	startproject "simplest"	
	targetdir "../bin"
	debugdir "../bin"
	flags { "NoExceptions", "NoRTTI", "NoPCH" }
	if (os.is("Windows")) then defines { "_CRT_SECURE_NO_WARNINGS" } end
    configuration { "x86", "Debug" }
        targetsuffix "_x86_d"   
    configuration { "x86", "Release" }
        targetsuffix "_x86"    
    configuration { "x64", "Debug" }
        targetsuffix "_x64_d"    
    configuration { "x64", "Release" }
        targetsuffix "_x64"

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
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic"}
		if (not os.is("windows")) then
		  links { "pthread" }
		end
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "simplest"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "simplest"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

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
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic"}
		if (not os.is("windows")) then
		  links { "pthread" }
		end
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "welcome"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "welcome"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "null"
	kind "ConsoleApp"
	language "C++"
	files {
	  "../demos/null/**.c*"
	  }
	includedirs {
	  "../include"
	}
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic"}
		if (not os.is("windows")) then
		  links { "pthread" }
		end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "null"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "null"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "enumerate"
	kind "ConsoleApp"
	language "C++"
	files {
	  "../demos/enumerate/**.c*"
	  }
	includedirs {
	  "../include"
	}
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic"}
		if (not os.is("windows")) then
		  links { "pthread" }
		end
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "enumerate"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "enumerate"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (WITH_LIBMODPLUG == 1) then
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
			targetname "libmodplug"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "libmodplug"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
if (WITH_SDL == 1) then

	project "SoloudDemoCommon"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"

	files {
	  "../demos/common/**.c*",
	  "../demos/common/imgui/**.c*",
	  "../demos/common/glew/GL/**.c*"
	  }
	includedirs {
	  "../include",
	  "../demos/common",
	  "../demos/common/imgui",
	  "../demos/common/glew",
	  sdl_include
	}

		configuration "Debug"
			defines { "DEBUG", "GLEW_STATIC"}
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "solouddemocommon"
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG", "GLEW_STATIC" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "solouddemocommon"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end
-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "SoloudStatic"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"

		defines { "MODPLUG_STATIC" }
if (WITH_LIBMODPLUG == 1) then
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

if (WITH_ALSA == 1) then
	defines {"WITH_ALSA"}
	files {
	  "../src/backend/alsa/**.c*"
	  }
	includedirs {
	  "../include"
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

if (WITH_SDL_STATIC == 1) then
		defines { "WITH_SDL_STATIC" }
	files {
	  "../src/backend/sdl_static/**.c*"
	  }
	includedirs {
	  "../include",
	  sdl_include
	}
end

if (WITH_SDL2_STATIC == 1) then
		defines { "WITH_SDL2_STATIC" }
	files {
	  "../src/backend/sdl2_static/**.c*"
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
if (WITH_NULL == 1) then
    defines { "WITH_NULL" }
	files {
	  "../src/backend/null/**.c*"
	  }
	includedirs {
	  "../include"
	}
end    

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_static"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_static"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
if (WITH_TOOLS == 1) then

	project "codegen"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/codegen/**.cpp"
		}
if (WITH_LIBMODPLUG == 1) then
		defines { "WITH_MODPLUG" }
end
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "codegen"
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "codegen"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (WITH_TOOLS == 1) then

	project "tedsid2dump"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/tedsid2dump/**.cpp"
		}
if (WITH_LIBMODPLUG == 1) then
		defines { "WITH_MODPLUG" }
end
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "tedsid2dump"
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "tedsid2dump"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (WITH_TOOLS == 1) then

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
			targetname "resamplerlab"
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "resamplerlab"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (WITH_TOOLS == 1) then

	project "lutgen"
		kind "ConsoleApp"
		language "C++"
		files {
		  "../src/tools/lutgen/**.c*"
		}

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "lutgen"
			flags { "Symbols" }

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "lutgen"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
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

		links {"SoloudStatic"}
		if (not os.is("windows")) then
		  links { "pthread" }
		end
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end
if (WITH_ALSA == 1) then
	links {"asound"}
end


		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "c_test"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "c_test"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "SoloudDynamic"
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

		links {"SoloudStatic"}
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end

if (os.is("Windows")) then
	linkoptions { "/DEF:\"../../src/c_api/soloud.def\"" }
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud"
			implibdir("../lib")
			implibname "soloud"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud"
			implibdir("../lib")
			implibname("soloud")
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

--
--  The rest of the projects require SDL
--

if (WITH_SDL == 1) then

function CommonDemo(_name)
  project(_name)
	kind "WindowedApp"
	language "C++"
	files {
	  "../demos/" .. _name .. "/**.c*"
	  }
	includedirs {
	  "../include",
	  "../demos/common",
	  "../demos/common/imgui",
	  "../demos/common/glew",
	  sdl_include
	}
	libdirs {
	  sdl_lib
	}
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic", "SoloudDemoCommon", "sdlmain", "sdl", "opengl32"}

		configuration "Debug"
			defines { "DEBUG", "GLEW_STATIC" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname (_name)
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG", "GLEW_STATIC" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname (_name)
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  CommonDemo("3dtest")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  CommonDemo("virtualvoices")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  CommonDemo("mixbusses")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  CommonDemo("pewpew")

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
if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic", "sdlmain", "sdl"}
if (WITH_LIBMODPLUG == 1) then
		links {"libmodplug"}
end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "space"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "space"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

   CommonDemo("multimusic")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

   CommonDemo("monotone")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

   CommonDemo("tedsid")

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
  project "piano"
	kind "WindowedApp"
	language "C++"
	files {
	  "../demos/piano/**.c*",
	  "../demos/common/**.c*",
	  "../demos/common/imgui/**.c*",
	  "../demos/common/glew/GL/**.c*"
	  }
	includedirs {
	  "../include",
	  "../demos/common",
	  "../demos/common/imgui",
	  "../demos/common/glew",
	  sdl_include
	}
	libdirs {
	  sdl_lib
	}

if (WITH_ALSA == 1) then
	links {"asound"}
end

	if (WITH_PORTMIDI == 1) then
		includedirs {
		portmidi_include
		}
		defines {"USE_PORTMIDI"}
		links { "portmidi" }
	end

		links {"SoloudStatic", "sdlmain", "sdl", "opengl32"}

		configuration "Debug"
			defines { "DEBUG", "GLEW_STATIC" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "piano"
			flags { "Symbols" }
		if (WITH_PORTMIDI == 1) then
			libdirs { portmidi_debug }
		end


		configuration "Release"
			defines { "NDEBUG", "GLEW_STATIC" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "piano"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
		if (WITH_PORTMIDI == 1) then
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

if (WITH_ALSA == 1) then
	links {"asound"}
end

		links {"SoloudStatic", "sdlmain", "sdl"}

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "env"
			flags { "Symbols" }


		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "env"
			flags { "EnableSSE2", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
