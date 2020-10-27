#include <iostream>
#include <string>
#include <unistd.h>
#include "libs/CheckLua.hpp"
#include "libs/Package/Package.hpp"

extern "C"
{
#include "libs/lua535/include/lua.h"
#include "libs/lua535/include/lauxlib.h"
#include "libs/lua535/include/lualib.h"
}

int build() {
  lua_State *L = luaL_newstate();
  Package package{L};
  luaL_openlibs(L);
  
 

  int r = luaL_dofile(L, "quantum.lua");
  
  if (CheckLua(L, r)){
    lua_getglobal(L, "package");
    if (lua_istable(L, -1)){
      lua_pushstring(L, "name");
      lua_gettable(L, -2);
      package.name  = lua_tostring(L, -1);
      lua_pop(L, 1);
      std::cout << "Package Name: " << package.name << std::endl;

      lua_pushstring(L, "version");
      lua_gettable(L, -2);
      package.version  = lua_tostring(L, -1);
      lua_pop(L, 1);
      std::cout << "Package Version: " << package.name << std::endl;

      lua_pushstring(L, "source");
      lua_gettable(L, -2);
      package.source  = lua_tostring(L, -1);
      lua_pop(L, 1);

      lua_register(L, "quantum_install", package.lua_quantum_install);
    }
    
  
  }

  lua_close(L);
  return 0;
}
