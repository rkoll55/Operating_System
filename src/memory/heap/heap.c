//
// Created by rkoll55 on 4/10/23.
//

#include "heap.h"
#include "kernel.h"

static int heap_validate_alignment(void* ptr) {
    return ((unsigned int) ptr % RCOS_HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table){
    return 0;
}