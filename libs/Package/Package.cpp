#include "Package.hpp"
#include <iostream>
#include <unistd.h>

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

bool Package::download(){
    chdir("builddir/");
    std::string cmd("git clone ");
    cmd.append(source);

    bool result=!!system(cmd.c_str());
    
    return result;
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