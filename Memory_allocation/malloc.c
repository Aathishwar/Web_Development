#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_SIZE 1048576  // 1 MB
#define BLOCK_SIZE sizeof(Block)

typedef struct Block {
    size_t size;
    int free;
    struct Block *next;
} Block;

char memory_pool[POOL_SIZE];
Block *free_list = NULL;

void init_memory_pool() {
    free_list = (Block *)memory_pool;
    free_list->size = POOL_SIZE - BLOCK_SIZE;
    free_list->free = 1;
    free_list->next = NULL;
}

void split_block(Block *block, size_t size) {
    if (block->size >= size + BLOCK_SIZE + 4) {
        Block *new_block = (Block *)((char *)(block + 1) + size);
        new_block->size = block->size - size - BLOCK_SIZE;
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;

        printf("[SPLIT] Block at %p split:\n", (void *)block);
        printf("--> Allocated block: size = %zu\n", block->size);
        printf("--> New free block at %p: size = %zu\n",(void *)new_block, new_block->size);
    }
}

void *my_malloc(size_t size) {
    Block *curr = free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {
            split_block(curr, size);
            curr->free = 0;
            return (void *)(curr + 1);
        }
        curr = curr->next;
    }

    return NULL;  
}

void merge_blocks() {
    Block *curr = free_list;

    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += BLOCK_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void my_free(void *ptr) {
    if (!ptr) return;

    Block *block = (Block *)ptr - 1;
    block->free = 1;
    merge_blocks();
}
void *my_calloc(int size,int type){
    void * new=my_malloc(size*type);
    if(new)
      memset(new,0,size*type);
    return new;
}

void *my_realloc(void *ptr, size_t new_size) {
    if (!ptr) 
        return my_malloc(new_size);

    Block *old_block = (Block *)ptr - 1;

    if (old_block->size >= new_size) 
        return ptr;

    void *new_ptr = my_malloc(new_size); // new block allocation
    if (!new_ptr) return NULL;

    memcpy(new_ptr, ptr, old_block->size); // copy old block

    my_free(ptr);

    return new_ptr;
}

void memory_status() {
    printf("\n=== Memory Blocks Status ===\n");
    Block *curr = free_list;
    int index = 1;

    while (curr) {
        printf("Block %d | Addr: %p | Size: %zu | %s\n",index++,(void *)curr,curr->size,curr->free ? "FREE" : "USED");
        curr = curr->next;
    }

    printf("============================\n\n");
}
