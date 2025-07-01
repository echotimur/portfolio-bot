#include "portfolio_calculate.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>


PortfolioCalculator::PortfolioCalculator(APIClient& api_client) : api_client_(api_client) {}


bool PortfolioCalculator::loadPortfolio(const std::string& filepath) {
    
    std::ifstream ifs(filepath);

    try{

        nlohmann::json data = nlohmann::json::parse(ifs);

        portfolio_data_.clear();

        for(nlohmann::json::iterator it_date = data.begin(); it_date != data.end(); it_date++){

            std::cout << it_date.key() << ":\n"; 

            if(it_date.value().is_array() && !it_date.value().empty()){

                nlohmann::json& people = it_date.value()[0];

                for(nlohmann::json::iterator it_name = people.begin(); it_name != people.end(); it_name++){

                    if(it_name.value().is_array() && !it_name.value().empty()){

                        std::cout << "├--" << it_name.key() << ":\n";

                        nlohmann::json& entry = it_name.value()[0];

                        for(nlohmann::json::iterator it_currency = entry.begin(); it_currency != entry.end(); it_currency++){

                            if(it_currency.value().is_string() && !it_currency.value().empty()){

                                std::cout << "|  └--" << it_currency.key() << ": " << it_currency.value() << std::endl;

                                portfolio_data_[it_currency.key()] += std::stod(it_currency.value().get<std::string>());

                                // std::cout << key << " | " << value << std::endl;

                            } else{

                                std::cout << "Can't read the " << it_currency.key() << " values" << std::endl;

                            }

                        }

                    } else{

                        std::cout << "Can't read the " << it_name.key() << " currency value" << std::endl;

                    }
                    
                }

            } else{

                std::cout << "Can't read the " << it_date.key() << " date values" << std::endl;

            }

        }
    } catch(const std::exception& except){
        std::cerr << "JSON parse FAILED: " << except.what() << std::endl;
    }

    return true;
}

double PortfolioCalculator::calculateTotalValue(const std::string& base_currency) {
    double total_value = 0;
    double ruble_price = api_client_.getPrice("USD/RUB");
    double asset_price = 0;

    if (ruble_price < 0) {
        std::cerr << "Error: Could not get exchange rate for USD/RUB. Cannot convert foreign currency assets." << std::endl;
        return -1.0;
    }

    for (const auto& [key, value] : portfolio_data_) {

        if (value == 0) {
            continue;
        }

        if (key == "RUB") {
            total_value += value;
            continue;
        }

        if (key.find("RUB") != std::string::npos){
            asset_price = api_client_.getPrice(key);
            total_value = asset_price * value;
            asset_price = 0;
        }

        asset_price = api_client_.getPrice(key);

        total_value += asset_price * value * ruble_price;
        asset_price = 0;
    }

    return total_value;
}
