solution "SoLoud"
  location(_ACTION)
	configurations { "Debug", "Release" }
	targetdir "../bin"
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
      "../core",
      "../audiosource/**"
    }

		links {"StaticLib"}
		
		if (os.is("Windows")) then links {"winmm"} else links {"portaudio"} end

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "simplest_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "simplest_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "winmm"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/winmm/**.c*"
      }
    includedirs {
      "../core"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_winmm_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_winmm_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "xaudio2"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/xaudio2/**.c*"
      }
    includedirs {
      "../core",
      "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_xaudio2_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_xaudio2_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (os.is("Windows")) then 

  project "wasapi"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/wasapi/**.c*"
      }
    includedirs {
      "../core"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_wasapi_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_wasapi_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "sdl"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/sdl/**.c*"
      }
    includedirs {
      "../core",
      "/libraries/sdl/include"      
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_sdl_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_sdl_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "portaudio"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/portaudio/**.c*"
      }
    includedirs {
      "../core",
      "/libraries/portaudio/include"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_portaudio_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_portaudio_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

if (not os.is("Windows")) then 

  project "oss"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/oss/**.c*"
      }
    includedirs {
      "../core"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_oss_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_oss_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

end

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "openal"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/openal/**.c*"
      }
    includedirs {
      "../core",
      "/libraries/openal/include"      
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_openal_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_openal_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
    
-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

	project "StaticLib"
		kind "StaticLib"
		language "C++"
		files 
		{ 
			"../core/**.h", 
	    "../core/**.cpp", 
	    "../core/*.c",
	    "../filter/**.h", 
	    "../filter/**.cpp", 
	    "../filter/*.c",
	    "../audiosource/**.h", 
	    "../audiosource/**.cpp", 
	    "../audiosource/*.c" 
	  }

		includedirs 
		{
		  "../core",
			"../core/**",
			"../filter**",
			"../audiosource**" 
		}


		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "soloud_x86_d"
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "soloud_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --
			