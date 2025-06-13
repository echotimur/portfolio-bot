cmake_minimum_required(VERSION 3.10)

project(PortfolioBot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include vendor)

link_directories(src)

set(SOURCE_FILES
    src/main.cpp
    src/api_client.cpp
    src/portfolio_calculat.cpp
)

add_executable(portfolioBot ${SOURCE_FILES})

find_package(CURL REQUIRED)
target_link_libraries(PortfolioBot PRIVATE CURL::libcurl)
