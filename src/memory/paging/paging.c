#include "paging.h"
__attribute__((aligned(4096))) static uint32_t page_directory[PAGING_ENTRIES];
__attribute__((aligned(4096))) static uint32_t first_page_table[PAGING_ENTRIES];

void paging_init(void) {
    for (int i = 0; i < PAGING_ENTRIES; i++) {
        // identity-map first 4MB, 4KB at a time
        first_page_table[i] = (i * 0x1000) | PAGING_PRESENT | PAGING_WRITABLE;
        page_directory[i] = 0; // not present
    }
    page_directory[0] = ((uint32_t)first_page_table) | PAGING_PRESENT | PAGING_WRITABLE;
}

void paging_enable(void) {
    __asm__ volatile ("mov %0, %%cr3" :: "r"(page_directory));
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // set PG bit
    __asm__ volatile ("mov %0, %%cr0" :: "r"(cr0));
}