solution "Emulators"
  DIR = path.getabsolute("") .. "/"
  LIBDIR = path.join(DIR, "ThirdParty/")
  location("build/")
  -- Configurations
  configurations{
    "Debug",
    "Release",
  }
  platforms{
    "x32",
    "native"
  }

  project "Chip_8"
    kind("ConsoleApp")
    language "C++"
    files{
      path.join(DIR ,"*.cpp"),
      path.join(DIR, "*.h"),
    }

    GLFW = (LIBDIR .. "glfw-3.2.1.bin.WIN32/")
    GLM = (LIBDIR .. "glm/")
    GLEW = (LIBDIR .. "glew-2.0.0/lib/Release/Win32/")
    -- Global info
    libdirs{
      GLEW,
    }
    includedirs{
      GLFW .. "include/",
      GLM
    }
    debugdir(DIR)

    configuration "Debug"
      targetdir(DIR .. "Debug")
      flags{ "Symbols"}
      defines("DEBUG")

    configuration "Release"
      targetdir(DIR .. "Release")
      flags { "OptimizeSpeed", "No64BitChecks"}
    -- Platform specific
    configuration{_ACTION}
      lib_path = path.join("lib-",_ACTION)
      libdirs{
        path.join(GLFW,lib_path),
      }
      defines{ "GLEW_STATIC"}

    configuration{ "vs*" }
      defines{ "WIN32"}
      pchheader "stdafx.h"
      pchsource "stdafx.cpp"
    configuration{}
