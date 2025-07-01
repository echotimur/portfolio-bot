#pragma once

#include <string>
#include <map>
#include "api_client.hpp"
#include "nlohmann/json.hpp"


class PortfolioCalculator {
    public:
    
        explicit PortfolioCalculator(APIClient& api_client);
    
        bool loadPortfolio(const std::string& filepath);
    
        double calculateTotalValue(const std::string& base_currency = "RUB");
    
    
    private:
    
        APIClient& api_client_;
    
        std::map<std::string, double> portfolio_data_;

};