cmake_minimum_required(VERSION 3.15)
project(variant_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Catch2 и Boost
find_package(Catch2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS variant)

# Создание исполняемого файла для тестов
add_executable(test_variant test_main.cpp variant.cpp)
target_link_libraries(test_variant PRIVATE 
    Catch2::Catch2WithMain 
    Boost::variant)

# Интеграция с CTest
include(CTest)
enable_testing()
add_test(NAME variant_tests COMMAND test_variant)