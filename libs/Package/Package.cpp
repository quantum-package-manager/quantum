#include "libs/Package/Package.hpp"
#include <unistd.h>
#include <iostream>

extern "C"
{
#include "libs/lua535/include/lua.h"
#include "libs/lua535/include/lauxlib.h"
#include "libs/lua535/include/lualib.h"
}

// add checks later - api call: lua_gettop returns the number of items on the stack
Package::Package(const lua_State *L) : L(L) {}

bool Package::download() {
    chdir("builddir/");
    std::string cmd = "git clone "+source;
    command = cmd.c_str();
    system(command);
}

bool Package::build(lua_State *L){
    lua_getglobal(L, "build");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
}

bool Package::install(lua_State *L){
    lua_getglobal(L, "install");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
}

int Package::lua_quantum_install(lua_State *L){
    std::string cmd = "cp "+file+" ../../bindir/"+name;
    const char *command = cmd.c_str();
    system(command);
}