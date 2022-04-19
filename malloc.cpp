#include "malloc.hpp"
#include <iostream>


header_malloc_t *look_for_available_memory(size_t size)
{
    header_malloc_t *temp = start;

    while (temp != NULL) {
        if (temp->is_free && temp->size >= size)
            return (temp);
        temp = temp->next;
    }
    return (NULL);
}

header_malloc_t *update_header (header_malloc_t *header, size_t size, void *memory_block)
{
    header = (header_malloc_t*)memory_block;
    header->size = size;
    header->is_free = false;
    header->next = NULL;
    if (!start)
        start = header;
    if (end)
        end->next = header;
    end = header;
    pthread_mutex_unlock(&lock);
    return (header);
}

void *my_malloc(size_t size)
{
    void *memory_block;
    header_malloc_t *header;

    pthread_mutex_lock(&lock);
    if (!size)
        return (NULL);
    header = look_for_available_memory(size);
    if (header != NULL) {
        header->is_free = false;
        pthread_mutex_unlock(&lock);
        return (void*)(header + 1);
    }
    memory_block = sbrk((sizeof(header_malloc_t) + size));
    if (memory_block == (void*) - 1) {
        pthread_mutex_unlock(&lock);
        return (NULL);
    }
    header = update_header(header, size, memory_block);
    return (void*)(header + 1);
}