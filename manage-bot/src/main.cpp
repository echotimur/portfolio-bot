#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>

#include "cli_manage.hpp"
#include "dir_changer.hpp"
#include "telegram_manage.hpp"
#include "nlohmann/json.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]){

    // Описание опций
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("tg,t", "starting telegram bot")
        ("cli,c", "execute CLI bot")
    ;


    // Парсинг командной строки
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    // обработка опций
    try{

        if(vm.count("help")){
            std::cout << desc << std::endl;
        }

        if(vm.count("cli")){

            DIRCH changeToCLIDir("../../cli-bot/build");
            changeToCLIDir.changeDir();
        
            CLIManage cli("./portfolio");

            std::string portfolioOutput = cli.getCommandOutput();

            std::cout << "Portfolio output:\n" << portfolioOutput << std::endl;

            changeToCLIDir.backDir();

            return 0;


        }
        
        if(vm.count("tg")){

            std::string telegram_apikey;

            std::ifstream ifs("../config/config.json");

            nlohmann::json config = nlohmann::json::parse(ifs);

            ifs.close();

            if (config.contains("telegram_apikey") && config["telegram_apikey"].is_string()) {
                telegram_apikey = config["telegram_apikey"].get<std::string>();
            } else {
                throw std::runtime_error("Error: 'twelvedata_apikey' not found or is not a string in config.json.");
            }

            DIRCH change("../../cli-bot/build");
            change.changeDir();
        
            TGManage tg(telegram_apikey);

            tg.startTGBot();

            change.backDir();

            return 0;

        }

    } catch (std::exception& e){
        std::cout << "Exception: " << e.what() << std::endl;
    }




    return 0;

}