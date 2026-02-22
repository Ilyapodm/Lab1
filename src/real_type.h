#ifndef REAL_TYPE_H
#define REAL_TYPE_H
#include "type_info.h"

const TypeInfo* ofReal();

// Map
void real_map_multiply(void* dest, const void* src);
void real_map_square(void* dest, const void* src);
void real_map_add_one(void* dest, const void* src);

// Where
int is_positive(const void* elem);
int is_negative(const void* elem);

#endif // REAL_TYPE_H