#include "string_type.h"
#include "type_info.h"
#include <string.h>

// Initializing the field of TypeInfo for Strings
static const TypeInfo StringType = {
    .item_size = sizeof(char*), // 8 bytes, not a 

    .compare_asc = string_compare_asc,
    .compare_desc = string_compare_desc,
    .copy = string_copy,
    .destroy = string_destroy
};

// Getter of StringType
const TypeInfo* ofString() {
    return &StringType;
}


int string_compare_asc(const void *left, const void *right) {
    const char *str_left = *(char**)left;
    const char *str_right = *(char**)right;

    if (!str_left && !str_right) // ??
        return 0;
    if (!str_left)
        return -1;
    if (!str_right) 
        return 1;

    return strcmp(str_left, str_right);
}

int string_compare_desc(const void *left, const void *right) {
    return -string_compare_asc(left, right);
}

void string_copy(void *dest, const void *src) { 
    char **dest_str = (char**)dest;
    const char *src_str = *(char**)src;

    if (src_str) {
        size_t len = strlen(src_str);

        char *new_str = malloc(len + 1);

        if (!new_str) {
            *dest_str = NULL;
            return;
        }

        strcpy(new_str, src_str);
        
        *dest_str = new_str;
    }
    else   
        *dest_str = NULL;
}


void string_destroy(void *data) {
    char *str = *(char**)data;
    free(str);
}