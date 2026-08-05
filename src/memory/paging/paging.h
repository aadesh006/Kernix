#define PAGING_PRESENT   0x1
#define PAGING_WRITABLE  0x2
#define PAGING_ENTRIES   1024
#include <stdint.h>
#include <stddef.h>

typedef uint32_t* page_directory_t;

void paging_init(void);
void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags);
void paging_enable(void);