# Путь к заголовочным файлам из src
include_directories(../src)

# Тест из тестового файла + всех .c файлов
add_executable(test_dynamic_array
    test_dynamic_array.c
    ../src/dynamic_array.c
    ../src/real_type.c
    ../src/string_type.c
)

# Регистрируем тест для ctest
add_test(NAME test_dynamic_array COMMAND test_dynamic_array)