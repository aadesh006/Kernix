#ifndef KERNIX_PMM_H
#define KERNIX_PMM_H

#include <stdint.h>
#include <stddef.h>

#define PMM_FRAME_SIZE 4096
#define PMM_MAX_FRAMES 32768

void pmm_init(uint32_t mem_size_bytes);
void* pmm_alloc_frame(void);
void pmm_free_frame(void* frame);

#endif