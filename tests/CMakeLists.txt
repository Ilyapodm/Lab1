# Подключаем заголовочные файлы из src
include_directories(../src)

# Создаём исполняемый файл для тестов
add_executable(test_dynamic_array test_dynamic_array.c)

# Линкуем библиотеку
target_link_libraries(test_dynamic_array dynamic_array)

# Регистрируем тест в CTest
add_test(NAME test_dynamic_array COMMAND test_dynamic_array)