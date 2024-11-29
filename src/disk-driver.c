#include "idt.h"
#include "util.h"
#include "idt.h"
#include "disk-driver.h"
#include "mutex.h"

uint32_t disk_read_LBA;
disk_sector_t* disk_read_destination;
disk_task_t current_task;
char last_disk_command;

char disk_mutex = 0;

void delay_400ns(){
    char status;
    for(int i = 0; i < 15; i++) status = inPortB(0x1F7);
    return;
}

void disk_read(disk_sector_t* destination, uint32_t LBA){

    current_task.type = 'r';
    current_task.LBA = LBA;
    current_task.memory_adress = destination;

    outPortB(0x1F6, 0xE0 | ((LBA >> 24) & 0x0F));

    delay_400ns();

    outPortB(0x1F2, 1);
    outPortB(0x1F3, (unsigned char)LBA);
    outPortB(0x1F4, (unsigned char)(LBA >> 8));
    outPortB(0x1F5, (unsigned char)(LBA >> 16));

    outPortB(0x1F7, 0x20);

    return;
}

void disk_identify(disk_sector_t* destination){

    current_task.type = 'i';
    current_task.memory_adress = destination;

    outPortB(0x1F6, 0xA0);//select master drive

    delay_400ns();

    outPortB(0x1F2, 0); //Sectorcount, LBAlo, LBAmid, LBAhi IO ports = 0
    outPortB(0x1F3, 0);
    outPortB(0x1F4, 0);
    outPortB(0x1F5, 0);

    outPortB(0x1F7, 0xEC);

    return;
}

void disk_finished_read(disk_task_t disk_task){
    memset(disk_task.memory_adress, 0, sizeof(uint32_t));   
    asm volatile(   
        "mov %0, %%esi;"
        "mov $256, %%ecx;"
        "mov $0x1F0, %%edx;"
        "rep insw;"
        : 
        :"r" (disk_task.memory_adress) 
        :"%esi", "%ecx", "%edx"
    );
    printf("disk read\n");
    printd(*(uint32_t*)disk_task.memory_adress, 16); newl();

    return;
}

disk_task_t disk_task_queue[1024];

/*

funktioner:

*/

void disk_interrupt_handler(){

    while(inPortB(0x1F7) & 0x80); //Poll until BSY (busy) bit clears
    char status = inPortB(0x1F7);

    if(status & 0x01) {
        printf("disk error occured\n");
        printf("error register: "); printd(inPortB(0x1F1), 2); newl();
        return;
    }

    //'i' means identify
    if (current_task.type == 'i'){
        printf("IDENTIFY disk command sent \n");
        if(status == 0){
            printf("no master drive found on primary ATA bus\n");
        }      
        else if((inPortB(0x1f4) | inPortB(0x1f5))){ //these ports are zero for ATA drives
            printf("drive is not ATA \n");
            return;
        }

        else printf("drive is ATA \n");

        while(1){//poll until DRQ or ERROR bit is set
            status = inPortB(0x1F7);
            if(status & 0x08) break; //DRQ bit set, data transfer is ready
            if(status & 0x01) break; //ERROR bit is set, FUCKING SHIT FUCK YOU?????
        }          
         
        if(status & 1) {
            printf("disk error occured, processor halted\n");
            for(;;);
        };
        if(status & 0x08){
            printf("master drive found on primary bus\n");
            printf("data transfer ready after IDENTIFY \n");    
            disk_finished_read();    
        }
    }

    //'r' means READ
    else if (current_task.type == 'r'){
        if(status & 0x08) disk_finished_read(current_task);
        else printf("som weird shit happened idk\n");
    }

    disk_task_t new_task; 

    pop_queue(&new_task, disk_task_queue, sizeof(disk_task_t));
    if(new_task.type == 'r'){
        disk_read(new_task.memory_adress, new_task.LBA);
    }
    else if(new_task == 'i'){
        disk_identify(new_task.memory_adress);
    }
    return;
}

void disk_init_interrupt(){
    outPortB(0x3F6, 0);
    irq_install_handler(14, disk_interrupt_handler);
}
