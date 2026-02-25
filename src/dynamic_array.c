#include "dynamic_array.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct DynamicArray {    
    size_t size;
    size_t capacity;
    void *data;
    const TypeInfo *type; // pointer to const
};

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
    arr->type->create(dest, value); // Use exactly create (give a value, not pointer to value)
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

// Returns the type of the array
const TypeInfo* dyn_array_type(const DynamicArray* arr) {
    return arr ? arr->type : NULL;
}

// Ascending sort
void dyn_array_sort_asc(DynamicArray* arr) {
    if (!arr || !arr->type || !arr->type->compare || arr->size <= 1) return;
    qsort(arr->data, arr->size, arr->type->item_size, arr->type->compare);
}

// Pointer to any compare function
static int (*type_compare)(const void*, const void*) = NULL;

// Adapter uses the general type's compare function for descending compare
static int compare_desc_adapter(const void* a, const void* b) {
    // Descending means swapping the arguments
    return type_compare(b, a);
}

// Descending sort
void dyn_array_sort_desc(DynamicArray* arr) {
    if (!arr || !arr->type || !arr->type->compare || arr->size <= 1) return;

    type_compare = arr->type->compare;
    qsort(arr->data, arr->size, arr->type->item_size, compare_desc_adapter);
    type_compare = NULL;
}

static void merge(DynamicArray *arr, size_t left, size_t mid, size_t right) {
    size_t size_left = mid - left + 1;
    size_t size_right = right - mid;
    size_t item_size = arr->type->item_size;

    // Create 2 buffers for each subarray
    void *buf_left = malloc(item_size * size_left);
    void *buf_right = malloc(item_size * size_right);
    
    // Fallback
    if (!buf_left || !buf_right) {
        free(buf_left);
        free(buf_right);
        return;
    }

    // Copy from arr to buffers
    memcpy(buf_left, (char*)arr->data + left * item_size, item_size * size_left);
    memcpy(buf_right, (char*)arr->data + (mid + 1) * item_size, item_size * size_right);

    size_t left_index = 0, right_index = 0, arr_index = left;
    while(left_index < size_left && right_index < size_right) {
        void *left_item = (char*)buf_left + left_index * item_size;
        void *right_item = (char*)buf_right + right_index * item_size;

        if (arr->type->compare(left_item, right_item)) {
            memcpy((char*)arr->data + arr_index * item_size, (char*)buf_left + left_index * item_size, item_size);
            arr_index++;
            left_index++;
        }

        else {
            memcpy((char*)arr->data + arr_index * item_size, (char*)buf_right + right_index * item_size, item_size);
            arr_index++;
            right_index++;
        }
    }

    // On of the buffers is empty, have to clear the other
    while(left_index < size_left) {
        memcpy((char*)arr->data + arr_index * item_size, (char*)buf_left + left_index * item_size, item_size);
            arr_index++;
            left_index++;
    }

    while (right_index < size_right) {
        memcpy((char*)arr->data + arr_index * item_size, (char*)buf_right + right_index * item_size, item_size);
            arr_index++;
            right_index++;
    }

    free(buf_left);
    free(buf_right);
}

void dyn_array_merge_sort(DynamicArray *arr, size_t left, size_t right) {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    dyn_array_merge_sort(arr, left, mid);
    dyn_array_merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void dyn_array_print(const DynamicArray* arr) {
    if (!arr || !arr->type || !arr->type->print) 
        return;
    if (arr->size == 0) { 
        printf("[]\n");
        return; 
    }

    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        const void* elem = (const char*)arr->data + i * arr->type->item_size;
        arr->type->print(elem);
        if (i < arr->size - 1) printf(", ");
    }
    printf("]\n");
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
    result->size = 0; // Set at 0 and increase in process to have current size if the copy fails
    
    // Copy from the first arr
    for (size_t i = 0; i < a->size; i++) {
        void *src = (char*)a->data + (i * a->type->item_size);
        void *dst = (char*)result->data + (i * a->type->item_size);
        a->type->copy(dst, src);
        result->size++;
    }
    
    // Copy from the second arr
    for (size_t i = 0; i < b->size; i++) {
        void* src = (char*)b->data + (i * b->type->item_size);
        void* dst = (char*)result->data + ((a->size + i) * b->type->item_size); // already include the first copy size
        b->type->copy(dst, src);
        result->size++;
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
    result->size = 0; // Set at 0 and increase in process to have current size if the copy fails
    result->capacity = arr->size;
    
    for (size_t i = 0; i < arr->size; i++) {
        void* src = (char*)arr->data + (i * arr->type->item_size);
        void* dst = (char*)result->data + (i * arr->type->item_size);
        mapper(dst, src);   // Mapper makes the map and transports at the same time
        result->size++;
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