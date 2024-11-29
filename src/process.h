#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "util.h"
#include "paging.h"

#define no_of_processes 256

typedef struct{
    struct InterruptRegisters register_context;
    uint8_t flags;
    uint32_t pid;
    uint8_t alive;
}process;

extern process PCB[no_of_processes];
extern uint32_t current_process;

void context_switch(struct InterruptRegisters* regs);
void initProcesses();
void initContextSwitching();

void ontick();


#endif