#include <iostream>
#include <fstream> 
#include <string>
#include "nlohmann/json.hpp"
#include "api_client.hpp"
#include "portfolio_calculate.hpp"
// #include "telegram_bot.hpp" 

namespace fs = std::filesystem;


int main() {
    std::string twelvedata_apikey;

    try {
        // 1. Загрузить конфигурацию из config/config.json
        std::cout << "\nLoading configuration from config/config.json..." << std::endl;
        
        std::ifstream ifs("../config/config.json");

        nlohmann::json config = nlohmann::json::parse(ifs);

        ifs.close();

        // Получаем API ключ Twelvedata
        if (config.contains("twelvedata_apikey") && config["twelvedata_apikey"].is_string()) {
            twelvedata_apikey = config["twelvedata_apikey"].get<std::string>();
        } else {
            throw std::runtime_error("Error: 'twelvedata_apikey' not found or is not a string in config.json.");
        }

        // // Получаем токен Telegram бота (пока не используется, но для полноты)
        // if (config.contains("telegram_bot_token") && config["telegram_bot_token"].is_string()) {
        //     telegram_bot_token = config["telegram_bot_token"].get<std::string>();
        // } else {
        //     // Не является критической ошибкой, если бота нет или токен не нужен прямо сейчас
        //     std::cerr << "Warning: 'telegram_bot_token' not found or is not a string in config.json. Bot functionality may be limited." << std::endl;
        // }

        // Проверяем, что API ключ не пуст
        if (twelvedata_apikey.empty()) {
            throw std::runtime_error("Error: Twelvedata API key is empty. Please check config.json.");
        }

        // 2. Инициализировать модули
        // Создаем экземпляр ApiClient с загруженным API ключом
        APIClient apiClient(twelvedata_apikey);

        // Создаем экземпляр PortfolioCalculator, передавая ему объект ApiClient
        PortfolioCalculator calculator(apiClient);

        // TelegramBot bot(telegram_bot_token, calculator); // Раскомментировать, когда TelegramBot будет реализован

        // 3. Загрузить данные портфеля
        std::cout << "Loading portfolio data from data/portfolio.json..." << std::endl;
        if (!calculator.loadPortfolio("../data/portfolio.json")) {
            throw std::runtime_error("Failed to load portfolio data. Exiting.");
        }

        // 4. Рассчитать общую стоимость портфеля
        std::cout << "\nCalculating total portfolio value (in RUB):\n" << std::endl;
        double total_value = calculator.calculateTotalValue("RUB");

        if (total_value >= 0) {
            std::cout << "\nRESULT: ₽" << std::fixed << std::setprecision(2) << total_value << std::endl << std::endl;
        } else {
            std::cerr << "\nFailed to calculate total portfolio value. See errors above." << std::endl << std::endl;
        }

        // 5. Запустить бота (эта часть пока закомментирована, так как класс TelegramBot не реализован)
        // std::cout << "Bot started..." << std::endl;
        // bot.start();

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1; // Возвращаем ненулевой код для обозначения ошибки
    }

    return 0; // Успешное завершение программы
}