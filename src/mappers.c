#include "mappers.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/********************************************************************
 * Mappers and Predicates for Real Numbers
 *******************************************************************/

// Map (Real)
void real_map_multiply(void* dest, const void* src) {
    *(double*)dest = *(const double*)src * 2.0;
}

// Map (Real)
void real_map_square(void* dest, const void* src) {
    *(double*)dest = *(const double*)src * *(const double*)src;
}

// Map (Real)
void real_map_add_one(void* dest, const void* src) {
    *(double*)dest = *(const double*)src + 1.0;
}

// Predicate 
int is_positive(const void* elem) {
    return (*(const double*)elem) > 0;
}

// Predicate
int is_negative(const void* elem) {
    return (*(const double*)elem) < 0;
}

// Predicate
int is_even_int(const void* elem) {
    double val = *(const double*)elem;
    return ((int)val % 2 == 0);
}

/********************************************************************
 * Mappers and Predicates for Strings
 *******************************************************************/

// Map
void string_map_to_upper(void* dest, const void* src) {
    const char* const* src_ptr = (const char* const*)src;
    char** dest_ptr = (char**)dest;
    
    if (*src_ptr == NULL) {
        *dest_ptr = NULL;
        return;
    }
    
    size_t len = strlen(*src_ptr);
    *dest_ptr = malloc(len + 1);
    if (!*dest_ptr) return;
    
    for (size_t i = 0; i < len; i++) {
        (*dest_ptr)[i] = toupper((*src_ptr)[i]);
    }
    (*dest_ptr)[len] = '\0';
}

// Map
void string_map_to_lower(void* dest, const void* src) {
    const char* const* src_ptr = (const char* const*)src;
    char** dest_ptr = (char**)dest;
    
    if (*src_ptr == NULL) {
        *dest_ptr = NULL;
        return;
    }
    
    size_t len = strlen(*src_ptr);
    *dest_ptr = malloc(len + 1);
    if (!*dest_ptr) return;
    
    for (size_t i = 0; i < len; i++) {
        (*dest_ptr)[i] = tolower((*src_ptr)[i]);
    }
    (*dest_ptr)[len] = '\0';
}

// Predicate for Strings
int string_not_empty(const void* elem) {
    // Pointer to const pointer: can't change the *str_ptr (pointer in the array) and the exact data
    const char* const* str_ptr = (const char* const*)elem;
    return (*str_ptr != NULL && strlen(*str_ptr) > 0);
}

// Predicate for Strings
int string_starts_with_a(const void* elem) {
    // Pointer to const pointer: can't change the *str_ptr (pointer in the array) and the exact data
    const char* const* str_ptr = (const char* const*)elem;
    if (*str_ptr == NULL || strlen(*str_ptr) == 0) return 0;
    char first = (*str_ptr)[0];
    return (first == 'a' || first == 'A');
}