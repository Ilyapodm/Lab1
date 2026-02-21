#ifndef MAPPERS_H
#define MAPPERS_H

// Map функции для double
void real_map_multiply(void* dest, const void* src);
void real_map_square(void* dest, const void* src);
void real_map_add_one(void* dest, const void* src);

// Map функции для строк
void string_map_to_upper(void* dest, const void* src);
void string_map_to_lower(void* dest, const void* src);

// Predicate функции
int is_positive(const void* elem);
int is_negative(const void* elem);
int string_not_empty(const void* elem);
int string_starts_with_a(const void* elem);

#endif // MAPPERS_H