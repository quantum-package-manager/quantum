#include <iostream>
#include <string>
#include "build.hpp"

int main(int argc, char *argv[]){
	if ( argc != 1 ){
		std::string arg=argv[1];
        std::string pkg=argv[2];
		if ( arg == "install") {
            std::string cmd = "curl -LO https://raw.githubusercontent.com/RhinoCodes/repo/main/repo/"+pkg+"/quantum.lua";
            const char *command = cmd.c_str();
            system(command);
	    build();
            cmd = "rm quantum.lua";
            command = cmd.c_str();
            system(command);
		} else if ( arg == "remove" ) {
			std::string cmd="rm -rf bindir/"+pkg+" && find bin/ -xtype l -delete";
	  		const char *command = cmd.c_str();
	  		system(command);
		}
	} else {
		std::cout << "Quantum Package Manager 0.0.2a\n";
	}
}
