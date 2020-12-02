#include "Package.hpp"
#include "libs/CheckLua.hpp"
#include "libs/Checksum.hpp"
#include <iostream>
#include <unistd.h>

void tokenize(std::string const &str, const char delim,
			std::vector<std::string> &out)
{
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}

bool Package::build(){
    std::string cmd;
    cmd = "echo $(pwd) > ~/log";
    system(cmd.c_str());
    lua_getglobal(L, "build");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }
    return true;
}

bool Package::download(){
    chdir("builddir/");
    std::string cmd;
    
    if (git==true){          
      cmd = "git clone ";
      cmd.append(source); 
    } else{
      std::string s = source;
	    const char delim = '/';

	    std::vector<std::string> out;
	    tokenize(s, delim, out);

	    std::string download = out[out.size() - 1];

      std::string cmd = "curl -LO ";
      cmd.append(source); 
      system(cmd.c_str());
      if(checksumFile(download) == checksum){
        cmd = "tar xvf ";
        cmd.append(download); 
        system(cmd.c_str());

        cmd = "rm ";
        cmd.append(download);

        system(cmd.c_str());

        cmd = "mv ";
        cmd.append(name);
        cmd.append("* ");
        cmd.append(name);

        system(cmd.c_str());
      } else if(checksum == "none"){
        cmd = "tar xvf ";
        cmd.append(download); 
        system(cmd.c_str());

        cmd = "rm ";
        cmd.append(download);

        system(cmd.c_str());

        cmd = "mv ";
        cmd.append(name);
        cmd.append("* ");
        cmd.append(name);       

        system(cmd.c_str());
      } else{
        std::cout << std::endl << "CHECKSUM ERROR, EXITING" << std::endl;
        std::_Exit(1);
      }
    }
    bool result=!!system(cmd.c_str());
    
    return result;
}

bool Package::install(){
    lua_getglobal(L, "install");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    }

    return true;
}

bool Package::uninstall(){
    lua_getglobal(L, "uninstall");
    if (lua_isfunction(L, -1)){
      lua_pushstring(L, name.c_str());

      if (CheckLua(L, lua_pcall(L, 1, 1, 0))){
        std::cout << std::endl;
      }
    } else {
      std::string cmd="rm -rf ";
      cmd.append(installDir);
      cmd.append("/bindir/");
      cmd.append(name);
      cmd.append(" && find ");
      cmd.append(installDir);
      cmd.append("/bin/ -xtype l -delete");
      std::cout << std::endl << cmd << std::endl;
	  	const char *command = cmd.c_str();
	  	system(command);
    }

    return true;
}

void Package::clear(){
    name = "";
    version = "";
    summary = "";
    description = "";
    source = "";
    checksum = "";
    git = false;
}