//
// Created by rkoll55 on 20/09/23.
//
#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"

struct idt_desc idt_descriptor[RCOS_TOTAL_INTERRUPTS];
struct idtr_desc idr_descriptor;

extern void idt_load(struct idtr_desc* ptr);

void idt_zero() {
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptor[interrupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t) address >> 16;
}
void idt_init() {
    memset(idt_descriptor, 0, sizeof(idt_descriptor));
    idr_descriptor.limit = sizeof(idt_descriptor) -1;
    idr_descriptor.base = (uint32_t) idt_descriptor;

    idt_set(0, idt_zero);

    idt_load(&idr_descriptor);
}