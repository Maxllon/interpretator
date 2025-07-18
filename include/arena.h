#ifndef ARENA_H
#define ARENA_H

;typedef struct Arena Arena;
;typedef struct Region Region;

struct Region
{
    size_t size;
    char* data;
    char* head;
    size_t used;
    Region* next;
};
struct Arena
{
    Region* begin;
    Region* end;
};

Region* new_region(size_t size);
Arena* arena_create();
void* arena_alloc(size_t size);
void* arena_realloc(void* loc, size_t size);
void arena_destroy();


#endif