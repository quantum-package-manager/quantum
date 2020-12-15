#ifndef QUAMTUM_util_HPP_
#define QUAMTUM_util_HPP_

#include "libs/Package/Package.hpp"

int lua_quantum_install(lua_State *L){
    std::string file = lua_tostring(L, 1);
    bool is_already_in_bindir = lua_toboolean(L, 2);

    const char delim = '/';

	std::vector<std::string> out;
	tokenize(file, delim, out);

	std::string name = out[out.size() - 1];
    if(!is_already_in_bindir){
        std::string cmd = "cp ";
        cmd.append(file);
        cmd.append(" ../../bindir/");
        cmd.append(package.name);
        cmd.append("/");
        cmd.append(package.version);
        system(cmd.c_str());
    }

    chdir("../../bin");
    std::string cmd = "ln -s ../bindir/";
    cmd.append(package.name);
    cmd.append("/");
    cmd.append(package.version);
    cmd.append("/");
    cmd.append(file);
    system(cmd.c_str());

    return 1;
}

int lua_make(lua_State *L){
    std::string cmd("make -j$(expr $(nproc) - 1)");

    return !!system(cmd.c_str());
}

#endif // QUAMTUM_util_HPP_