#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "type_info.h"

typedef struct {
    size_t size;
    size_t capacity;
    void *data;
    const TypeInfo *type; // pointer to const
} DynamicArray;

DynamicArray* dyn_array_create(const TypeInfo* type); 
void dyn_array_destroy(DynamicArray *arr);

// Dynamic Array's working with elements
int dyn_array_push_back(DynamicArray *arr, const void *value);
int dyn_array_pop(DynamicArray *arr, void *out_buffer);

// Dynamic Array's getters
const void* dyn_array_get(const DynamicArray* arr, size_t index);
size_t dyn_array_size(const DynamicArray *arr);
size_t dyn_array_capacity(const DynamicArray *arr);

// Dynamic Array's sorts
void dyn_array_sort_asc(DynamicArray *arr);
void dyn_array_sort_desc(DynamicArray *arr);


int dyn_array_shrink_to_fit(DynamicArray *arr);

typedef void (*MapFunc)(void *dest, const void *src);
typedef int(*Predicate)(const void* element);

DynamicArray* dyn_array_map(const DynamicArray* arr, MapFunc mapper);
DynamicArray* dyn_array_where(const DynamicArray* arr, Predicate pred);
DynamicArray* dyn_array_concat(const DynamicArray* a, const DynamicArray* b);  
#endif // DYNAMIC_ARRAY_H