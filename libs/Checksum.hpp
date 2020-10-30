#include <iostream>
#include <string>

std::string checksumFile(std::string file) {
    std::string cmd="sha512sum ";
    cmd.append(file);

    
    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
      while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
          pclose(stream);
    }

    
    for(int i=0; i<file.length()+3; i++){
      data.pop_back();
    }

    return data;
}