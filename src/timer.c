#include <stdint.h>
#include "util.h"
#include "idt.h"
#include "timer.h"
#include "process.h"

const uint32_t freq = 1000;
uint64_t ticks;

void initTimer(){
    ticks = 0;
    
    uint32_t divisor = 1193180/freq;

    //0011 0110
    outPortB(0x43, 0x36);
    outPortB(0x40, (uint8_t)(divisor & 0xff));
    outPortB(0x40, (uint8_t)((divisor >> 8) & 0xff));

    return;
}