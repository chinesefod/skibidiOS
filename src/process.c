#include <stdint.h>
#include "util.h"
#include "process.h"
#include "paging.h"
#include "idt.h"
#include "timer.h"


extern void context_switch_iret(struct InterruptRegisters context);
process PCB[no_of_processes];
uint32_t current_process;
extern uint64_t ticks;

/*
1. spara regs i PCB
2. nästa process?
*/



void initProcesses(){
    current_process = 0;
    PCB[0].alive = 1;
    PCB[0].pid = 0;
    PCB[0].flags = 0;
}

void ontick(){
    ticks += 1;
    printf("tick\n");
}

void SKIBIDI(){
    printf("timer");
    return;
}

void initContextSwitching(){
    irq_install_handler(0, &context_switch);
    return;
}



void context_switch(struct InterruptRegisters* regs){
    ticks += 1;

    PCB[current_process].register_context = *regs;

    int i = current_process + 1;
    while(!PCB[i].alive){
        i = (i+1)%256;
    }

    current_process = i;

    *regs = PCB[i].register_context;

    return;
}
