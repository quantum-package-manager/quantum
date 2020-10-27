#include "libs/Package/Package.hpp"
#include "libs/CheckLua.hpp"
#include <unistd.h>
#include <iostream>

extern "C"
{
#include "libs/lua535/include/lua.h"
#include "libs/lua535/include/lauxlib.h"
#include "libs/lua535/include/lualib.h"
}

// add checks later - api call: lua_gettop returns the number of items on the stack
Package::Package(lua_State *L) : L(L) {}

bool Package::download() {
    chdir("builddir/");
    std::string cmd("git clone ");
    cmd.append(source);
    return !!system(cmd.c_str());
}

bool Package::build(){
    lua_getglobal(L, "build");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
    return true;
}

bool Package::install(){
    lua_getglobal(L, "install");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
    return true;
}

int Package::lua_quantum_install(){
    std::string file = lua_tostring(L, 1);
    std::string cmd("cp ");
    cmd.append(file);
    cmd.append(" ../../bindir/");
    cmd.append(name);
    return !!system(cmd.c_str());
}