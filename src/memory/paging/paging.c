//
// Created by rkoll55 on 31/10/23.
//
#include "paging.h"
#include "memory/heap/kheap.h"

void paging_load_directory(uint32_t* directory);

static uint32_t* current_directory = 0;

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags) {
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;

    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {

        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for (int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++) {
            entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
        }
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t) entry | flags | PAGING_IS_WRITABLE;
    }
    struct paging_4gb_chunk* page_table = kzalloc(sizeof(struct paging_4gb_chunk));
    page_table->directory_entry = directory;
    return page_table;
}

void paging_switch(uint32_t* directory) {
    paging_load_directory(directory);
    current_directory = directory;
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk) {
    return chunk->directory_entry;
}