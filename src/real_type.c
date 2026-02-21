#include "real_type.h"

#include "type_info.h"

// Initializing the field of TypeInfo for Real numbers
static const TypeInfo RealType = { // static
    .item_size = sizeof(double),

    .compare_asc = real_compare_asc,
    .compare_desc = real_compare_desc,
    .copy = real_copy,
    .destroy = real_destroy
};

// Getter of RealType
const TypeInfo* ofReal() {
    return &RealType;
}

int real_compare_asc(const void *left, const void *right) {
    // left > right:  1
    // left < right: -1
    // left == right: 0
    double double_left = *(const double*)left; 
    double double_right = *(const double*)right;
    return (double_left > double_right) - (double_left < double_right);
}

int real_compare_desc(const void *left, const void *right) {
    return -real_compare_asc(left, right);
}

void real_copy(void *dest, const void *src) {
    *(double *)dest = *(double *)src;
}

void real_destroy(void *data) { 
    
}
