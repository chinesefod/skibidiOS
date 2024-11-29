#include <stdint.h>
#include "multiboot.h"
#include "util.h"
#include "memory.h"

#define ALIGN 8
#define NULL 0

extern uint32_t _end;
uint32_t kheap_start;
uint32_t kheap_end = 0x6400000;


heap_chunk* free_list_start;

void initMemory(struct multiboot_info* bootInfo){
    int j = 0;
    for(int i = 0; i < bootInfo->mmap_length; i += sizeof(struct multiboot_mmap_entry)){
        struct multiboot_mmap_entry* mmmt = (struct multiboot_mmap_entry*)(i + bootInfo->mmap_addr);

        printf("Memory map region no. "); printd(j, 10); newl();
        printf("Start adress: "); printd(mmmt->addr_low ,16); 
        printf("| Length: "); printd(mmmt->len_low, 16); 
        printf("| type: "); printd(mmmt->type, 10); newl();

        j = j+1;
    }

    printf("kernel start & end location: "); printd(0x100000, 16); printf(" & "); printd(0x100000 + _end, 16); newl();
    printf("vbe: "); printd(bootInfo->vbe_mode, 16); newl();
    printf("boot_device: "); printd(bootInfo->boot_device, 16); newl();

}

void initKernelHeap(){       
    kheap_start = 0x100000 + _end;
    heap_chunk* initial_free_chunk = (heap_chunk *)kheap_start;

    initial_free_chunk->chunk_size = kheap_start - kheap_end;
    initial_free_chunk->flags = 0b001; //AMP,0 = main thread, 0 = inte memory mapped, 1 = prev_is_in_use 
    initial_free_chunk->prev_adr = NULL;
    initial_free_chunk->next_adr = NULL;

    free_list_start = initial_free_chunk;
    return;
}

void replace_tail(heap_chunk* chunk, heap_chunk* new_chunk){
    chunk->prev_adr->next_adr = new_chunk;
    return;
}

void replace_head(heap_chunk* chunk, heap_chunk* new_chunk){
    free_list_start = new_chunk;
    chunk->next_adr->prev_adr = new_chunk;
    return;
}

void replace_middle(heap_chunk* chunk, heap_chunk* new_chunk){
    chunk->next_adr->prev_adr = new_chunk;
    chunk->prev_adr->next_adr = new_chunk;
    return;
}

void remove_entry(heap_chunk* chunk){
    if(chunk->next_adr && chunk->prev_adr){
        chunk->prev_adr->next_adr = chunk->next_adr;
        chunk->next_adr->prev_adr = chunk->prev_adr;
        return;
    }
    if(chunk->next_adr && !chunk->prev_adr){
        chunk->next_adr->prev_adr = NULL;
        free_list_start = chunk->next_adr;
        return;
    }
    if(!chunk->next_adr && chunk->prev_adr){
        chunk->prev_adr->next_adr = NULL;
        return;
    }
}

void allocate_update_heap_metadata(heap_chunk* chunk, uint32_t size){

    heap_chunk* new_free_chunk = NULL;

    /*
    1. remove chunk from free list
    2. insert new_free_chunk (if its needed)
        funktioner: insert_tail, insert_head, insert_middle (efter chunk->prev_adr)
    
    */

    if(size + ALIGN + sizeof(heap_chunk) > chunk->chunk_size){
        remove_entry(chunk);
        return;
    }

    new_free_chunk = (heap_chunk*)((uint32_t)chunk + size + ALIGN);
    new_free_chunk->next_adr = chunk->next_adr;
    new_free_chunk->prev_adr = chunk->prev_adr;
    new_free_chunk->prev_size = size + ALIGN;
    new_free_chunk->flags = 0b001;

    new_free_chunk->chunk_size = chunk->chunk_size - (size + ALIGN);

    if(chunk->next_adr && chunk->prev_adr){
        replace_middle(chunk, new_free_chunk);
        chunk->next_adr->prev_size = new_free_chunk->chunk_size;
        return;
    }
    if(chunk->next_adr && !chunk->prev_adr){
        replace_head(chunk, new_free_chunk);
        chunk->next_adr->prev_size = new_free_chunk->chunk_size;
        return;
    }
    if(!chunk->next_adr && chunk->prev_adr){
        replace_tail(chunk, new_free_chunk);
        return;
    }
    if(!chunk->next_adr && !chunk->prev_adr){
        free_list_start = new_free_chunk;
        return;
    }
    
    return;

}

void* kmalloc(uint32_t size){

    heap_chunk* current_chunk = free_list_start;

    while(current_chunk != NULL){
        if(size + ALIGN <= current_chunk->chunk_size){
            
            allocate_update_heap_metadata(current_chunk, size);

            return (void*)((uint32_t)current_chunk + ALIGN);
        }
        current_chunk = current_chunk->next_adr;
    }
    return (void*)-1;

}

void free(void* user_pointer){
    heap_chunk* chunk = (heap_chunk*)(user_pointer - ALIGN);

}

/*
malloc:
    första gången malloc() används, system call för att få metadata om processen, information om memory map
    kmalloc vet redan detta dock så ingen skillnad mellan första och andra



*/

