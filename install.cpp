#include "install.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "libs/CheckLua.hpp"
#include "libs/Package/Package.hpp"
#include "libs/installed.hpp"
#include "libs/db/get.hpp"
#include "util.hpp"

lua_State *L = luaL_newstate();
Package package{L};

std::vector<std::string> dependencies;

// Big thanks to @Joseph Sible-Reinstate Monica  on StackOverflow for this function.
// Check him out here: https://stackoverflow.com/users/7509065/joseph-sible-reinstate-monica
static int set_dependencies(lua_State *L) {
    dependencies.clear();
    lua_settop(L, 0);
    lua_getglobal(L, "dependencies");
    for(lua_Integer i = 1; lua_geti(L, 1, i) != LUA_TNIL; ++i) {
        size_t len;
        const char *str = lua_tolstring(L, 2, &len);
        if(str) {
            dependencies.push_back(std::string{str, len});
        }
        lua_settop(L, 0);
        lua_getglobal(L, "dependencies");
    }
    return 0;
}

int install_pkg(std::string pkg, std::string version, std::string root){
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = root;
        install_dir.append("/usr/share/quantum/");
        if(root != "/"){
            std::string cmd = "mkdir -p ";
            cmd.append(root);
            cmd.append("/{/bin/,/usr/share/quantum/bindir,/usr/share/quantum/bin,/usr/share/quantum/builddir} && cp /usr/share/quantum/quantum.db ");
            cmd.append(root);
            cmd.append("/usr/share/quantum");
            std::cout << cmd;
            system(cmd.c_str());
        }
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum");
    }

    luaL_openlibs(L);

    lua_register(L, "quantum_install", lua_quantum_install);
    lua_register(L, "make", lua_make);
    /* std::fstream repo;
    std::string repox;
    std::string filePath=install_dir;
    filePath.append("/repo");
    repo.open(filePath,std::ios::in);
    if(repo.is_open()){
        std::string line;
        while(getline(repo, line)){
            repox=line;
        }
    } */

    chdir(install_dir.c_str());

    std::string cmd = "curl -LO ";
    cmd.append(get_url(pkg, "default"));
    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");

    if (lua_istable(L, -1)){
        set_dependencies(L);
        for(const auto &dep : dependencies) {
            build(dep, "default", root);
        }
    }

    std::cout << root;
    build(pkg, version, root);

    return 0;
}

int build(std::string pkg, std::string version, std::string root){
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = root;
        install_dir.append("/usr/share/quantum/");
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum");
    }

    luaL_openlibs(L);

    lua_register(L, "quantum_install", lua_quantum_install);
    lua_register(L, "make", lua_make);
    /* std::fstream repo;
    std::string repox;
    std::string filePath=install_dir;
    filePath.append("/repo");
    repo.open(filePath,std::ios::in);
    if(repo.is_open()){
        std::string line;
        while(getline(repo, line)){
            repox=line;
        }
    } */

    

    chdir(install_dir.c_str());

    std::string cmd = "curl -LO ";
    cmd.append(get_url(pkg, version));
    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");
    lua_register(L, "quantum_install", lua_quantum_install);
    lua_pushstring(L, root.c_str());
    lua_setglobal(L, "root")
    if (CheckLua(L, r)){
        lua_getglobal(L, "package");
        if (lua_istable(L, -1)){
            package.installDir = install_dir;

            lua_pushstring(L, "name");
            lua_gettable(L, -2);
            package.name  = lua_tostring(L, -1);
            lua_pop(L, 1);
                    
            lua_pushstring(L, "version");
            lua_gettable(L, -2);
            package.version  = lua_tostring(L, -1);
            lua_pop(L, 1);

            std::string install_dir_lua = install_dir;
            install_dir_lua.append("/bindir/");
            install_dir_lua.append(pkg);
            install_dir_lua.append("/");
            install_dir_lua.append(package.version);
            lua_pushstring(L, install_dir_lua.c_str());
            lua_setglobal(L, "install_dir");

            lua_pushstring(L, "root");
            lua_gettable(L, -2);
            bool root = lua_toboolean(L, -1);
            lua_pop(L, 1);

            if(root == true) {
                if(install_dir != "/usr/share/quantum/"){
                    std::cout << std::endl << "You must install this package as root." << std::endl;
                    exit (EXIT_FAILURE);
                }
            }

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

            chdir(install_dir.c_str());

            std::string version_file_path="bindir/";
            version_file_path.append(package.name);
            version_file_path.append("/version");

            std::ofstream version_file;
            version_file.open (version_file_path.c_str());
            version_file << package.version;
            version_file.close();

            cmd = "rm -rf builddir/*";
            system(cmd.c_str());

            cmd = "rm quantum.lua";
            system(cmd.c_str());
        }

    
    lua_close(L);
    L = luaL_newstate();

    return 0;
}
}
