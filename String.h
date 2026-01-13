#ifndef STRINGS_H
#define STRINGS_H

#define string_from_cstr(str) (String){ .data = str, .len = strlen(str), .capacity = strlen(str)}

typedef struct {
    char*  data;
    size_t len;
    size_t capacity;
} String;

char* string_to_cstr(Arena *arena, const String *s);
void display_string(String s);

#ifdef STRING_IMPLEMENTATION

char* string_to_cstr(Arena *arena, const String *s)
{
    char* buffer = arena_alloc(arena, (sizeof(char) * s->len) + 1);
    *(char*)mempcpy(buffer, s->data, s->len) = '\0';
    return buffer;
}

void display_string(String s)
{
    printf("(String) { \n    .data = %s, \n    .len = %zu, \n    capacity = %zu\n }\n",
            s.data, s.len, s.capacity);
}
#endif

#endif //STRINGS_H
