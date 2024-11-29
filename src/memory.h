#ifndef MEMORY_H
#define MEMORY_H

#include "multiboot.h"

void initMemory(struct multiboot_info* multibootInfo);
void initKernelHeap();
void* kmalloc(uint32_t size);

struct heap_chunk{ 
    uint32_t prev_size;
    uint32_t chunk_size: 29;
    uint32_t flags: 3; //AMP
    struct heap_chunk* prev_adr;
    struct heap_chunk* next_adr;

}__attribute__((packed));

typedef struct heap_chunk heap_chunk;



#endif