#include "dir_changer.hpp"

#include <cstdlib>
#include <unistd.h>

DIRCH::DIRCH(const std::string& path) : path_(path) {

    this->originalCWD[0] = '\0';

}


void DIRCH::changeDir() {
    

    if (getcwd(originalCWD, sizeof(originalCWD)) == NULL) {
        perror("getcwd");
        throw  std::runtime_error("Exception in CHANGE dir!");
    }

    // Перейти в директорию, где находится portfolio (или её родительскую, если нужно
    if (chdir(path_.c_str()) == -1) { 
        perror("chdir");
        throw  std::runtime_error("Exception in CHANGE dir!");
    }

}


void DIRCH::backDir() {

    // Вернуть текущую рабочую директорию обратно
    if (chdir(originalCWD) == -1) {
        perror("chdir");
        throw  std::runtime_error("Exception in BACK dir!");
    }

    this->originalCWD[0] = '\0';

}