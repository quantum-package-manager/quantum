#include "shell.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "libs/CheckLua.hpp"
#include "libs/Package/Package.hpp"
#include "libs/installed.hpp"
#include "libs/db/get.hpp"

// lua_State *L = luaL_newstate();
// Package package{L};

std::vector<std::string> dependenciesx;

// Big thanks to @Joseph Sible-Reinstate Monica  on StackOverflow for this function.
// Check him out here: https://stackoverflow.com/users/7509065/joseph-sible-reinstate-monica
static int set_dependencies(lua_State *L) {
    dependenciesx.clear();
    lua_settop(L, 0);
    lua_getglobal(L, "dependencies");
    for(lua_Integer i = 1; lua_geti(L, 1, i) != LUA_TNIL; ++i) {
        size_t len;
        const char *str = lua_tolstring(L, 2, &len);
        if(str) {
            dependenciesx.push_back(std::string{str, len});
            std::cout << str;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "dependencies");
    }
    return 0;
}

int lua_quantum_install_shell(lua_State *L){
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

int lua_make_shell(lua_State *L){
    std::string cmd("make -j$(expr $(nproc) - 1)");

    return !!system(cmd.c_str());
}

int run_shell(std::string shell_name){
    std::cout << "?";
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = "/usr/share/quantum/shells/";
        install_dir.append(shell_name);
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum/shells/");
        install_dir.append(shell_name);
    }

    
    std::string cmd = "cp ~/.bashrc ";
    cmd.append(install_dir);
    cmd.append("&& echo \"export PATH=");
    cmd.append(install_dir);
    cmd.append("/bin/:$PATH\" >> ");
    cmd.append(install_dir);
    cmd.append("/.bashrc && bash --rcfile ");
    cmd.append(install_dir);
    cmd.append("/.bashrc");

    std::cout << cmd;


    system(cmd.c_str());

    return 0;
}

int create_shell(std::string shell_name,std::string version){
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = "/usr/share/quantum/shells/";
        install_dir.append(shell_name);
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum/shells/");
        install_dir.append(shell_name);
    }

    std::string cmd = "mkdir -p  ";
    cmd.append(install_dir);
    cmd.append("/{bindir,bin,builddir} && cp quantum.db ");
    cmd.append(install_dir);
    system(cmd.c_str());

    luaL_openlibs(L);

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

    int r = luaL_dofile(L, "shell.lua");

    lua_getglobal(L, "dependencies");

    if (lua_istable(L, -1)){
        set_dependencies(L);
        for(const auto &dep : dependenciesx) {
            create(dep, shell_name, version);
            std::cout << "huh?";
        }
    }

    return 0;
}

int create(std::string pkg, std::string shell_name,std::string version){
    std::cout << "hi";
    auto me = getuid();
    auto myprivs = geteuid();
    std::string install_dir;

    if (myprivs == 0){
        install_dir = "/usr/share/quantum/shells/";
        install_dir.append(shell_name);
    } else {
        install_dir = std::getenv("HOME");
        install_dir.append("/quantum/shells/");
        install_dir.append(shell_name);
    }

    luaL_openlibs(L);

    lua_register(L, "quantum_install", lua_quantum_install_shell);
    lua_register(L, "make", lua_make_shell);
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
    std::cout << std::endl << cmd << std::endl;
    system(cmd.c_str());

    int r = luaL_dofile(L, "quantum.lua");
    lua_pushstring(L, install_dir.c_str());
    lua_setglobal(L, "install_dir");
    lua_register(L, "quantum_install", lua_quantum_install_shell);
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
