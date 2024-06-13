//
// Created by rkoll55 on 26/09/23.
//
#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08;
#define KERNEL_DATA_SELECTOR 0x10;
#define RCOS_TOTAL_INTERRUPTS 512
#define RCOS_HEAP_SIZE_BYTES 104857600 //100MB
#define RCOS_HEAP_BLOCK_SIZE 4096
#define RCOS_HEAP_ADDRESS 0x01000000
#define RCOS_HEAP_TABLE_ADDRESS 0x00007E00
#define RCOS_SECTOR_SIZE 512
#define RCOS_MAX_FILESYSTEMS 10
#define RCOS_MAX_FILE_DESCRIPTORS 1024
#endif