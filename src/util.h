#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

void printf(char *p);
void printd(uint32_t p, char base);
void newl();
void pprint(int a, int b, char c);

void syscall(int num);

void memset(void *p, unsigned char c, uint32_t s);

void outPortB(uint16_t port, uint8_t value);
void outPortW(uint16_t port, uint16_t value);

char inPortB(uint16_t port);

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void printRegister(char* register_name);
int stringcmp(char* s1, char* s2);

#endif

