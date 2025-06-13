#pragma once

#include <string>
#include <map>
#include "api_client.hpp"
#include "nlohmann/json.hpp"


class PortfolioCalculator {
public:
    // Конструктор, который принимает ссылку на объект ApiClient.
    // Это позволяет PortfolioCalculator запрашивать цены через ApiClient.
    explicit PortfolioCalculator(APIClient& api_client);

    // Метод для загрузки данных портфеля из JSON файла.
    // Возвращает true в случае успеха, false в случае ошибки.
    bool loadPortfolio(const std::string& filepath);

    // Метод для расчета общей стоимости портфеля в указанной базовой валюте (например, "RUB").
    // Возвращает общую стоимость или -1.0 в случае ошибки.
    double calculateTotalValue(const std::string& base_currency = "RUB");

private:
    APIClient& api_client_; // Ссылка на объект ApiClient для выполнения запросов к API
    std::map<std::string, double> portfolio_data_; // Данные портфеля: символ -> количество
};