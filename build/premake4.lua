newoption {
	trigger		  = "with-portmidi",
	description = "Use portmidi to drive midi keyboard in the piano demo"
}

local buildroot = ""

if _ACTION then buildroot = _ACTION end

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
		
		if (os.is("Windows")) 
		then 
			links {"backend_winmm"} 
		else 
			links {"backend_portaudio"} 
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
			targetname "simplest_x86"
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
      "/libraries/sdl/include"      
    }
    libdirs {
      "/libraries/sdl/lib"      
    }
    

		links {"StaticLib", "sdlmain", "sdl", "backend_sdl"}
		
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
			targetname "mixbusses_x86"
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
      "/libraries/sdl/include"      
    }
    libdirs {
      "/libraries/sdl/lib"      
    }
    
    if _OPTIONS["with-portmidi"] then
    	includedirs {
    	"/libraries/portmidi/pm_common"
    	}
    	defines {"USE_PORTMIDI"}
    	links { "portmidi" }
    end

		links {"StaticLib", "sdlmain", "sdl", "backend_sdl"}
		
		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "piano_d"
			flags { "Symbols" }
	    if _OPTIONS["with-portmidi"] then
	    	libdirs { "/libraries/portmidi/debug" }
	    end
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "piano_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
	    if _OPTIONS["with-portmidi"] then
	    	libdirs { "/libraries/portmidi/release" }
	    end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "multimusic"
    kind "WindowedApp"
    language "C++"
    files {
      "../demos/multimusic/**.c*"
      }
    includedirs {
      "../include",
      "/libraries/sdl/include"      
    }
    libdirs {
      "/libraries/sdl/lib"      
    }

		links {"StaticLib", "sdlmain", "sdl", "backend_sdl"}
		
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
			targetname "multimusic_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "backend_winmm"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/winmm/**.c*"
      }
    includedirs {
      "../include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_winmm_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_winmm_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "backend_xaudio2"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/xaudio2/**.c*"
      }
    includedirs {
      "../include",
      "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_xaudio2_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_xaudio2_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "backend_wasapi"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/wasapi/**.c*"
      }
    includedirs {
      "../include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_wasapi_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_wasapi_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "backend_sdl"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/sdl/**.c*"
      }
    includedirs {
      "../include",
      "/libraries/sdl/include"      
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_sdl_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_sdl_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "backend_portaudio"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/portaudio/**.c*"
      }
    includedirs {
      "../include",
      "/libraries/portaudio/include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_portaudio_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_portaudio_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (not os.is("Windows")) then 

  project "backend_oss"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/oss/**.c*"
      }
    includedirs {
      "../include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_oss_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_oss_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "backend_openal"
    kind "StaticLib"
		targetdir "../lib"
    language "C++"
    files {
      "../backend/openal/**.c*"
      }
    includedirs {
      "../include",
      "/libraries/openal/include"      
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (buildroot .. "/debug")
			targetname "soloud_openal_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (buildroot .. "/release")
			targetname "soloud_openal_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
    
-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "StaticLib"
		kind "StaticLib"
		targetdir "../lib"
		language "C++"
		files 
		{ 
	    "../core/**.c*", 
	    "../filter/**.c*", 
	    "../audiosource/**.c*", 
	  }

		includedirs 
		{
		  "../core",
			"../core/**",
			"../filter",
			"../audiosource/**",
      "../include"
		}


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
			