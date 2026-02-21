#ifndef REAL_TYPE_H
#define REAL_TYPE_H
#include "type_info.h"

const TypeInfo* ofReal();
int real_compare_asc(const void *left, const void *right);
int real_compare_desc(const void *left, const void *right);
void real_copy(void *dest, const void *src);
void real_destroy(void *data);

#endif // REAL_TYPE_H