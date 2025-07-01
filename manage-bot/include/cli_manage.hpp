#pragma once

#include <string>
#include <cstdlib>

class CLIManage {

    public:

        CLIManage(const std::string& cmd);
    
        ~CLIManage();
    
        std::string getCommandOutput();


    private:

        std::string cmd_;

        FILE* pipe_;

};