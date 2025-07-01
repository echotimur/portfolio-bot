#include "api_client.hpp"

#include <iostream>
#include <stdexcept>
#include <curl/curl.h>
#include "nlohmann/json.hpp"


APIClient::APIClient(const std::string& apiKey) : api_key_(apiKey) {
    curl_handle_ = curl_easy_init();

    if(!curl_handle_){
        throw std::runtime_error("----Failed to initializate cURL handle");
    }
}


APIClient::~APIClient() {
    if(curl_handle_){
        curl_easy_cleanup(curl_handle_);
    }
}


size_t APIClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}


double APIClient::getPrice(const std::string& symbol) {
    // 1. Готовим URL и строку для ответа
    std::string url = "https://api.twelvedata.com/price?symbol=" + symbol + "&apikey=" + api_key_;
    std::string response_string;

    // 2. Настраиваем cURL для этого конкретного запроса
    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, APIClient::WriteCallback);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response_string);

    // 3. Выполняем запрос
    CURLcode res = curl_easy_perform(curl_handle_);

    // 4. Проверяем на ошибки
    if (res != CURLE_OK) {
        std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
        return -1.0; // Возвращаем значение-индикатор ошибки
    }

    // 5. Парсим JSON и возвращаем цену (вся твоя логика отсюда)
    try {
        auto data = nlohmann::json::parse(response_string);
        if (data.contains("price")) {
            double price = std::stod(data["price"].get<std::string>());
            if (symbol.find("XAU") != std::string::npos) {
                return price / 31.1035;
            }
            return price;
        } else {
            std::cerr << "Invalid JSON structure: " << data.dump() << std::endl;
            return -1.0;
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON Parse Error: " << e.what() << std::endl;
        return -1.0;
    }
}