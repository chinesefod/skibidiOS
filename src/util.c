#include <stdint.h>
#include "process.h"
#include "util.h"

extern void print(int a, int b, char c);

void syscall(int num){
    __asm__ volatile(
        "mov %0, %%eax;"
        "int $0x80;"
        :           //inget output
        : "r" (num) //input
        : "%eax"    //clobbered registers
    );
    return;
}

void pprint(int a, int b, char c){
    int linear_adress = 0xB8000 + 2*sizeof(char)*(a + 80*b);
    *(char*)linear_adress = c;
    return;
}

void printf(char *p){
    char* q = p;
    static int x = 0;
    static int y = 0;
    while(*q != 0){
        if(*q == '\n'){
            x = 0;
            y += 1;
        }else if(x<79){
            pprint(x,y,*q);
            x += 1;
        }else{
            pprint(x,y,*q);
            x = 0;
            y += 1;
        }  
        q = q+1;
    }
    return;
}

void newl(){
    printf("\n");
    return;
}

void printd(uint32_t n, char base){
    char ar[35];
    ar[34] = 0;
    char* c = &ar[33];
    if(n == 0){
        *c = '0';
        printf(c);
        return;
    }
    for(int i = 0; n; i++){
        if (n%base < 10){
            *c = n%base + '0';
        }else{
            *c = n%base + 55;
        }
        n = n/base;
        c = c-1;
    }
    c = c+1;
    printf(c);
    return;  
}

int memcmp(void* p1, void* p2, uint32_t size){
    char* x1 = (char*)p1;
    char* x2 = (char*)p2;
    for(int i = 0; i < size; i++){
        if(x1[i] != x2[i]) return 1;
    }
    return 0;
}

int strcmp(char* s1, char* s2){
    int i = 0;
    while(s1[i] == s2[i] && s1[i] && s2[i]){
        i++;
    }
    if(s1[i] == 0 && s2[i] == 0){
        return 1;
    }
    return 0;
}

void printRegister(char* register_name){
    uint32_t register_value;
    if(strcmp(register_name,"eax")){

        asm volatile("mov %%eax, %0":"=r" (register_value): :);

    }else if(strcmp(register_name,"ebx")){
        
        asm volatile("mov %%ebx, %0":"=r" (register_value): :);

    }
    printd(register_value, 16); newl();
    return;
}

void memset(void *p, unsigned char c, uint32_t s){
    unsigned char* uchar_ptr = (unsigned char*)p;
    for(uint32_t i = 0; i < s; i++){
        uchar_ptr[i] = c;
    }
    return;
}

void outPortB(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
void outPortW(uint16_t port, uint16_t value){
    asm volatile ("out %1, %0" : : "dN" (port), "a" (value));
}

char inPortB(uint16_t port){
    char val;
    asm volatile ("inb %1, %0" : "=a" (val) : "dN" (port));
    return val;
}

void pop_queue(void* pop_destination, void* queue, uint32_t size);