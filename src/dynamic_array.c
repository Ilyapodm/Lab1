#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>

// Creates a new array
DynamicArray* dyn_array_create(const TypeInfo* type) {
    if (!type) return NULL;
    
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    if (!arr) return NULL;
    
    arr->data = NULL;
    arr->type = type;
    arr->size = 0;
    arr->capacity = 0;
    
    return arr;
}

// Destroys an array
void dyn_array_destroy(DynamicArray* arr) {
    if (!arr) return;
    
    // Destroy every element
    if (arr->type && arr->type->destroy && arr->data) {
        for (size_t i = 0; i < arr->size; i++) {
            void* elem = (char*)arr->data + (i * arr->type->item_size);
            arr->type->destroy(elem);
        }
    }
    
    // Destroy the elements' container 
    free(arr->data);
    // Destroy the array
    free(arr);
}

// Adds an element in the end
int dyn_array_push_back(DynamicArray* arr, const void* value) {
    if (!arr || !arr->type || !value) return -1;
    
    // Need more capacity
    if (arr->size >= arr->capacity) {
        size_t new_capacity = (arr->capacity == 0) ? 1 : arr->capacity * 2;
        // If the ptr is NULL, than realloc acts as malloc
        void* new_data = realloc(arr->data, new_capacity * arr->type->item_size);
        
        if (!new_data) return -1; 
        
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    
    // Copy the element in the end of the array
    void* dest = (char*)arr->data + (arr->size * arr->type->item_size);
    arr->type->copy(dest, value);
    arr->size++;
    
    return 0;
}

// Removes an element from the end
int dyn_array_pop(DynamicArray* arr, void* out_buffer) {
    if (!arr || arr->size == 0 || !out_buffer) return 0;
    
    // Put the item_ptr in the beginning of the item
    void* item_ptr = (char*)arr->data + ((arr->size - 1) * arr->type->item_size);
    arr->type->copy(out_buffer, item_ptr); 
    
    if (arr->type->destroy) {
        arr->type->destroy(item_ptr); 
    }
    
    arr->size--;
    return 1;
}

// Returns the element by index
const void* dyn_array_get(const DynamicArray* arr, size_t index) {
    if (!arr || index >= arr->size || !arr->data) return NULL;
    return (const char*)arr->data + (index * arr->type->item_size);
}

// Returns the size of the array
size_t dyn_array_size(const DynamicArray* arr) {
    return arr ? arr->size : 0;
}

// Returns the capacity of the array
size_t dyn_array_capacity(const DynamicArray* arr) {
    return arr ? arr->capacity : 0;
}

// Ascending sort
void dyn_array_sort_asc(DynamicArray* arr) {
    if (!arr || !arr->type || !arr->type->compare_asc || arr->size <= 1) return;
    qsort(arr->data, arr->size, arr->type->item_size, arr->type->compare_asc);
}

// Descending sort
void dyn_array_sort_desc(DynamicArray* arr) {
    if (!arr || !arr->type || !arr->type->compare_desc || arr->size <= 1) return;
    qsort(arr->data, arr->size, arr->type->item_size, arr->type->compare_desc);
}


int dyn_array_shrink_to_fit(DynamicArray* arr) {
    if (!arr || arr->size == 0) {
        free(arr->data);
        arr->data = NULL;
        arr->capacity = 0;
        return 1;
    }
    // Unable to shrink
    if (arr->size >= arr->capacity) return 1;
    
    void* new_data = realloc(arr->data, arr->size * arr->type->item_size);
    if (!new_data) return 0; // If realloc doesn't work - don't shrink
    
    arr->data = new_data;
    arr->capacity = arr->size;
    return 1;
}

DynamicArray* dyn_array_concat(const DynamicArray* a, const DynamicArray* b) {
    if (!a || !b || a->type != b->type) return NULL; // Only same type are possible to concat
    
    DynamicArray* result = dyn_array_create(a->type);
    if (!result) return NULL;
    
    size_t total = a->size + b->size;
    if (total == 0) return result; 
    
    // Do not use the push_back a lot of times, it's uneffective
    result->data = malloc(total * a->type->item_size);
    if (!result->data) {
        dyn_array_destroy(result);
        return NULL;
    }
    result->capacity = total;
    result->size = total;
    
    // Copy from the first arr
    for (size_t i = 0; i < a->size; i++) {
        void* src = (char*)a->data + (i * a->type->item_size);
        void* dst = (char*)result->data + (i * a->type->item_size);
        a->type->copy(dst, src);
    }
    
    // Copy from the second arr
    for (size_t i = 0; i < b->size; i++) {
        void* src = (char*)b->data + (i * b->type->item_size);
        void* dst = (char*)result->data + ((a->size + i) * b->type->item_size); // already include the first copy size
        b->type->copy(dst, src);
    }
    
    return result;
}

// Map (changes every element in the array)
DynamicArray* dyn_array_map(const DynamicArray* arr, MapFunc mapper) {
    if (!arr || !mapper) return NULL;
    
    DynamicArray* result = dyn_array_create(arr->type);
    if (!result) return NULL;
    
    if (arr->size == 0) return result;
    
    result->data = malloc(arr->size * arr->type->item_size);
    if (!result->data) {
        dyn_array_destroy(result);
        return NULL;
    }
    result->size = arr->size;
    result->capacity = arr->size;
    
    for (size_t i = 0; i < arr->size; i++) {
        void* src = (char*)arr->data + (i * arr->type->item_size);
        void* dst = (char*)result->data + (i * arr->type->item_size);
        mapper(dst, src);   // Mapper makes the map and transports at the same time
    }
    
    return result;
}

// Filters the array
DynamicArray* dyn_array_where(const DynamicArray* arr, Predicate pred) {
    if (!arr || !pred) return NULL;
    
    DynamicArray* result = dyn_array_create(arr->type);
    if (!result) return NULL; 
    
    if (arr->size == 0) return result;
    
    // Malloc for all elements (the worst case)
    result->data = malloc(arr->size * arr->type->item_size);
    if (!result->data) {
        dyn_array_destroy(result);
        return NULL;
    }
    result->capacity = arr->size;
    
    // Filter
    for (size_t i = 0; i < arr->size; i++) {
        void* elem = (char*)arr->data + (i * arr->type->item_size);
        if (pred(elem)) {
            void* dst = (char*)result->data + (result->size * arr->type->item_size);
            arr->type->copy(dst, elem);
            result->size++;
        }
    }

    dyn_array_shrink_to_fit(result); // "Fixes" the malloc for all elements
    return result;
}