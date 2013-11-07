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
      "../demos/simplest/**"
      }
    includedirs {
      "../core",
      "../audiosource/**"
    }

		configuration "Debug"
			defines { "DEBUG" }
			flags {"Symbols" }
			objdir (_ACTION .. "/debug")
			targetname "simplest_d"
			flags { "Symbols" }
			links {"StaticLib", "winmm"}
			

		configuration "Release"
			defines { "NDEBUG" }
			flags {"Optimize"}
			objdir (_ACTION .. "/release")
			targetname "simplest_x86"
			flags { "EnableSSE2", "NoMinimalRebuild", "OptimizeSpeed", "NoEditAndContinue", "No64BitChecks" }
			links {"StaticLib", "winmm"}

-- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< -- 8< --

  project "winmm"
    kind "StaticLib"
    language "C++"
    files {
      "../backend/winmm/**"
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
			