#pragma once

#include <string>

typedef void CURL;

class APIClient {

    public:

        APIClient(const std::string& apiKey);

        ~APIClient();

        double getPrice(const std::string& symbol);


    private: 

        std::string api_key_;

        CURL* curl_handle_;

        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);

};