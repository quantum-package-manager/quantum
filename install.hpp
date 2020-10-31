#ifndef QUAMTUM_install_HPP_
#define QUAMTUM_install_HPP_

#include <string>
#include "libs/Package/Package.hpp"

extern lua_State *L;
extern Package package;


int lua_quantum_install(lua_State *L);
int build(std::string pkg);

#endif // QUAMTUM_install_HPP_
