#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <stddef.h>  

// General type description
typedef struct {
    size_t item_size;

    // Methods of a type
    int  (*compare)(const void *left, const void *right);
    void (*copy)   (void *dest, const void *src); // src - the other cell of the array
    void (*create) (void *dest, const void *raw); // for user's raw data (not char** for strings)
    void (*destroy)(void *data);
    void (*print)  (const void *element);
} TypeInfo;

#endif // TYPE_INFO_H
