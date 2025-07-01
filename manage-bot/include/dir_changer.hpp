#pragma once

#include <string>

class DIRCH {

    public:

        DIRCH(const std::string& path);

        void changeDir();

        void backDir();


    private:

        std::string path_;

        char originalCWD[1024];

};