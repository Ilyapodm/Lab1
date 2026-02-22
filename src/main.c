#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dynamic_array.h"
#include "real_type.h"
#include "string_type.h"
#include "mappers.h"

/********************************************************************
 *Utils for Real Numbers 
 *******************************************************************/

// TODO Вывод/ввод лучше реализовать в самом объекте поле (typeinfo) (набор принтеров в разные места)
void print_real_array(const DynamicArray* arr) {
    if (!arr || arr->size == 0) {
        printf("Массив пуст.\n");
        return;
    }
    
    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        const double* val = dyn_array_get(arr, i);
        printf("%.2f", *val);
        if (i < arr->size - 1) printf(", ");
    }
    printf("]\n");
}

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
    } else {
        printf("Ошибка добавления элемента.\n");
    }
}

void pop_real_element(DynamicArray* arr) {
    if (arr->size == 0) {
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
    printf("Введите индекс (0-%zu): ", arr->size - 1);
    if (scanf("%zu", &index) != 1 || index >= arr->size) {
        printf("Неверный индекс!\n");
        while (getchar() != '\n');
        return;
    }
    
    const double* val = dyn_array_get(arr, index);
    printf("Элемент[%zu] = %.2f\n", index, *val);
}

/********************************************************************
 *Utils for Strings 
 *******************************************************************/
void print_string_array(const DynamicArray* arr) {
    if (!arr || arr->size == 0) {
        printf("Массив пуст.\n");
        return;
    }
    
    printf("[");
    for (size_t i = 0; i < arr->size; i++) {
        const char* const* str_ptr = (const char* const*)dyn_array_get(arr, i);
        const char* str = *str_ptr;
        printf("\"%s\"", str ? str : "NULL");
        if (i < arr->size - 1) printf(", ");
    }
    printf("]\n");
}

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
    
    char* str = strdup(buffer); // malloc + strcpy
    if (!str) {
        printf("Ошибка выделения памяти!\n");
        return;
    }
    
    if (dyn_array_push_back(arr, &str) == 0) {
        printf("Строка добавлена.\n");
        free(str);
    } else {
        free(str);
        printf("Ошибка добавления строки.\n");
    }
}

void pop_string_element(DynamicArray* arr) {
    if (arr->size == 0) {
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
}

void get_string_element(DynamicArray* arr) {
    size_t index;
    printf("Введите индекс (0-%zu): ", arr->size - 1);
    if (scanf("%zu", &index) != 1 || index >= arr->size) {
        printf("Неверный индекс!\n");
        while (getchar() != '\n');
        return;
    }
    
    // Pointer to const pointer: can't change the *str_ptr (pointer in the array) and the exact data
    const char* const* str_ptr = (const char* const*)dyn_array_get(arr, index); 
    const char* str = *str_ptr;
    printf("Элемент[%zu] = \"%s\"\n", index, str ? str : "NULL");
}


/********************************************************************
 * Main Menu Usils
 *******************************************************************/
void wait_for_enter() {
    printf("\nНажмите Enter для продолжения...");
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
    
    int choice = -1;
    int running = 1;
    
    while (running) {
        clear_screen();
        printf("===== МЕНЮ РАБОТЫ С МАССИВОМ ЧИСЕЛ (double) =====\n\n");
        printf("Текущий массив: ");
        print_real_array(arr);
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
        
        // choice написать сюда 
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        
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
                if (arr->size > 0) {
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
                    print_real_array(mapped);
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
                    print_real_array(mapped);
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
                    print_real_array(mapped);
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
                    print_real_array(filtered);
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
                    print_real_array(filtered);
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
                        print_real_array(concatenated);
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
    
    int choice;
    int running = 1;
    
    while (running) {
        clear_screen();
        printf("===== МЕНЮ РАБОТЫ С МАССИВОМ СТРОК (char*) =====\n\n");
        printf("Текущий массив: ");
        print_string_array(arr);
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
                if (arr->size > 0) {
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
                    print_string_array(mapped);
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
                    print_string_array(mapped);
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
                    print_string_array(filtered);
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
                    print_string_array(filtered);
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
                            char* str = strdup(buffer);
                            if (str) {
                                dyn_array_push_back(arr2, &str);
                            }
                        }
                    }
                    
                    DynamicArray* concatenated = dyn_array_concat(arr, arr2);
                    if (concatenated) {
                        printf("Результат конкатенации:\n");
                        print_string_array(concatenated);
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
 * Main Menu
 *******************************************************************/

int main() {
    int choice;
    int running = 1;
    
    printf("===== ДИНАМИЧЕСКИЙ МАССИВ НА СИ =====\n\n");
    
    while (running) {
        printf("\nГлавное меню:\n");
        printf("1. Работа с массивом чисел (double)\n");
        printf("2. Работа с массивом строк (char*)\n");
        printf("0. Выход из программы\n");
        printf("\nВыберите тип массива: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                double_array_menu();
                break;
                
            case 2:
                string_array_menu();
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