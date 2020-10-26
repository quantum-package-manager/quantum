#include <iostream>
#include <string>
#include <unistd.h>

extern "C"
{
#include "lua535/include/lua.h"
#include "lua535/include/lauxlib.h"
#include "lua535/include/lualib.h"
}

struct pkg {
  std::string name;
  std::string version;
  std::string summary;
  std::string description;
  std::string source;
};
pkg package;

bool CheckLua(lua_State *L, int r){
  if (r == LUA_OK){
      return true;
  } else {
    std::string errormsg = lua_tostring(L, -1);
    std::cout << errormsg << std::endl;
    return false;
  }
}

int lua_quantum_install(lua_State *L){
  // add checks later - api call: lua_gettop returns the number of items on the stack
  std::string file = lua_tostring(L, 1);

  std::string cmd = "mkdir bindir/"+package.name;
  const char *command = cmd.c_str();
  system(command);

  chdir("builddir/");

  cmd = "git clone "+package.source;
  command = cmd.c_str();
  system(command);

  chdir(package.name.c_str());

  cmd = "cp "+file+" ../../bindir/"+package.name;
  command = cmd.c_str();
  system(command);

  chdir("../../bin");

  cmd = "ln -s ../bindir/"+package.name+"/* .";
  command = cmd.c_str();
  system(command);

  chdir("..");
  return 1;
}

int build() {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  lua_register(L, "quantum_install", lua_quantum_install);

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

      
    }

    lua_getglobal(L, "install");
    if (lua_isfunction(L, -1)){
      



      lua_pushstring(L, package.name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
  }

  lua_close(L);
  return 0;
}
