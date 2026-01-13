//based on TSODING's arena implementation
#ifndef ARENA_H
#define ARENA_H
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_DEFAULT_SIZE (1024*8)

typedef struct Region Region;

struct Region {
    Region* next;
    size_t capacity;
    size_t count;
    uintptr_t data[];
};

typedef struct {
    Region *start, *end;
} Arena;

void* arena_alloc(Arena *arena, size_t size);
void  arena_free(Arena *arena);
void  arena_reset(Arena *arena);

#ifdef ARENA_IMPLEMENTATION
Region* new_region(size_t size)
{
    Region* region = malloc(sizeof(Region) + sizeof(uintptr_t) * size);
    assert(region);
    region->capacity = size;
    region->count = 0;
    region->next = NULL;
    return region;
}

void free_region(Region *r)
{
    free(r);
}

void* arena_alloc(Arena *arena, size_t size_r)
{
    size_t size = (size_r + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    if(arena->end == NULL)
    {
       assert(arena->start == NULL);
       size_t capacity = ARENA_DEFAULT_SIZE;
       if(capacity < size) capacity = size;
       arena->end = new_region(capacity);
       arena->start = arena->end;
    }
    while(arena->end->count + size > arena->end->capacity && arena->end->next != NULL)
    {
        arena->end = arena->end->next;
    }
    if(arena->end->count + size > arena->end->capacity)
    {
        assert(arena->end->next == NULL);
        arena->end->next = new_region(size);
        arena->end = arena->end->next;
    }
    void* data = &arena->end->data[arena->end->count];
    arena->end->count += size;
    return data;
}

void arena_reset(Arena *arena)
{
    Region *current = arena->start;
    while(current != NULL)
    {
        current->count = 0;
        current = current->next;
    }
    arena->end = arena->start;
}

void arena_free(Arena *arena)
{
    Region *p;
    while(arena->start)
    {
        p = arena->start;
        arena->start = arena->start->next;
        free_region(p);
    }
    arena->start = NULL;
    arena->end = NULL;
}
#endif //ARENA_IMPLEMENTATION
#endif //ARENA_H
