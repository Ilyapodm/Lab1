#include <stdio.h>
#include <stdlib.h>

// функция для обмена переменных между собой
void swap(void *a, void *b, size_t size) {
    char *res_a = a;
    char *res_b = b;
    char *temp = malloc(size);
    if (!temp) return;
    // temp <- res_a
    // res_a <- res_b
    // res_b <- temp
    for (int i = 0; i < size; i++) {
        *(temp + i) = *(res_a + i);
        *(res_a + i) = *(res_b + i);
        *(res_b + i) = *(temp + i);
    }

    free(temp);
}

int main(void) {
    int ar[10] = {[5] = 5, [3] = 10000};

    return 0;
}

