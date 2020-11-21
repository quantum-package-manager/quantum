#ifndef QUAMTUM_remove_HPP_
#define QUAMTUM_remove_HPP_

#include <string>
#include "libs/Package/Package.hpp"

extern lua_State *L;
extern Package package;

int remove(std::string pkg);

#endif // QUAMTUM_install_HPP_
