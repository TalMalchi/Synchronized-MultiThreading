#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#ifndef MALLOC_H_
#define MALLOC_H_

// 

typedef struct header_malloc_s {
    size_t size;
    bool is_free;
    struct header_malloc_s *next;
} header_malloc_t;

header_malloc_t *start;
header_malloc_t *end;
pthread_mutex_t lock;

void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);
void my_free(void *ptr);

#endif 