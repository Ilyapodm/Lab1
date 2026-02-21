#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <stddef.h>  

// General type description
typedef struct {
    size_t item_size;

    // Methods of a type
    int (*compare_asc)(const void *left, const void *right);
    int (*compare_desc)(const void *left, const void *right);
    void (*copy)(void *dest, const void *src);
    void (*destroy)(void *data);
} TypeInfo;

#endif // TYPE_INFO_H