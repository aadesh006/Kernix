#include "heap.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>

// Helper to check if an address is perfectly divisible by 4096
static bool heap_validate_alignment(void* ptr) {
    return ((unsigned int)ptr % 4096) == 0;
}

// Helper to ensure the table size matches the memory space provided
static int heap_validate_table(void* ptr, void* end, struct heap_table* table) {
    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / 4096;
    
    if (table->total != total_blocks) {
        return -1; // Invalid argument
    }
    return 0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table) {
    int res = 0;

    // The heap memory MUST start and end on a 4096-byte boundary
    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end)) {
        return -1;
    }

    // Zero out the heap structure
    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heap_validate_table(ptr, end, table);
    if (res < 0) {
        return res;
    }

    // Initialize the entire entry table to FREE (0x00)
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

    return res;
}