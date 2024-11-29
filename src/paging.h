#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "process.h"

typedef struct {
    uint32_t present    : 1;  // Page present in memory
    uint32_t rw         : 1;  // Read/write permission
    uint32_t user       : 1;  // User/supervisor
    uint32_t accessed   : 1;  // Accessed
    uint32_t dirty      : 1;  // Dirty
    uint32_t reserved   : 7;  // Reserved bits
    uint32_t frame      : 20; // Physical frame address (aligned to 4 KB)
}__attribute__((packed)) page_table_entry_t;  

typedef struct{
    page_table_entry_t entry[1024];
}__attribute__((packed)) page_table_t;

typedef struct {
    uint32_t present        : 1;  // Page present in memory
    uint32_t rw             : 1;  // Read/write permission
    uint32_t user           : 1;  // User/supervisor
    uint32_t cachedisable   : 1;
    uint32_t writethrough   : 1;
    uint32_t accessed       : 1;  // Accessed
    uint32_t extra2         : 1;
    uint32_t pagingsize     : 1;
    uint32_t extra1         : 4;  // Reserved bits
    uint32_t pta            : 20; // Physical frame address (aligned to 4 KB)
}__attribute__((packed)) page_dir_entry_t;  

typedef struct{
    page_dir_entry_t entry[1024];
}__attribute__((packed)) page_dir_t;

extern page_dir_t kernelPagedirectory;
extern page_table_t kernelPagetable;

void initKernelPagetables(page_table_t* p);
void initKernelPagedirectory(page_dir_t* p);

#endif