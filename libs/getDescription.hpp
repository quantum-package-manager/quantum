#include <iostream>
#include <unistd.h>
#include <fstream>
#include "CheckLua.hpp"

extern "C"
{
#include "lua535/include/lua.h"
#include "lua535/include/lauxlib.h"
#include "lua535/include/lualib.h"
}

int desc(std::string pkg){
    lua_State *L = luaL_newstate();

    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = "/usr/share/quantum/";
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum");
    }

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

    chdir(install_dir.c_str());
    std::string cmd = "curl -LO ";
    cmd.append(repox);
    cmd.append(pkg);
    cmd.append("/quantum.lua");


    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");
    lua_pushstring(L, install_dir.c_str());
    lua_setglobal(L, "install_dir");
    lua_register(L, "quantum_install", lua_quantum_install);
    if (CheckLua(L, r)){
        lua_pushstring(L, "description");
        lua_getglobal(L, "description");
        std::cout << std::endl << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    return 0;
}