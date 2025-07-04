check_command() {
    local cmd="$1"
    local install_msg="$2"
    if ! command -v "$cmd" &> /dev/null; then
        echo "Ошибка: Команда '$cmd' не найдена."
        echo "Пожалуйста, установите '$cmd'. Например: $install_msg"
        exit 1
    fi
}

check_library_header() {
    local lib_name="$1"
    local header_file="$2"
    local install_msg="$3"
    local found=false

    local common_include_paths=(
        "/usr/local/include"
        "/usr/include"
        "/opt/homebrew/include"
    )

    for path in "${common_include_paths[@]}"; do
        if [ -f "$path/$header_file" ]; then
            echo "Зависимость '$lib_name' найдена в $path."
            found=true
            break
        fi
    done

    if [ "$found" = false ]; then
        echo "Ошибка: Заголовочный файл '$header_file' для библиотеки '$lib_name' не найден."
        echo "Пожалуйста, убедитесь, что '$lib_name' установлен(а) и доступен(а) в стандартных путях."
        echo "Например: $install_msg"
        exit 1
    fi
}

echo "--- Проверка системных зависимостей ---"

check_command "curl" "sudo apt install curl (Debian/Ubuntu), sudo dnf install curl (Fedora), brew install curl (macOS)"

check_command "cmake" "sudo apt install cmake (Debian/Ubuntu), sudo dnf install cmake (Fedora), brew install cmake (macOS)"

check_command "g++" "sudo apt install build-essential (Debian/Ubuntu), sudo dnf groupinstall \"Development Tools\" (Fedora), brew install gcc (macOS)"

check_library_header "Boost" "boost/version.hpp" "sudo apt install libboost-all-dev (Debian/Ubuntu), sudo dnf install boost-devel (Fedora), brew install boost (macOS)"

check_library_header "TgBot" "tgbot/tgbot.h" "Возможно, вам потребуется собрать TgBot из исходников. Инструкции см. на официальном GitHub репозитории TgBot API."

echo "Все необходимые зависимости найдены."
echo ""

echo "--- Начинаем сборку микросервисов ---"

MICROSERVICE_DIRS=(
    "cli-bot"
    "manage-bot"
)

if [[ "$(basename "$(pwd)")" != "portfolio" ]]; then
    echo "Ошибка: Этот скрипт должен быть запущен из корневой директории 'portfolio'."
    echo "Текущая директория: $(pwd)"
    exit 1
fi

for dir in "${MICROSERVICE_DIRS[@]}"; do
    echo "--- Сборка проекта: $dir ---"
    
    if [ ! -d "$dir" ]; then
        echo "Ошибка: Директория '$dir' не найдена. Проверьте путь или структуру проекта."
        exit 1
    fi

    cd "$dir" || { echo "Не удалось перейти в директорию $dir. Проверьте права доступа."; exit 1; }

    if [ ! -d "build" ]; then
        echo "Создание директории 'build' в $dir..."
        mkdir build || { echo "Не удалось создать директорию 'build' в $dir. Проверьте права доступа."; exit 1; }
    fi

    cd build || { echo "Не удалось перейти в директорию 'build' в $dir. Проверьте права доступа."; exit 1; }

    echo "Запуск cmake в $(pwd)..."

    cmake .. || { echo "Ошибка: cmake завершился с ошибкой в $dir. Проверьте CMakeLists.txt."; exit 1; }

    echo "Запуск make в $(pwd)..."

    make || { echo "Ошибка: make завершился с ошибкой в $dir. Проверьте ошибки компиляции."; exit 1; }

    echo "Сборка $dir завершена успешно."
    echo ""
    
    cd ../.. 
done

echo "Все микросервисы успешно собраны!"