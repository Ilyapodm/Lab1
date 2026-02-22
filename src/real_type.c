#include "real_type.h"
#include <stdio.h>
#include "type_info.h"

static int real_compare(const void *left, const void *right);
static void real_copy(void *dest, const void *src);
static void real_create(void *dest, const void *src);
static void real_destroy(void *data);
static void real_print(const void* elem);

/********************************************************************
 * TypeInfo realization  
 *******************************************************************/

// Initializing the fields of TypeInfo for Real numbers
static const TypeInfo RealType = {
    .item_size = sizeof(double),

    .compare = real_compare,
    .copy = real_copy,
    .create = real_create,
    .destroy = real_destroy,
    .print = real_print
};

// Getter of RealType
const TypeInfo* ofReal() {
    return &RealType;
}

// Compare for Real Numbers
static int real_compare(const void *left, const void *right) {
    // left > right:  1
    // left < right: -1
    // left == right: 0
    double double_left = *(const double*)left; 
    double double_right = *(const double*)right;
    return (double_left > double_right) - (double_left < double_right);
}

// Copy for Real Numbers
static void real_copy(void *dest, const void *src) {
    *(double *)dest = *(double *)src;
}

// Create Real Numbers
static void real_create(void *dest, const void *src) {
    // The same as real_copy, because an item is already a value
    *(double *)dest = *(const double *)src;  
}

// Destroy for Real Numbers
static void real_destroy(void *data) { 
    
}

// Print for Real Numbers
static void real_print(const void* elem) {
    printf("%.2f", *(const double*)elem);
}

/********************************************************************
 * Map and Where realization  
 *******************************************************************/

// Map 
void real_map_multiply(void* dest, const void* src) {
    *(double*)dest = *(const double*)src * 2.0;
}

// Map
void real_map_square(void* dest, const void* src) {
    *(double*)dest = *(const double*)src * *(const double*)src;
}

// Map 
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