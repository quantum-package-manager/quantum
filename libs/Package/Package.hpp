#include <string>

extern "C"
{
#include "../lua535/include/lua.h"
#include "../lua535/include/lauxlib.h"
#include "../lua535/include/lualib.h"
}

struct Package {
  std::string name;
  std::string version;
  std::string summary;
  std::string description;
  std::string source;
  std::string repo;
  lua_State *L;

  Package(lua_State *L);
  bool download();
  bool build();
  bool install();
  int lua_quantum_install();
};