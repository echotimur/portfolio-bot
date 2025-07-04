cmake_minimum_required(VERSION 3.10)

project(portfolio CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include vendor)

link_directories(src)

set(SOURCE_FILES
    src/main.cpp
    src/api_client.cpp
    src/portfolio_calculate.cpp
)

add_executable(portfolio ${SOURCE_FILES})

find_package(CURL REQUIRED)
target_link_libraries(portfolio PRIVATE CURL::libcurl)
