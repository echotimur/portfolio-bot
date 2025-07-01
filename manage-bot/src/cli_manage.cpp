#include "cli_manage.hpp"

#include <cstdlib>
#include <string>
#include <vector>

CLIManage::CLIManage(const std::string& cmd) : cmd_(cmd) {

    pipe_ = popen(cmd_.c_str(), "r");
    if (!pipe_) {
        throw std::runtime_error("----Failed to initializate PIPE");
    }

}


CLIManage::~CLIManage() {

    pclose(pipe_);

}


std::string  CLIManage::getCommandOutput() {

    std::string result = "";


    const int CHUNCK_SIZE = 4096;
    std::vector<char> buffer(CHUNCK_SIZE);

    size_t bytesRead;

    while ((bytesRead = fread(buffer.data(), 1, buffer.size(), pipe_)) > 0) {
        
        result.append(buffer.data(), bytesRead);
        
    }

    buffer.clear();

    return result;

}