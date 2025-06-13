#include <iostream>
#include <fstream>      
#include <string>       
#include "nlohmann/json.hpp" 
#include "api_client.hpp" 
#include "portfolio_calculate.hpp" 


nlohmann::json loadConfig(const std::string& filepath) {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        throw std::runtime_error("Error: Could not open config file: " + filepath);
    }
    try {
        return nlohmann::json::parse(ifs);
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Error: JSON parsing failed for config file " + filepath + ": " + e.what());
    }
}

int main() {
    std::string twelvedata_apikey;

    try {

        std::cout << "Loading configuration from config/config.json..." << std::endl;
        nlohmann::json config = loadConfig("config/config.json");

        if (config.contains("twelvedata_apikey") && config["twelvedata_apikey"].is_string()) {
            twelvedata_apikey = config["twelvedata_apikey"].get<std::string>();
        } else {
            throw std::runtime_error("Error: 'twelvedata_apikey' not found or is not a string in config.json.");
        }

        if (config.contains("telegram_bot_token") && config["telegram_bot_token"].is_string()) {
            telegram_bot_token = config["telegram_bot_token"].get<std::string>();
        } else {
            std::cerr << "Warning: 'telegram_bot_token' not found or is not a string in config.json. Bot functionality may be limited." << std::endl;
        }

        if (twelvedata_apikey.empty()) {
            throw std::runtime_error("Error: Twelvedata API key is empty. Please check config.json.");
        }

        APIClient apiClient(twelvedata_apikey);

        PortfolioCalculator calculator(apiClient);

        std::cout << "Loading portfolio data from data/portfolio.json..." << std::endl;
        if (!calculator.loadPortfolio("data/portfolio.json")) {
            throw std::runtime_error("Failed to load portfolio data. Exiting.");
        }

        std::cout << "\nCalculating total portfolio value (in RUB):" << std::endl;
        double total_value = calculator.calculateTotalValue("RUB");

        if (total_value >= 0) {
            std::cout << "\nRESULT: ₽" << std::fixed << std::setprecision(2) << total_value << std::endl << std::endl;
        } else {
            std::cerr << "\nFailed to calculate total portfolio value. See errors above." << std::endl << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
