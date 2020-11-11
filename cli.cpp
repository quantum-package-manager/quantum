#include <iostream>
#include <string>
#include "install.hpp"
#include "libs/installed.hpp"

int main(int argc, char *argv[]){
    
    
    if ( argc != 1 ){
		std::string arg=argv[1];
        std::string pkg=argv[2];

        if(arg=="install" || arg=="remove"){
            std::string answer;
            std::cout << "Name" << std::endl << pkg << std::endl << std::endl << "Do you want to continue? [Y/n] ";
            std::cin >> answer;

            if(answer == "y"){
                if(arg == "install"){
                    install_pkg(pkg);
                } 
            /* else if (arg == "remove"){
                std::string cmd="rm -rf bindir/"+pkg+" && find bin/ -xtype l -delete";
	  		    const char *command = cmd.c_str();
	  		    system(command);
            } */ // Remove support for uninstalling for a bit
            }   
        } else if(arg=="installed"){
            if (installed(pkg)) {
                std::cout << "yes" << std::endl;
            } else {
                std::cout << "no" << std::endl;
            }
        }
    } else {
        std::cout << "Quantum Package Manager v2 - 0.0.4a" << std::endl;
    }
}