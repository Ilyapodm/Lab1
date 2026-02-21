#ifndef STRING_TYPE_H
#define STRING_TYPE_H
#include "type_info.h"

const TypeInfo* ofString();
int string_compare_asc(const void *left, const void *right);
int string_compare_desc(const void *left, const void *right);
void string_copy(void *dest, const void *src);
void string_destroy(void *data);

#endif // STRING_TYPE_H