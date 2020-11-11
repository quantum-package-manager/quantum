#include "installed.hpp"

bool doesPathExist(const std::string &s){
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

bool installed(const std::string &pkg){
    std::string globalPath = "/usr/share/quantum/bindir/";
    globalPath.append(pkg);

    std::string localPath = std::getenv("HOME");
    localPath.append("/quantum-lua/bindir/");
    localPath.append(pkg);

    if(doesPathExist(localPath)){
        return true;
    } else if(doesPathExist(globalPath)){
        return true;
    } else {
        return false;
    }
}

