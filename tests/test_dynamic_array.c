#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/dynamic_array.h"
#include "../src/real_type.h"
#include "../src/string_type.h"

/********************************************************************
 * Tests of Dynamic Array with Real Numbers
 *******************************************************************/

// Test 1: Creating and destroying
void test_create_destroy_real() {
    printf("Test 1: Create and destroy (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    assert(arr != NULL);
    assert(dyn_array_size(arr) == 0);
    assert(dyn_array_capacity(arr) == 0);
    assert(dyn_array_type(arr) == ofReal());
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 2: Adding a new element (real)
void test_push_back_real() {
    printf("Test 2: Push back (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double val1 = 3.14;
    int result = dyn_array_push_back(arr, &val1);
    assert(result == 0);
    assert(dyn_array_size(arr) == 1);
    
    double val2 = 2.71;
    result = dyn_array_push_back(arr, &val2);
    assert(result == 0);
    assert(dyn_array_size(arr) == 2);
    
    const double* retrieved = dyn_array_get(arr, 0);
    assert(retrieved != NULL);
    assert(*retrieved == 3.14);
    
    retrieved = dyn_array_get(arr, 1);
    assert(retrieved != NULL);
    assert(*retrieved == 2.71);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 3: Removing an element (real)
void test_pop_real() {
    printf("Test 3: Pop (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double val1 = 10.5;
    double val2 = 20.3;
    dyn_array_push_back(arr, &val1);
    dyn_array_push_back(arr, &val2);
    
    double popped;
    int result = dyn_array_pop(arr, &popped);
    assert(result == 1);
    assert(popped == 20.3);
    assert(dyn_array_size(arr) == 1);
    
    result = dyn_array_pop(arr, &popped);
    assert(result == 1);
    assert(popped == 10.5);
    assert(dyn_array_size(arr) == 0);
    
    result = dyn_array_pop(arr, &popped);
    assert(result == 0);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 4: Sort (real)
void test_sort_real() {
    printf("Test 4: Sort (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double values[] = {5.5, 2.2, 8.8, 1.1, 3.3};
    for (int i = 0; i < 5; i++) {
        dyn_array_push_back(arr, &values[i]);
    }
    
    dyn_array_sort_asc(arr);
    const double* elem;
    elem = dyn_array_get(arr, 0); assert(*elem == 1.1);
    elem = dyn_array_get(arr, 1); assert(*elem == 2.2);
    elem = dyn_array_get(arr, 2); assert(*elem == 3.3);
    elem = dyn_array_get(arr, 3); assert(*elem == 5.5);
    elem = dyn_array_get(arr, 4); assert(*elem == 8.8);
    
    dyn_array_sort_desc(arr);
    elem = dyn_array_get(arr, 0); assert(*elem == 8.8);
    elem = dyn_array_get(arr, 1); assert(*elem == 5.5);
    elem = dyn_array_get(arr, 2); assert(*elem == 3.3);
    elem = dyn_array_get(arr, 3); assert(*elem == 2.2);
    elem = dyn_array_get(arr, 4); assert(*elem == 1.1);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 5: Mapping (real)
void test_map_real() {
    printf("Test 5: Map (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double values[] = {1.0, 2.0, 3.0};
    for (int i = 0; i < 3; i++) {
        dyn_array_push_back(arr, &values[i]);
    }
    
    DynamicArray* mapped = dyn_array_map(arr, real_map_multiply);
    assert(mapped != NULL);
    assert(dyn_array_size(mapped) == 3);
    
    const double* elem;
    elem = dyn_array_get(mapped, 0); assert(*elem == 2.0);
    elem = dyn_array_get(mapped, 1); assert(*elem == 4.0);
    elem = dyn_array_get(mapped, 2); assert(*elem == 6.0);
    
    dyn_array_destroy(arr);
    dyn_array_destroy(mapped);
    printf(" OK\n");
}

// Test 6: Where (real)
void test_where_real() {
    printf("Test 6: Where (real)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double values[] = {-2.0, 3.0, -1.0, 5.0, 0.0, 7.0};
    for (int i = 0; i < 6; i++) {
        dyn_array_push_back(arr, &values[i]);
    }
    
    DynamicArray* filtered = dyn_array_where(arr, is_positive);
    assert(filtered != NULL);
    assert(dyn_array_size(filtered) == 3);
    
    const double* elem;
    elem = dyn_array_get(filtered, 0); assert(*elem == 3.0);
    elem = dyn_array_get(filtered, 1); assert(*elem == 5.0);
    elem = dyn_array_get(filtered, 2); assert(*elem == 7.0);
    
    dyn_array_destroy(arr);
    dyn_array_destroy(filtered);
    printf(" OK\n");
}

// Test 7: Concatination (real)
void test_concat_real() {
    printf("Test 7: Concat (real)...");
    
    DynamicArray* arr1 = dyn_array_create(ofReal());
    DynamicArray* arr2 = dyn_array_create(ofReal());
    
    double vals1[] = {1.0, 2.0};
    double vals2[] = {3.0, 4.0, 5.0};
    
    for (int i = 0; i < 2; i++) dyn_array_push_back(arr1, &vals1[i]);
    for (int i = 0; i < 3; i++) dyn_array_push_back(arr2, &vals2[i]);
    
    DynamicArray* concatenated = dyn_array_concat(arr1, arr2);
    assert(concatenated != NULL);
    assert(dyn_array_size(concatenated) == 5);
    
    const double* elem;
    elem = dyn_array_get(concatenated, 0); assert(*elem == 1.0);
    elem = dyn_array_get(concatenated, 1); assert(*elem == 2.0);
    elem = dyn_array_get(concatenated, 2); assert(*elem == 3.0);
    elem = dyn_array_get(concatenated, 3); assert(*elem == 4.0);
    elem = dyn_array_get(concatenated, 4); assert(*elem == 5.0);
    
    dyn_array_destroy(arr1);
    dyn_array_destroy(arr2);
    dyn_array_destroy(concatenated);
    printf(" OK\n");
}

// Test 8: Shrink to fit (real)
void test_shrink_to_fit_real() {
    printf("Test 8: Shrink to fit (double)...");
    
    DynamicArray* arr = dyn_array_create(ofReal());
    
    double val = 1.0;
    for (int i = 0; i < 3; i++) {
        dyn_array_push_back(arr, &val);
    }
    
    int result = dyn_array_shrink_to_fit(arr);
    assert(result == 1);
    assert(dyn_array_capacity(arr) == dyn_array_size(arr));
    assert(dyn_array_capacity(arr) == 3);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

/********************************************************************
 * Tests of Dynamic Array with Strings
 *******************************************************************/

 // Test 9: Creating and destroying an array (String)
void test_create_destroy_string() {
    printf("Test 9: Create and destroy (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    assert(arr != NULL);
    assert(dyn_array_size(arr) == 0);
    assert(dyn_array_capacity(arr) == 0);
    assert(dyn_array_type(arr) == ofString());
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 10: Adding a new element (String)
void test_push_back_string() {
    printf("Test 10: Push back (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());

    int result = dyn_array_push_back(arr, "hello");
    assert(result == 0);
    assert(dyn_array_size(arr) == 1);
    
    result = dyn_array_push_back(arr, "world");
    assert(result == 0);
    assert(dyn_array_size(arr) == 2);
    
    const char* const* retrieved = dyn_array_get(arr, 0);
    assert(retrieved != NULL);
    assert(strcmp(*retrieved, "hello") == 0);
    
    retrieved = dyn_array_get(arr, 1);
    assert(retrieved != NULL);
    assert(strcmp(*retrieved, "world") == 0);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 11: Removing an element (String)
void test_pop_string() {
    printf("Test 11: Pop (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    
    dyn_array_push_back(arr, "first");
    dyn_array_push_back(arr, "second");
    
    char* popped = NULL;
    int result = dyn_array_pop(arr, &popped);
    assert(result == 1);
    assert(popped != NULL);
    assert(strcmp(popped, "second") == 0);
    assert(dyn_array_size(arr) == 1);
    free(popped);
    
    result = dyn_array_pop(arr, &popped);
    assert(result == 1);
    assert(popped != NULL);
    assert(strcmp(popped, "first") == 0);
    assert(dyn_array_size(arr) == 0);
    free(popped);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 12: Sort (String)
void test_sort_string() {
    printf("Test 12: Sort (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    
    char* strs[] = {"banana", "apple", "cherry"};
    for (int i = 0; i < 3; i++) {
        dyn_array_push_back(arr, strs[i]);
    }
    
    dyn_array_sort_asc(arr);
    const char* const* elem;
    elem = dyn_array_get(arr, 0); assert(strcmp(*elem, "apple") == 0);
    elem = dyn_array_get(arr, 1); assert(strcmp(*elem, "banana") == 0);
    elem = dyn_array_get(arr, 2); assert(strcmp(*elem, "cherry") == 0);
    
    dyn_array_sort_desc(arr);
    elem = dyn_array_get(arr, 0); assert(strcmp(*elem, "cherry") == 0);
    elem = dyn_array_get(arr, 1); assert(strcmp(*elem, "banana") == 0);
    elem = dyn_array_get(arr, 2); assert(strcmp(*elem, "apple") == 0);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Test 13: Mapping (Strings)
void test_map_string() {
    printf("Test 13: Map (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    
    char* strs[] = {"hello", "World", "TEST"};
    for (int i = 0; i < 3; i++) {
        dyn_array_push_back(arr, strs[i]);
    }
    
    // Test: upper
    DynamicArray* mapped_upper = dyn_array_map(arr, string_map_to_upper);
    assert(mapped_upper != NULL);
    assert(dyn_array_size(mapped_upper) == 3);
    
    const char* const* elem;
    elem = dyn_array_get(mapped_upper, 0); assert(strcmp(*elem, "HELLO") == 0);
    elem = dyn_array_get(mapped_upper, 1); assert(strcmp(*elem, "WORLD") == 0);
    elem = dyn_array_get(mapped_upper, 2); assert(strcmp(*elem, "TEST") == 0);
    
    // Test: Lower
    DynamicArray* mapped_lower = dyn_array_map(arr, string_map_to_lower);
    assert(mapped_lower != NULL);
    assert(dyn_array_size(mapped_lower) == 3);
    
    elem = dyn_array_get(mapped_lower, 0); assert(strcmp(*elem, "hello") == 0);
    elem = dyn_array_get(mapped_lower, 1); assert(strcmp(*elem, "world") == 0);
    elem = dyn_array_get(mapped_lower, 2); assert(strcmp(*elem, "test") == 0);
    
    dyn_array_destroy(arr);
    dyn_array_destroy(mapped_upper);
    dyn_array_destroy(mapped_lower);
    printf(" OK\n");
}

// Test 14: Where (Strings)
void test_where_string() {
    printf("Test 14: Where (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    
    char* strs[] = {"apple", "", "banana", "avocado"};
    for (int i = 0; i < 4; i++) {
        dyn_array_push_back(arr, strs[i]);
    }
    
    DynamicArray* filtered = dyn_array_where(arr, string_starts_with_a);
    assert(filtered != NULL);
    assert(dyn_array_size(filtered) == 2);
    
    const char* const* elem;
    elem = dyn_array_get(filtered, 0); assert(strcmp(*elem, "apple") == 0);
    elem = dyn_array_get(filtered, 1); assert(strcmp(*elem, "avocado") == 0);
    
    dyn_array_destroy(arr);
    dyn_array_destroy(filtered);
    printf(" OK\n");
}

// Test 15: Concatination (string)
void test_concat_string() {
    printf("Test 15: Concat (string)...");
    
    DynamicArray* arr1 = dyn_array_create(ofString());
    DynamicArray* arr2 = dyn_array_create(ofString());
    
    char* vals1[] = {"Hello", "Banana"};
    char* vals2[] = {"Apple", "Dog", "Python"};

    for (int i = 0; i < 2; i++)
        dyn_array_push_back(arr1, vals1[i]);
    for (int i = 0; i < 3; i++)
        dyn_array_push_back(arr2, vals2[i]);

    DynamicArray* concatenated = dyn_array_concat(arr1, arr2);
    assert(concatenated != NULL);
    assert(dyn_array_size(concatenated) == 5);

    const char* const* elem;
    elem = dyn_array_get(concatenated, 0); assert(strcmp(*elem, "Hello")  == 0);
    elem = dyn_array_get(concatenated, 1); assert(strcmp(*elem, "Banana") == 0);
    elem = dyn_array_get(concatenated, 2); assert(strcmp(*elem, "Apple")  == 0);
    elem = dyn_array_get(concatenated, 3); assert(strcmp(*elem, "Dog")    == 0);
    elem = dyn_array_get(concatenated, 4); assert(strcmp(*elem, "Python") == 0);

    dyn_array_destroy(arr1);
    dyn_array_destroy(arr2);
    dyn_array_destroy(concatenated);
    printf(" OK\n");
}

// Test 16: Shrink to fit (real)
void test_shrink_to_fit_string() {
    printf("Test 16: Shrink to fit (string)...");
    
    DynamicArray* arr = dyn_array_create(ofString());
    
    char val[] = "Hello";
    for (int i = 0; i < 3; i++) {
        dyn_array_push_back(arr, val);
    }
    
    int result = dyn_array_shrink_to_fit(arr);
    assert(result == 1);
    assert(dyn_array_capacity(arr) == dyn_array_size(arr));
    assert(dyn_array_capacity(arr) == 3);
    
    dyn_array_destroy(arr);
    printf(" OK\n");
}

// Main function
int main() {
    printf("========================================\n");
    printf("  ЗАПУСК ТЕСТОВ DYNAMIC ARRAY\n");
    printf("========================================\n\n");
    
    // Launch all the tests
    test_create_destroy_real();
    test_push_back_real();
    test_pop_real();
    test_sort_real();
    test_map_real();
    test_where_real();
    test_concat_real();
    test_shrink_to_fit_real();
    test_create_destroy_string();
    test_push_back_string();
    test_pop_string();
    test_sort_string();
    test_map_string();
    test_where_string();
    test_concat_string();
    test_shrink_to_fit_string();
    
    printf("\n========================================\n");
    printf("  ВСЕ ТЕСТЫ ПРОЙДЕНЫ!\n");
    printf("========================================\n");
    
    return 0;
}