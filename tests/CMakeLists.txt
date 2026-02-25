# Files from /src 
include_directories(../src)

# Creating a test
add_executable(test_dynamic_array
    test_dynamic_array.c
    ../src/dynamic_array.c
    ../src/real_type.c
    ../src/string_type.c
)

# Register for ctest
add_test(NAME test_dynamic_array COMMAND test_dynamic_array)