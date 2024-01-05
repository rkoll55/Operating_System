//
// Created by rkoll55 on 10/09/23.
//

#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define WHITE 15
#define RCOS_MAX_PATH 108

void kernel_main();
void print(const char* word);
#endif