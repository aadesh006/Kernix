#include "pmm.h"
#include "../memory.h"
static uint8_t frame_bitmap[PMM_MAX_FRAMES / 8];
static uint32_t total_frames;

void pmm_init(uint32_t mem_size_bytes) {
    total_frames = mem_size_bytes / PMM_FRAME_SIZE;
    memset(frame_bitmap, 0, sizeof(frame_bitmap));
    // mark frames used by kernel + heap region as taken up front
}

void* pmm_alloc_frame(void) {
    for (uint32_t i = 0; i < total_frames; i++) {
        if (!(frame_bitmap[i / 8] & (1 << (i % 8)))) {
            frame_bitmap[i / 8] |= (1 << (i % 8));
            return (void*)(i * PMM_FRAME_SIZE);
        }
    }
    return NULL;
}

void pmm_free_frame(void* frame) {
    uint32_t i = (uint32_t)frame / PMM_FRAME_SIZE;
    frame_bitmap[i / 8] &= ~(1 << (i % 8));
}