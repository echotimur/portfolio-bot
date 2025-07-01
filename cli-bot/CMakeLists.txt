# Указываем минимальную требуемую версию CMake
cmake_minimum_required(VERSION 3.10)

# Название вашего проекта
project(portfolio CXX)

# Указываем стандарт C++17 для компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Добавляем директории для поиска заголовочных файлов
# include/ - ваши заголовочные файлы
# vendor/ - заголовочные файлы внешних библиотек (например, nlohmann/json)
include_directories(include vendor)

# Добавляем директорию для исходных файлов
# src/ - ваши файлы .cpp
link_directories(src)

# Добавляем поддиректорию vendor/nlohmann/json для nlohmann/json
# Это предполагает, что nlohmann/json.hpp лежит в vendor/nlohmann/json/json.hpp
# Если у вас vendor/nlohmann/json.hpp, то просто 'vendor' уже достаточно
# Тем не менее, для корректной работы с include_directories, лучше, чтобы nlohmann/json.hpp
# был доступен по пути vendor/nlohmann/json.hpp или vendor/json.hpp,
# или явно добавить vendor/nlohmann к include_directories, если json.hpp внутри nlohmann.
# Предположим, что nlohmann::json.hpp находится в vendor/nlohmann/json.hpp
# В этом случае, include_directories(include vendor) должно сработать, если вы используете #include "nlohmann/json.hpp"
# Если nlohmann/json.hpp находится в vendor/nlohmann/json.hpp, тогда include_directories(include vendor/nlohmann)

# Для простоты и соответствия вашей структуре, убедимся, что nlohmann::json.hpp
# доступен через include_directories(vendor). Это означает, что
# путь к нему должен быть vendor/json.hpp или если он в подпапке vendor/nlohmann/json.hpp,
# то включать его надо как "nlohmann/json.hpp" и в include_directories должен быть "vendor".
# Если nlohmann::json.hpp лежит в vendor/nlohmann/json.hpp, то достаточно include_directories(vendor).
# Если структура vendor/nlohmann/json.hpp, то нужно include_directories(vendor).
# А в коде #include "nlohmann/json.hpp".

# Определяем исходные файлы
# Предполагается, что эти файлы будут существовать после их создания
set(SOURCE_FILES
    src/main.cpp
    src/api_client.cpp
    src/portfolio_calculate.cpp
)

# Создаем исполняемый файл
add_executable(portfolio ${SOURCE_FILES})

# Если у вас есть внешние библиотеки, которые нужно слинковать,
# добавьте их здесь. Например, для cURL:
# find_package(CURL REQUIRED)
# target_link_libraries(PortfolioBot CURL::libcurl)
# Если у вас libcurl установлена в системе.
# Если cURL статически подключается, вам нужно будет указать путь к его библиотеке.
# В вашем parser.cpp используется cURL, поэтому его нужно будет слинковать.
# Для простоты, если cURL устанавливается через систему и ее библиотеки находятся в стандартных путях,
# find_package(CURL REQUIRED) и target_link_libraries(PortfolioBot CURL::libcurl) сработают.
# Иначе, вам может понадобиться указать путь к .lib или .a файлам cURL вручную
# или использовать пакетный менеджер, который интегрируется с CMake.

# Для локального использования cURL, если вы скомпилировали его или у вас есть его статическая библиотека:
# set(CURL_INCLUDE_DIR /path/to/curl/include)
# set(CURL_LIBRARY /path/to/curl/lib/libcurl.a) # или .lib на Windows
# include_directories(${CURL_INCLUDE_DIR})
# target_link_libraries(PortfolioBot ${CURL_LIBRARY})

# Предполагаем, что cURL установлен в вашей системе, и CMake может его найти:
find_package(CURL REQUIRED)
target_link_libraries(portfolio PRIVATE CURL::libcurl)

# Для nlohmann/json не требуется явное линкование, так как это header-only библиотека.

# Для более продвинутого CMake, вы можете использовать install() команды
# для установки исполняемого файла и библиотек.