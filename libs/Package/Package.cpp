#include "Package.hpp"
#include "../Checksum.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

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
      std::string cmd("git clone ");
      cmd.append(source); 
    } else{
      std::string s = source;
	    const char delim = '/';

	    std::vector<std::string> out;
	    tokenize(s, delim, out);

	    std::string download = out[out.size() - 1];

      std::cout << download;
      std::string cmd = "curl -LO ";
      cmd.append(source); 
      system(cmd.c_str());
      if(checksumFile(download) == checksum){
        cmd = "tar xvf ";
        cmd.append(download); 
        system(cmd.c_str());

        std::string dir_suffix = download;

        for(int i=0; i<7; i++){
          dir_suffix.pop_back();
        }

        cmd = "mv ";
        cmd.append(name); 
        cmd.append("-");
        cmd.append(dir_suffix);
        cmd.append(" ");
        cmd.append(name);
        system(cmd.c_str());
      } else if(checksum == "none"){
        cmd = "tar xvf ";
        cmd.append(download); 
        system(cmd.c_str());

        std::string dir_suffix = download;

        for(int i=0; i<7; i++){
          dir_suffix.pop_back();
        }

        cmd = "mv ";
        cmd.append(name); 
        cmd.append("-");
        cmd.append(dir_suffix);
        cmd.append(" ");
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