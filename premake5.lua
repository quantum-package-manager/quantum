solution "quantum"
    configurations {
        "release",
        "debug"
    }

    project "quantum"
        language "C++"
        kind "ConsoleApp"
        targetdir 'build/%{cfg.buildcfg}'
        warnings "Extra"
        files {"**.cpp"}
        includedirs {"."}
        links {"lua", "sqlite3"}

        filter "release*"
            optimize "On"
