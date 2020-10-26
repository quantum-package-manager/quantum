#include <string>
#include <unistd.h>

static inline void runfile(std::string file){
  std::string cmd = "bash "+file;
  const char *command = cmd.c_str();
  system(command);
}

void downloadfile(std::string url){
  std::string cmd="curl -LO "+url+" >> /dev/null";
  const char *command = cmd.c_str();
  system(command);
}

void install_pkg(std::string pkg){
      std::string cmd = "mkdir bindir/"+pkg;
      const char *command = cmd.c_str();
      system(command);

      chdir("builddir/");

      // downloadfile("https://raw.githubusercontent.com/RhinoCodes/repo/main/repo/"+pkg+"/deps"); -- Will Rework Dependencies later
      
      /*
      std::ifstream file("deps");
      std::string str;
      while (std::getline(file, str)) {
        if(str.length() != 0){
          install_pkg(str);
        }
      }
      */
}
