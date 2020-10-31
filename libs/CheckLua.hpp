#ifndef QUAMTUM_libs_CheckLua_HPP_
#define QUAMTUM_libs_CheckLua_HPP_

#include <iostream>

extern "C"
{
#include "lua535/include/lua.h"
#include "lua535/include/lauxlib.h"
#include "lua535/include/lualib.h"
}

static inline bool CheckLua(lua_State *L, int r){
  if (r == LUA_OK){
      return true;
  } else {
    std::string errormsg = lua_tostring(L, -1);
    std::cout << errormsg << std::endl;
    return false;
  }
}

#endif // QUAMTUM_libs_CheckLua_HPP_
