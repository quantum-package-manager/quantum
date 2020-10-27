#include <iostream>

extern "C"
{
#include "lua535/include/lua.h"
#include "lua535/include/lauxlib.h"
#include "lua535/include/lualib.h"
}

bool CheckLua(lua_State *L, int r){
  if (r == LUA_OK){
      return true;
  } else {
    std::string errormsg = lua_tostring(L, -1);
    std::cout << errormsg << std::endl;
    return false;
  }
}