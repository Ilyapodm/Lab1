#include "string_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "type_info.h"

static int string_compare(const void *left, const void *right);
static void string_copy(void *dest, const void *src);
static void string_create(void *dest, const void *src);
static void string_destroy(void *data);
static void string_print(const void* elem);

/********************************************************************
 * TypeInfo realization  
 *******************************************************************/

// Initializing the fields of TypeInfo for Strings
static const TypeInfo StringType = {
    .item_size = sizeof(char*), // 8 bytes, not a string actually

    .compare = string_compare,
    .copy = string_copy,
    .create = string_create,
    .destroy = string_destroy,
    .print = string_print
};

// Getter of StringType
const TypeInfo* ofString() {
    return &StringType;
}

// Compare for Strings
static int string_compare(const void *left, const void *right) {
    const char *str_left = *(char**)left;
    const char *str_right = *(char**)right;

    if (!str_left && !str_right) 
        return 0;
    if (!str_left)
        return -1;
    if (!str_right) 
        return 1;

    return strcmp(str_left, str_right);
}

// Copy for Strings
static void string_copy(void *dest, const void *src) {
    // *(char**)src to get a string
    // dest and src - pointers to data's cell (char**)
    char **dest_str    = (char **)dest;
    const char *src_str = *(const char **)src;  // general string
    if (!src_str) {
        *dest_str = NULL;
        return;
    }
    char *new_str = malloc(strlen(src_str) + 1);
    if (!new_str) {
        *dest_str = NULL;
        return;
    }
    strcpy(new_str, src_str);
    *dest_str = new_str;
}

// Create Strings (from user's input)
static void string_create(void *dest, const void *src) {
    char **dest_str = (char **)dest;
    const char *raw  = (const char *)src; // The difference between creating and copying
    //src is raw user data (char*), unlike copy() where src is a cell (char**)
    if (!raw) {
        *dest_str = NULL;
        return;
    }
    char *new_str = malloc(strlen(raw) + 1);
    if (!new_str) {
        *dest_str = NULL;
        return;
    }
    strcpy(new_str, raw);
    *dest_str = new_str;
}

// Destroy of Strings
static void string_destroy(void *data) {
    char *str = *(char**)data;
    free(str);
}

// Print for Strings
static void string_print(const void* elem) {
    // char** is type of an element of data for strings, make only 1 * to get string
    const char* str = *(const char**)elem;  
    printf("\"%s\"", str ? str : "NULL");
}

/********************************************************************
 * Map and Where realization 
 *******************************************************************/

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