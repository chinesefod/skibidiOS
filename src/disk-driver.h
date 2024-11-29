#ifndef DISK_DRIVER_H
#define DISK_DRIVER_H
typedef char disk_sector_t[512];

typedef struct{
    uint32_t pid;
    char type;
    uint32_t LBA;
    disk_sector_t* memory_adress;
}disk_task_t;

void disk_read(disk_sector_t* destination, uint32_t LBA);
void disk_identify(disk_sector_t* destination);
void disk_init_interrupt();
#endif