#include "util.h"
#include "interrupts/idt.h"
#include <stdint.h>
#include "process.h"
//#include "paging.h"
#include "keyboard.h"
#include "gdt.h"
#include "timer.h"
#include "multiboot.h"
#include "memory.h"
#include "disk-driver.h"

char *message = "hello";

page_dir_t kernelPagedirectory;
page_table_t kernelPagetable;

/*
TODO:
event queue för kernel, kernel task? typ: open("path/to/your/file"), 



*/

void kmain(uint32_t magic, struct multiboot_info* bootInfo){

    initGdt();
    initIdt();

    initMemory(bootInfo);
    initKernelHeap();

    initTimer();
    initProcesses();
    initContextSwitching();
    init_disk_interrupt();

    disk_sector_t* test_disk_destination = kmalloc(sizeof(disk_sector_t));
    disk_sector_t* IDENTIFY_sector = kmalloc(sizeof(disk_sector_t));

    add_disk_queue(IDENTIFY_sector, 0, 'i');
    add_disk_queue(test_disk_destination, 0, 'r');
    //disk_read(test_disk_destination, 1);
    

    for(;;);
    initKeyboard();
    /*initKernelPagetables(&kernelPagetable);
    initKernelPagedirectory(&kernelPagedirectory);*/

    __asm__ volatile(
        "mov %%cr0, %%eax;"
        "or %0, %%eax;"
        "mov %%eax, %%cr0;"
        "mov %1, %%eax;"
        "mov %%eax, %%cr3;"
        :
        :"r" (0x80000000), "r" ((uint32_t)&kernelPagedirectory)
        :"%eax"
    );

    return;
}

