#include "portfolio_calculate.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>


PortfolioCalculator::PortfolioCalculator(APIClient& api_client) : api_client_(api_client) {}


bool PortfolioCalculator::loadPortfolio(const std::string& filepath) {
    
    std::ifstream ifs(filepath);

    try {

        nlohmann::json data = nlohmann::json::parse(ifs);

        portfolio_data_.clear();

        for (auto& [key, value] : data.items())
            portfolio_data_[key] = std::stod(value.get<std::string>());

    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Error: JSON parsing failed for portfolio file " << filepath << ": " << e.what() << std::endl;
        return false;
    }
    return true;
}

double PortfolioCalculator::calculateTotalValue(const std::string& base_currency) {
    double total_value = 0;
    double ruble_price = api_client_.getPrice("USD/RUB");
    double asset_price = 0;
    double quantity = 0;

    if (ruble_price < 0) {
        std::cerr << "Error: Could not get exchange rate for USD/RUB. Cannot convert foreign currency assets." << std::endl;
        return -1.0;
    }

    for (const auto& [key, value] : portfolio_data_) {

        if (value == 0) {
            std::cout << key << ": ₽" << value << std::endl;
            continue;
        }

        if (key == "RUB") {
            total_value += value;
            std::cout << key << ": ₽" << value << std::endl;
            continue;
        }

        if (key.find("RUB") != std::string::npos){
            asset_price = api_client_.getPrice(key);
            total_value = asset_price * value;
            std::cout << key << ": ₽" << asset_price * value << std::endl;
            asset_price = 0;
        }

        asset_price = api_client_.getPrice(key);

        total_value += asset_price * value * ruble_price;
        std::cout << key << ": ₽" << asset_price * value * ruble_price << std::endl;
        asset_price = 0;
    }

    return total_value;
}
