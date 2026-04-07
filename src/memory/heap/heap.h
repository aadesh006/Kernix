#ifndef KERNIX_HEAP_H
#define KERNIX_HEAP_H

#include <stdint.h>
#include <stddef.h>

// Bitwise flags for the Entry Table
#define HEAP_BLOCK_TABLE_ENTRY_FREE  0x00
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01

// High-bit flags to chain blocks together
#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

// Represents the metadata array
struct heap_table {
    HEAP_BLOCK_TABLE_ENTRY* entries; // Pointer to the status array
    size_t total;                    // Total number of blocks in the heap
};

// Represents the heap itself
struct heap {
    struct heap_table* table;
    void* saddr;                     // Start address of the actual usable data pool
};

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap, void* ptr);

#endif