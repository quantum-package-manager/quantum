#include "remove.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "libs/CheckLua.hpp"
#include "libs/Package/Package.hpp"
#include "libs/installed.hpp"

int remove(std::string pkg){
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = "/usr/share/quantum/";
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum-lua");
    }

    chdir(install_dir.c_str());

    luaL_openlibs(L);

    std::fstream repo;
    std::string repox;
    std::string filePath=install_dir;
    filePath.append("/repo");
    repo.open(filePath,std::ios::in);
    if(repo.is_open()){
        std::string line;
        while(getline(repo, line)){
            repox=line;
        }
    }

    std::string cmd = "curl -LO ";
    cmd.append(repox);
    cmd.append(pkg);
    cmd.append("/quantum.lua");

    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");

    if (CheckLua(L, r)){
        lua_getglobal(L, "package");
        if (lua_istable(L, -1)){
            package.installDir = install_dir;

            lua_pushstring(L, "name");
            lua_gettable(L, -2);
            package.name  = lua_tostring(L, -1);
            lua_pop(L, 1);
                    
            lua_pushstring(L, "version");
            lua_gettable(L, -2);
            package.version  = lua_tostring(L, -1);
            lua_pop(L, 1);

            std::string cmd("mkdir -p bindir/");
            cmd.append(package.name);
            cmd.append("/");
            cmd.append(package.version);
            system(cmd.c_str());

            lua_pushstring(L, "source");
            lua_gettable(L, -2);
            package.source  = lua_tostring(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "git");
            lua_gettable(L, -2);
            package.git  = lua_toboolean(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "checksum");
            lua_gettable(L, -2);
            if(!lua_isnil(L, -1)){
                package.checksum = lua_tostring(L, -1);
            } else{
                package.checksum = "none";
            }
            lua_pop(L, 1);

            package.uninstall();

            chdir(install_dir.c_str());

            cmd = "rm quantum.lua";
            system(cmd.c_str());
        }
    }

    lua_close(L);
        
    return 0;
}