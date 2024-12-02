#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H
typedef char disk_sector_t[512];

typedef struct{
    uint32_t pid;
    char type;
    uint32_t LBA;
    disk_sector_t* memory_adress;
}disk_task_t;

void init_disk_interrupt();
int add_disk_queue(disk_sector_t* memory_adress, uint32_t LBA, char type);
#endif