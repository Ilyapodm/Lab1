#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "dynamic_array.h"
#include "real_type.h"
#include "string_type.h"

#define MAX_ARRAY_SIZE 100000000ULL

/********************************************************************
 *Utils for Real Numbers 
 *******************************************************************/

void add_real_element(DynamicArray* arr) {
    double value;
    printf("Введите число: ");
    if (scanf("%lf", &value) != 1) {
        printf("Ошибка ввода!\n");
        while (getchar() != '\n'); // Clear the buffer
        return;
    }
    if (dyn_array_push_back(arr, &value) == 0) {
        printf("Элемент добавлен.\n");
        while (getchar() != '\n');
    } else {
        printf("Ошибка добавления элемента.\n");
        while (getchar() != '\n');
    }
}

void pop_real_element(DynamicArray* arr) {
    if (dyn_array_size(arr) == 0) {
        printf("Массив пуст!\n");
        return;
    }
    
    double popped;
    if (dyn_array_pop(arr, &popped)) {
        printf("Удалён элемент: %.2f\n", popped);
    } else {
        printf("Ошибка удаления.\n");
    }
}

void get_real_element(DynamicArray* arr) {
    size_t index;
    printf("Введите индекс (0-%zu): ", dyn_array_size(arr) - 1);
    if (scanf("%zu", &index) != 1 || index >= dyn_array_size(arr)) {
        printf("Неверный индекс!\n");
        while (getchar() != '\n');
        return;
    }
    
    const double* val = dyn_array_get(arr, index);
    printf("Элемент[%zu] = %.2f\n", index, *val);
    while (getchar() != '\n');
}

/********************************************************************
 *Utils for Strings 
 *******************************************************************/

void add_string_element(DynamicArray* arr) {
    char buffer[1024];
    printf("Введите строку: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) { 
        // fgets knows the limits of the buffer + reads the spaces, but saves \n
        printf("Ошибка ввода!\n");
        return;
    }
    
    // Delete the \n
    buffer[strcspn(buffer, "\n")] = '\0'; // strcspn return the first index of the \n
    
    if (dyn_array_push_back(arr, buffer) == 0) {
        printf("Строка добавлена.\n");
    } else {
        printf("Ошибка добавления строки.\n");
    }
}

void pop_string_element(DynamicArray* arr) {
    if (dyn_array_size(arr) == 0) {
        printf("Массив пуст!\n");
        return;
    }
    
    char* popped = NULL;
    if (dyn_array_pop(arr, &popped)) {
        if (popped) {
            printf("Удалена строка: \"%s\"\n", popped);
        } else {
            printf("Удалена NULL-строка.\n");
        }
    } else {
        printf("Ошибка удаления.\n");
    }
    free(popped);
}

void get_string_element(DynamicArray* arr) {
    size_t index;
    printf("Введите индекс (0-%zu): ", dyn_array_size(arr) - 1);
    if (scanf("%zu", &index) != 1 || index >= dyn_array_size(arr)) {
        printf("Неверный индекс!\n");
        while (getchar() != '\n');
        return;
    }
    
    // Pointer to const pointer: can't change the *str_ptr (pointer in the array) and the exact data
    const char* const* str_ptr = (const char* const*)dyn_array_get(arr, index); 
    const char* str = *str_ptr;
    printf("Элемент[%zu] = \"%s\"\n", index, str ? str : "NULL");
    while (getchar() != '\n');
}

/********************************************************************
 * Main Menu Utils 
 *******************************************************************/
void wait_for_enter() {
    printf("\nНажмите Enter для продолжения...\n");
    while (getchar() != '\n');
}

void clear_screen() {
    #ifdef _WIN32 // Crossplatform, I have never used
        system("cls");
    #else
        system("clear");
    #endif
}

/********************************************************************
 * Menu for Real Numbers Array 
 *******************************************************************/

void double_array_menu() {
    DynamicArray* arr = dyn_array_create(ofReal());
    if (!arr) {
        printf("Ошибка создания массива!\n");
        return;
    }
    
    int running = 1;
    
    while (running) {
        clear_screen();
        printf("===== МЕНЮ РАБОТЫ С МАССИВОМ ЧИСЕЛ (double) =====\n\n");
        printf("Текущий массив: ");
        dyn_array_print(arr);
        printf("\n");
        printf("1. Добавить элемент\n");
        printf("2. Удалить последний элемент (pop)\n");
        printf("3. Получить элемент по индексу\n");
        printf("4. Показать размер и вместимость\n");
        printf("5. Сортировка по возрастанию\n");
        printf("6. Сортировка по убыванию\n");
        printf("7. Сжать память (shrink_to_fit)\n");
        printf("8. Map: умножить все на 2\n");
        printf("9. Map: возвести в квадрат\n");
        printf("10. Map: прибавить 1 ко всем\n");
        printf("11. Where: только положительные\n");
        printf("12. Where: только отрицательные\n");
        printf("13. Конкатенация с другим массивом\n");
        printf("0. Выйти (массив будет уничтожен)\n");
        printf("\nВыберите действие: ");
        
        int choice = -1;
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n'); // Immediately clear the buffer
        
        switch (choice) {
            case 1:
                add_real_element(arr);
                wait_for_enter();
                break;
                
            case 2:
                pop_real_element(arr);
                wait_for_enter();
                break;
                
            case 3:
                if (dyn_array_size(arr) > 0) {
                    get_real_element(arr);
                } else {
                    printf("Массив пуст!\n");
                }
                wait_for_enter();
                break;
                
            case 4:
                printf("Размер: %zu\n", dyn_array_size(arr));
                printf("Вместимость: %zu\n", dyn_array_capacity(arr));
                wait_for_enter();
                break;
                
            case 5:
                dyn_array_sort_asc(arr);
                printf("Отсортировано по возрастанию.\n");
                wait_for_enter();
                break;
                
            case 6:
                dyn_array_sort_desc(arr);
                printf("Отсортировано по убыванию.\n");
                wait_for_enter();
                break;
                
            case 7:
                if (dyn_array_shrink_to_fit(arr)) {
                    printf("Память сжата. Новая вместимость: %zu\n", dyn_array_capacity(arr));
                } else {
                    printf("Ошибка сжатия памяти.\n");
                }
                wait_for_enter();
                break;
                
            case 8: {
                DynamicArray* mapped = dyn_array_map(arr, real_map_multiply);
                if (mapped) {
                    printf("Результат map (умножить на 2):\n");
                    dyn_array_print(mapped);
                    dyn_array_destroy(mapped);
                } else {
                    printf("Ошибка map.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 9: {
                DynamicArray* mapped = dyn_array_map(arr, real_map_square);
                if (mapped) {
                    printf("Результат map (в квадрате):\n");
                    dyn_array_print(mapped);
                    dyn_array_destroy(mapped);
                } else {
                    printf("Ошибка map.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 10: {
                DynamicArray* mapped = dyn_array_map(arr, real_map_add_one);
                if (mapped) {
                    printf("Результат map (+1):\n");
                    dyn_array_print(mapped);
                    dyn_array_destroy(mapped);
                } else {
                    printf("Ошибка map.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 11: {
                DynamicArray* filtered = dyn_array_where(arr, is_positive);
                if (filtered) {
                    printf("Результат where (только положительные):\n");
                    dyn_array_print(filtered);
                    dyn_array_destroy(filtered);
                } else {
                    printf("Ошибка where.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 12: {
                DynamicArray* filtered = dyn_array_where(arr, is_negative);
                if (filtered) {
                    printf("Результат where (только отрицательные):\n");
                    dyn_array_print(filtered);
                    dyn_array_destroy(filtered);
                } else {
                    printf("Ошибка where.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 13: {
                // Creates the second array for concat
                DynamicArray* arr2 = dyn_array_create(ofReal());
                printf("Создан второй массив. Добавьте в него элементы:\n");
                
                int num_elements;
                printf("Сколько элементов добавить во второй массив? ");
                if (scanf("%d", &num_elements) == 1 && num_elements > 0) {
                    while (getchar() != '\n'); // Clear the buffer before cycle
                    for (int i = 0; i < num_elements; i++) {
                        double val;
                        printf("  Элемент %d: ", i + 1);
                        if (scanf("%lf", &val) == 1) {
                            dyn_array_push_back(arr2, &val);
                        }
                    }
                    
                    DynamicArray* concatenated = dyn_array_concat(arr, arr2);
                    if (concatenated) {
                        printf("Результат конкатенации:\n");
                        dyn_array_print(concatenated);
                        dyn_array_destroy(concatenated);
                    } else {
                        printf("Ошибка конкатенации.\n");
                    }
                }
                
                dyn_array_destroy(arr2);
                wait_for_enter();
                break;
            }
                
            case 0:
                running = 0;
                break;
                
            default:
                printf("Неверный выбор!\n");
                wait_for_enter();
        }
    }
    
    dyn_array_destroy(arr);
    printf("Массив уничтожен.\n");
}

/********************************************************************
 * Menu for String Array 
 *******************************************************************/

void string_array_menu() {
    DynamicArray* arr = dyn_array_create(ofString());
    if (!arr) {
        printf("Ошибка создания массива!\n");
        return;
    }
    
    int running = 1;
    
    while (running) {
        clear_screen();
        printf("===== МЕНЮ РАБОТЫ С МАССИВОМ СТРОК (char*) =====\n\n");
        printf("Текущий массив: ");
        dyn_array_print(arr);
        printf("\n");
        printf("1. Добавить строку\n");
        printf("2. Удалить последнюю строку (pop)\n");
        printf("3. Получить строку по индексу\n");
        printf("4. Показать размер и вместимость\n");
        printf("5. Сортировка по алфавиту (A-Я)\n");
        printf("6. Сортировка по алфавиту (Я-А)\n");
        printf("7. Сжать память (shrink_to_fit)\n");
        printf("8. Map: в верхний регистр\n");
        printf("9. Map: в нижний регистр\n");
        printf("10. Where: только непустые строки\n");
        printf("11. Where: начинающиеся с 'a' или 'A'\n");
        printf("12. Конкатенация с другим массивом\n");
        printf("0. Выйти (массив будет уничтожен)\n");
        printf("\nВыберите действие: ");
        
        int choice = -1;
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        // Clear the buffer
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                add_string_element(arr);
                wait_for_enter();
                break;
                
            case 2:
                pop_string_element(arr);
                wait_for_enter();
                break;
                
            case 3:
                if (dyn_array_size(arr) > 0) {
                    get_string_element(arr);
                } else {
                    printf("Массив пуст!\n");
                }
                wait_for_enter();
                break;
                
            case 4:
                printf("Размер: %zu\n", dyn_array_size(arr));
                printf("Вместимость: %zu\n", dyn_array_capacity(arr));
                wait_for_enter();
                break;
                
            case 5:
                dyn_array_sort_asc(arr);
                printf("Отсортировано по алфавиту (A-Я).\n");
                wait_for_enter();
                break;
                
            case 6:
                dyn_array_sort_desc(arr);
                printf("Отсортировано по алфавиту (Я-А).\n");
                wait_for_enter();
                break;
                
            case 7:
                if (dyn_array_shrink_to_fit(arr)) {
                    printf("Память сжата. Новая вместимость: %zu\n", dyn_array_capacity(arr));
                } else {
                    printf("Ошибка сжатия памяти.\n");
                }
                wait_for_enter();
                break;
                
            case 8: {
                DynamicArray* mapped = dyn_array_map(arr, string_map_to_upper);
                if (mapped) {
                    printf("Результат map (в верхний регистр):\n");
                    dyn_array_print(mapped);
                    dyn_array_destroy(mapped);
                } else {
                    printf("Ошибка map.\n");
                }
                wait_for_enter();
                break;
            }
            case 9: {
                DynamicArray* mapped = dyn_array_map(arr, string_map_to_lower);
                if (mapped) {
                    printf("Результат map (в нижний регистр):\n");
                    dyn_array_print(mapped);
                    dyn_array_destroy(mapped);
                } else {
                    printf("Ошибка map.\n");
                }
                wait_for_enter();
                break;
            }
            
            case 10: {
                DynamicArray* filtered = dyn_array_where(arr, string_not_empty);
                if (filtered) {
                    printf("Результат where (непустые строки):\n");
                    dyn_array_print(filtered);
                    dyn_array_destroy(filtered);
                } else {
                    printf("Ошибка where.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 11: {
                DynamicArray* filtered = dyn_array_where(arr, string_starts_with_a);
                if (filtered) {
                    printf("Результат where (начинаются с 'a'/'A'):\n");
                    dyn_array_print(filtered);
                    dyn_array_destroy(filtered);
                } else {
                    printf("Ошибка where.\n");
                }
                wait_for_enter();
                break;
            }
                
            case 12: {
                // Creates the second array for concat
                DynamicArray* arr2 = dyn_array_create(ofString());
                printf("Создан второй массив. Добавьте в него строки:\n");
                
                int num_elements;
                printf("Сколько строк добавить во второй массив? ");
                if (scanf("%d", &num_elements) == 1 && num_elements > 0) {
                    // Clear the buffer
                    while (getchar() != '\n');
                    
                    for (int i = 0; i < num_elements; i++) {
                        char buffer[1024];
                        printf("  Строка %d: ", i + 1);
                        if (fgets(buffer, sizeof(buffer), stdin)) {
                            buffer[strcspn(buffer, "\n")] = '\0';    
                            dyn_array_push_back(arr2, buffer);
                        }
                    }
                    
                    DynamicArray* concatenated = dyn_array_concat(arr, arr2);
                    if (concatenated) {
                        printf("Результат конкатенации:\n");
                        dyn_array_print(concatenated);
                        dyn_array_destroy(concatenated);
                    } else {
                        printf("Ошибка конкатенации.\n");
                    }
                }
                
                dyn_array_destroy(arr2);
                wait_for_enter();
                break;
            }
                
            case 0:
                running = 0;
                break;
                
            default:
                printf("Неверный выбор!\n");
                wait_for_enter();
        }
    }
    
    dyn_array_destroy(arr);
    printf("Массив уничтожен.\n");
}

/********************************************************************
 * Sort compare
 *******************************************************************/

void compare_sort_real(unsigned long long size) {
    DynamicArray* merge_sort_arr = dyn_array_create(ofReal());
    DynamicArray* qsort_arr = dyn_array_create(ofReal());

    // Adding a random number to 2 arrays
    for (unsigned long long arr_index = 0; arr_index < size; arr_index++) {
        double random_real = (double)rand() / RAND_MAX * 200000.0 - 100000.0;
        dyn_array_push_back(merge_sort_arr, &random_real);
        dyn_array_push_back(qsort_arr, &random_real);
    }

    printf("Выполняем merge sort\n");
    // Measuring the merge sort time
    clock_t start_merge_sort = clock();
    dyn_array_merge_sort(merge_sort_arr, 0, size-1);
    clock_t end_merge_sort = clock();
    double time_merge_sort = (double)(end_merge_sort - start_merge_sort) / CLOCKS_PER_SEC;
    
    printf("Выполняем qsort\n");
    // Measuring the qsort time
    clock_t start_qsort = clock();
    dyn_array_sort_asc(qsort_arr);
    clock_t end_qsort = clock();
    double time_qsort = (double)(end_qsort - start_qsort) / CLOCKS_PER_SEC;

    dyn_array_destroy(merge_sort_arr);
    dyn_array_destroy(qsort_arr);

    printf("Время моей merge sort для %llu действительных чисел: %.6f сек\n", size, time_merge_sort);
    printf("Время встроенной qsort для %llu действительных чисел: %.6f сек\n", size, time_qsort);
    if (time_merge_sort > time_qsort) 
        printf("Qsort для %llu действительных чисел была выполнена быстрее, чем merge sort на %f секунд\n", size, time_merge_sort - time_qsort);
    else 
        printf("Merge sort для %llu действительных чисел была выполнена быстрее, чем qsort на %f секунд\n", size, time_qsort - time_merge_sort);
    wait_for_enter();
}

void compare_sort_string(unsigned long long size) {
    DynamicArray* merge_sort_arr = dyn_array_create(ofString());
    DynamicArray* qsort_arr = dyn_array_create(ofString());

    srand(time(NULL));

    const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

        int charset_size = strlen(charset);

        char random_string[11]; // Random word

    for (unsigned long long arr_index = 0; arr_index < size; arr_index++) {
        // Randomize a len of the string (1-10) + 1 symbol for '\0'
        int string_len = (rand() % 10) + 1;

        int symbol = 0;
        for (; symbol < string_len; symbol++){
            random_string[symbol] = charset[rand() % charset_size];
        }

        random_string[symbol] = '\0';

        dyn_array_push_back(merge_sort_arr, random_string); 
        dyn_array_push_back(qsort_arr, random_string);
    }

    printf("Выполняем merge sort\n");
    clock_t start_merge_sort = clock();
    dyn_array_merge_sort(merge_sort_arr, 0, size-1);
    clock_t end_merge_sort = clock();
    double time_merge_sort = (double)(end_merge_sort - start_merge_sort) / CLOCKS_PER_SEC;
    

    printf("Выполняем qsort\n");
    clock_t start_qsort = clock();
    dyn_array_sort_asc(qsort_arr);
    clock_t end_qsort = clock();
    double time_qsort = (double)(end_qsort - start_qsort) / CLOCKS_PER_SEC;

    dyn_array_destroy(merge_sort_arr);
    dyn_array_destroy(qsort_arr);

    printf("Время моей merge sort для %llu строк: %.6f сек\n", size, time_merge_sort);
    printf("Время встроенной qsort для %llu строк: %.6f сек\n", size, time_qsort);
    if (time_merge_sort > time_qsort) 
        printf("Qsort для %llu строк была выполнена быстрее, чем merge sort на %f секунд\n", size, time_merge_sort - time_qsort);
    else 
        printf("Merge sort для %llu строк была выполнена быстрее, чем qsort на %f секунд\n", size, time_qsort - time_merge_sort);
    wait_for_enter();
}

void compare_sort() {
    int running = 1;
    
    while (running) {
        long long size = 1000000;
        clear_screen();
        printf("Сравнение сортировок merge sort и qsort для n элементов\n");
        printf("Введите количество элементов в массивах или 0 для выхода в меню:\n");
        if (scanf("%lld", &size) != 1 || size > MAX_ARRAY_SIZE) {
            while (getchar() != '\n');
            printf("Введите положительное число!\n");
            continue;
        }

        if (size < 0) {
            printf("Введите положительное число!\n");
            wait_for_enter();
            continue;
        }

        if (size == 0) {
            running = 0;
            break;
        }

        printf("\n1. Вещественные числа (double)\n");
        printf("2. Строки (char *)\n");
        printf("0. Выйти\n");
        printf("Выберете тип данных для массивов:\n");

        int choice = -1;
            
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
            
        // Clear the buffer
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                compare_sort_real(size);
                break;
                
            case 2:
                compare_sort_string(size);
                break;

            case 0:
                running = 0;
                break;

            default:
                printf("Неверный выбор!\n");
                wait_for_enter();
        }
    }
}

/********************************************************************
 * Main Menu
 *******************************************************************/

int main() {
    srand(time(NULL)); // for compare
    int running = 1; 
    
    printf("===== ДИНАМИЧЕСКИЙ МАССИВ НА СИ =====\n\n");
    
    while (running) {
        printf("\nГлавное меню:\n");
        printf("1. Работа с массивом чисел (double)\n");
        printf("2. Работа с массивом строк (char*)\n");
        printf("3. (Бонус) Сравнение времени работы merge sort и qsort\n");
        printf("0. Выход из программы\n");
        printf("\nВыберите тип массива: ");

        int choice = -1;

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n'); // Immediately clear the buffer
        
        switch (choice) {
            case 1:
                double_array_menu();
                break;
                
            case 2:
                string_array_menu();
                break;

            case 3:
                compare_sort();
                break;
                
            case 0:
                running = 0;
                printf("Выход из программы. До свидания!\n");
                break;
                
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
    }
    
    return 0;
}