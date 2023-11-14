#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour) {
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour) {
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

//keeps track of the position we are writing to
void terminal_writechar(char c, char colour) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row += 1;
        return;
    }
    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize() {
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

// for null terminated strings only
size_t strlen(const char* str) {
    size_t count = 0;
    while (str[count]) {
        count++;
    }
    return count;
}

void newLine() {
    terminal_row++;
    terminal_col = 0;
}

void print(const char* word) {
    size_t length = strlen(word);
    for (int i =0; i < length; i++) {
        terminal_writechar(word[i], WHITE);
    }
}

extern void problem();

static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main() {

    terminal_initialize();
    print("Hello World!\n");
    print("I just made a terminal");

    //init heap
    kheap_init();

    // init interrupt descriptor table
    idt_init();

    //enable paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    // switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
	
	char* ptr = kzalloc(4096);
	paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*) 0x1000, (uint32_t) ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITABLE);

    // enable paging
    enable_paging();

    // enable interrupts
    enable_interrupts();
}
