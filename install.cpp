#include "install.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "libs/CheckLua.hpp"
#include "libs/Package/Package.hpp"

lua_State *L = luaL_newstate();
Package package{L};

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
    std::string cmd("make");

    return !!system(cmd.c_str());
}

int build(std::string pkg){
    luaL_openlibs(L);

    lua_register(L, "quantum_install", lua_quantum_install);
    lua_register(L, "make", lua_make);
    std::fstream repo;
    std::string repox;
    repo.open("repo",std::ios::in);
    if(repo.is_open()){
        std::string line;
        while(getline(repo, line)){
            repox=line;
        }
    }

    std::string cmd = "curl -LO ";
    cmd.append(repox);
    cmd.append(pkg);
    cmd.append("/quantum.lua");

    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");
    lua_register(L, "quantum_install", lua_quantum_install);
    if (CheckLua(L, r)){
        lua_getglobal(L, "package");
        if (lua_istable(L, -1)){
            lua_pushstring(L, "name");
            lua_gettable(L, -2);
            package.name  = lua_tostring(L, -1);
            lua_pop(L, 1);
                    
            lua_pushstring(L, "version");
            lua_gettable(L, -2);
            package.version  = lua_tostring(L, -1);
            lua_pop(L, 1);

            std::string cmd("mkdir -p bindir/");
            cmd.append(package.name);
            cmd.append("/");
            cmd.append(package.version);
            system(cmd.c_str());

            lua_pushstring(L, "source");
            lua_gettable(L, -2);
            package.source  = lua_tostring(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "git");
            lua_gettable(L, -2);
            package.git  = lua_toboolean(L, -1);
            lua_pop(L, 1);
            
            lua_pushstring(L, "checksum");
            lua_gettable(L, -2);
            if(!lua_isnil(L, -1)){
                package.checksum = lua_tostring(L, -1);
            } else{
                package.checksum = "none";
            }
            lua_pop(L, 1);
                    
            package.download();
            chdir(package.name.c_str());
            
            package.build();
            package.install();

            chdir("..");

            cmd = "rm -rf builddir/*";
            system(cmd.c_str());


            cmd = "rm quantum.lua";
            system(cmd.c_str());
        }
    }   
    lua_close(L);

    return 0;
}