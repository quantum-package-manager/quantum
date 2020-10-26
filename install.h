#include <iostream>
#include <fstream>
#include <string>
#include "util.h"

static inline int install(std::string pkg){
  std::string answer;

  std::cout << "Name" << std::endl;
  std::cout << pkg << std::endl << std::endl;

  std::cout << "Do you want to continue? [Y/n] ";
  std::cin >> answer;
  if(answer == "y"){
    downloadfile("https://raw.githubusercontent.com/RhinoCodes/repo/main/repo/"+pkg+"/quantumfile");
    downloadfile("https://raw.githubusercontent.com/RhinoCodes/repo/main/repo/"+pkg+"/deps");

    std::ifstream file("deps");
    std::string str;
    while (std::getline(file, str)) {
      if(str.length() != 0){
        install_pkg(str);
      }
    }

    install_pkg(pkg);


  }
}
