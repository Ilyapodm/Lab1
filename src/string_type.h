#ifndef STRING_TYPE_H
#define STRING_TYPE_H
#include "type_info.h"

const TypeInfo* ofString();

// Map
void string_map_to_upper(void* dest, const void* src);
void string_map_to_lower(void* dest, const void* src);

// Where
int string_not_empty(const void* elem);
int string_starts_with_a(const void* elem);

#endif // STRING_TYPE_H