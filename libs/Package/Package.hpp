#include <iostream>
#include <string>

extern "C"
{
#include "../lua535/include/lua.h"
#include "../lua535/include/lauxlib.h"
#include "../lua535/include/lualib.h"
}

struct Package{
    Package(lua_State *L) : L(L) {};

    std::string name;
    std::string version;
    std::string summary;
    std::string description;
    std::string source;
    lua_State *L;

    bool download();
    bool build();
    bool install();
};