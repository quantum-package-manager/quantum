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
        links {"libs/lua535/lua53", "sqlite3"}

        filter "release*"
            optimize "On"
