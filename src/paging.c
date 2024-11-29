#include <stdint.h>
#include "paging.h"
#include "util.h"

/* 
256 st pagetables som kartlägger lägre 4e-delen av minnet

varje ny process får ett pagedirectory, i detta pagedirectoryt kommer 
de 256 första entries att peka till dessa 256 första pagetables
*/

page_dir_t kernelPagedirectory;
page_table_t kernelPagetable;


void initKernelPagetables(page_table_t* p){
    
    return;
}

void initKernelPagedirectory(page_dir_t* p){

    return;
}

/*
void requestNewPage(struct pagedirectory* p){

}
*/





//nån datastruktur som håller koll på fria pages+

/*void initialize_bitmap(){
    unsigned char* i = (unsigned char*)0x91000;
    while((unsigned char*)0x91000 <= i && i <= (unsigned char*)0xb0fff){
        *i = 0;
        i += 1;
    }
}*/

void zeropagedir(page_dir_t* p){
    memset(p, 0, sizeof(page_dir_t));
    return;
}

void zeropagetable(page_table_t* p){
    memset(p, 0, sizeof(page_dir_t));
    return;
}

void zeroalltheshit(page_dir_t* p, page_table_t* q){
    for(int i; i < 1024; i++) memset(p->entry, 0, sizeof(page_dir_entry_t));
    for(int j; j<4096; j++) memset(q->entry, 0, sizeof(page_table_entry_t));
    return;
}

