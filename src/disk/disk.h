//
// Created by rkoll55 on 15/11/23.
//

#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int RCOS_DISK_TYPE;
#define RCOS_DISK_TYPE_REAL 0

struct disk {
    RCOS_DISK_TYPE type;
    int sector_sizel;
    struct filesystem* filesystem;
};

void disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif
