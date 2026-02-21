# Создаём статическую библиотеку из всех модулей
add_library(dynamic_array STATIC
    dynamic_array.c
    real_type.c
    string_type.c
    mappers.c
)

# Создаём исполняемый файл из главного файла
add_executable(Lab1 main.c)

# Линкуем библиотеку к исполняемому файлу
target_link_libraries(Lab1 dynamic_array)