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

// Helper function to round up our requested bytes to the nearest 4096-byte block
static uint32_t heap_align_value_to_upper(uint32_t val) {
    if ((val % 4096) == 0) {
        return val;
    }
    val = (val - (val % 4096)); // Round down to nearest 4096
    val += 4096;                // Add a block to go up
    return val;
}

// Scans the entry table to find a contiguous block of free memory
static int heap_get_start_block(struct heap* heap, uint32_t total_blocks) {
    struct heap_table* table = heap->table;
    int current_block_count = 0;
    int start_block = -1;

    for (size_t i = 0; i < table->total; i++) {
        // We only care about the lowest 4 bits to check if it's FREE (ignoring IS_FIRST/HAS_NEXT)
        if ((table->entries[i] & 0x0F) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
            current_block_count = 0;
            start_block = -1;
            continue;
        }

        // We found a free block
        if (start_block == -1) {
            start_block = i;
        }
        current_block_count++;

        // If we found enough contiguous blocks, return the starting index
        if (current_block_count == total_blocks) {
            return start_block;
        }
    }

    return -ENOMEM; // Out of memory
}

// Applies our bitwise flags to the entry table once memory is allocated
static void heap_mark_blocks_taken(struct heap* heap, int start_block, int total_blocks) {
    int end_block = (start_block + total_blocks) - 1;

    for (int i = start_block; i <= end_block; i++) {
        heap->table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        
        // If it's the very first block, set the IS_FIRST flag
        if (i == start_block) {
            heap->table->entries[i] |= HEAP_BLOCK_IS_FIRST;
        }
        
        // If it's not the last block, tell the system there's more connected to it
        if (i != end_block) {
            heap->table->entries[i] |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

// The main allocation function
void* heap_malloc(struct heap* heap, size_t size) {
    if (size == 0) {
        return NULL;
    }

    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / 4096;
    
    int start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < 0) {
        return NULL; // No memory found
    }

    heap_mark_blocks_taken(heap, start_block, total_blocks);
    
    // Calculate the actual physical memory address and return it
    // physical_address = start_address + (block_index * 4096)
    return (void*)(heap->saddr + (start_block * 4096));
}

// The main deallocation function
void heap_free(struct heap* heap, void* ptr) {
    if (!ptr) {
        return;
    }

    // Work backwards: calculate the block index from the memory address
    int start_block = (int)((ptr - heap->saddr) / 4096);
    
    for (int i = start_block; i < heap->table->total; i++) {
        HEAP_BLOCK_TABLE_ENTRY entry = heap->table->entries[i];
        
        // Mark the block as free
        heap->table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        
        // If it doesn't have the HAS_NEXT flag, we've reached the end of the allocation
        if (!(entry & HEAP_BLOCK_HAS_NEXT)) {
            break;
        }
    }
}