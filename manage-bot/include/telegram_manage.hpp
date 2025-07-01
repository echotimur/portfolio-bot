#pragma once

#include <string>
#include <tgbot/tgbot.h>
#include "cli_manage.hpp"



class TGManage {

    public:

        TGManage(const std::string& apikey);

        void startTGBot();
        

    private:
        
        TgBot::Bot bot;


};