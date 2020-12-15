#ifndef QUAMTUM_shell_HPP_
#define QUAMTUM_shell_HPP_

#include <string>
#include "libs/Package/Package.hpp"

extern lua_State *L;
extern Package package;


int lua_quantum_install(lua_State *L);
int create(std::string pkg, std::string shell_name, std::string version="default");
int create_shell(std::string shell_name, std::string version="default");
int run_shell(std::string shell_name);

#endif // QUAMTUM_shell_HPP_