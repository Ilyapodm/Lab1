#ifndef LAB1_DYNAMICARRAY_H
#define LAB1_DYNAMICARRAY_H
#include <stddef.h>
// Сам массив
typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t item_size;
} dynArray;

// Получить элемент по индексу

// Добавить новый элемент

// Удалить элемент


#endif //LAB1_DYNAMICARRAY_H