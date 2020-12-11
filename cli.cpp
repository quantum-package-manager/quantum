#include <iostream>
#include <string>
#include <unistd.h>
#include "install.hpp"
#include "remove.hpp"
#include "libs/installed.hpp"
#include "libs/getDescription.hpp"
#include "libs/db/get_version.hpp"

int main(int argc, char *argv[]){
    
    
    if ( argc != 1 ){
	std::string arg=argv[1];
	std::string pkg="";
	if(argc == 3){
		pkg=argv[2];
	}
        if(arg=="install" || arg=="remove"){
            std::string answer;
            std::cout << "Name" << std::endl << pkg << std::endl << std::endl << "Do you want to continue? [Y/n] ";
            std::cin >> answer;

            if(answer == "y"){
                if(arg == "install"){
		            if(argc==3){
                        install_pkg(pkg);
		            } else {
			            install_pkg(pkg, argv[3]);
		            }
                } else if (arg == "remove"){
                    if(argc==3){
                        remove(pkg);
                    } else {
                        remove(pkg, argv[3]);
                    }
                }
            }   
        } else if(arg=="installed"){
            if (installed(pkg)) {
                std::cout << "yes" << std::endl;
            } else {
                std::cout << "no" << std::endl;
            }
        } else if(arg=="description"){
            desc(pkg);
        } else if(arg=="populate"){
            auto myprivs = geteuid();
            std::string install_dir;

            if (myprivs == 0){
                install_dir = "/usr/share/quantum/";
            } else {
                install_dir = std::getenv("HOME");
                install_dir.append("/quantum");
            }

		    std::string cmd="cd ";
            cmd.append(install_dir);
            cmd.append(" && curl -LO https://raw.githubusercontent.com/quantum-package-manager/repo/main/quantum.db");
		    system(cmd.c_str());
    	} else if(arg=="update"){
            auto me = getuid();
            auto myprivs = geteuid();
            std::string install_dir;

            if (myprivs == 0){
                install_dir = "/usr/share/quantum/";
            } else {
                install_dir = std::getenv("HOME");
                install_dir.append("/quantum");
            }

            std::fstream version_file;
            std::string version;
            std::string filePath=install_dir;
            filePath.append("/bindir/");
            filePath.append(pkg);
            filePath.append("/version");
            version_file.open(filePath,std::ios::in);
            if(version_file.is_open()){
                std::string line;
                while(getline(version_file, line)){
                    version=line;
                }
            }
            if(version == get_version(pkg)){
                std::cout << pkg << " Is Up To Date!" << std::endl;
            } else{
                std::string answer;

                std::cout << pkg << " Needs to be updated, proceed? [Y/n] ";
                std::cin >> answer;
                if(answer=="y"){
                    remove(pkg);
                    install_pkg(pkg);
                }
            }
        }
    } else {
        std::cout << "Quantum Package Manager v2 - 0.1.5a" << std::endl;
    }
}
