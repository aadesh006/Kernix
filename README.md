# Kernix: A 32-bit Operating System

A custom 32-bit operating system built from scratch in C and x86 assembly, featuring a bootloader, protected mode kernel, interrupt handling, paging-based memory management, a heap allocator, and a basic shell interface.

Current Status: Memory management core complete (PMM, paging, heap) — GRUB/Multiboot2 integration not yet started

## Features

- **Custom Bootloader** - Written in x86 assembly with real mode to protected mode transition
- **32-bit Protected Mode Kernel** - Proper GDT (Global Descriptor Table) setup
- **Interrupt Handling** - IDT (Interrupt Descriptor Table) supporting 512 interrupt vectors
- **Programmable Interrupt Controller (PIC)** - Hardware interrupt management
- **VGA Text Mode** - 80x25 character display with color support
- **Keyboard Driver** - PS/2 keyboard input with circular buffer
- **Basic Shell** - Command-line interface with built-in commands
- **Physical Memory Manager (PMM)** - Bitmap-based frame allocator tracking physical memory in 4KB frames
- **Paging / Virtual Memory Manager (VMM)** - Identity-mapped page directory and page tables, enabled via CR3/CR0
- **Page Fault Handling** - ISR 14 reads the faulting address from CR2 and reports it before halting
- **Heap Allocator** - Block-based allocator (4KB blocks) with a status table supporting `malloc`/`free`-style allocation, layered on top of the PMM/paging setup
- **Modular Architecture** - Clean separation of kernel subsystems

## Project Structure

```
Kernix/
├── bin/                      # Compiled binaries (os.bin)
├── build/                    # Intermediate object files
├── src/
│   ├── boot/
│   │   └── boot.asm         # Bootloader (real → protected mode)
│   ├── idt/
│   │   ├── idt.asm          # IDT assembly routines (incl. isr14 page fault stub)
│   │   ├── idt.c            # IDT implementation, ISR/IRQ handlers
│   │   └── idt.h            # IDT interface
│   ├── io/
│   │   └── io.h             # Port I/O operations (outb/inb)
│   ├── isr/
│   │   └── isr.asm          # Interrupt service routines
│   ├── keyboard/
│   │   ├── keyboard.c       # Keyboard driver implementation
│   │   ├── keyboard.h       # Keyboard interface
│   │   └── scancode.h       # Scancode to ASCII mapping
│   ├── memory/
│   │   ├── memory.c         # Low-level memory operations (memset, strncmp)
│   │   ├── memory.h         # Memory interface
│   │   ├── pmm/
│   │   │   ├── pmm.c        # Bitmap physical frame allocator
│   │   │   └── pmm.h        # PMM interface
│   │   ├── paging/
│   │   │   ├── paging.c     # Page directory/table setup, CR3/CR0 paging enable
│   │   │   └── paging.h     # Paging interface
│   │   └── heap/
│   │       ├── heap.c       # Block-based heap allocator
│   │       └── heap.h       # Heap interface
│   ├── shell/
│   │   ├── shell.c          # Command shell implementation
│   │   └── shell.h          # Shell interface
│   ├── terminal/
│   │   ├── line.c           # Line editing and buffering
│   │   ├── line.h           # Line editor interface
│   │   ├── terminal.c       # Terminal output driver
│   │   ├── terminal.h       # Terminal interface
│   │   └── vga.h            # VGA color definitions
│   ├── kernel.asm           # Kernel entry point (assembly)
│   ├── kernel.c             # Main kernel code
│   ├── kernel.h             # Kernel interface
│   ├── config.h             # System configuration
│   └── linker.ld            # Linker script
├── build.sh                  # Build automation script
├── Makefile                  # Build configuration
├── .gdbinit                  # GDB debugging setup
└── README.md
```

## Prerequisites

### Required Tools

- **Cross-Compiler**: `i686-elf-gcc` and `i686-elf-ld`
- **Assembler**: NASM (Netwide Assembler)
- **Emulator**: QEMU (`qemu-system-i386`)
- **Build Tool**: GNU Make
- **Debugger**: GDB (optional, for debugging)

### Setting Up the Cross-Compiler

The project requires an i686-elf cross-compiler. Follow the [OSDev GCC Cross-Compiler Guide](https://wiki.osdev.org/GCC_Cross-Compiler) to build it.

**Expected installation path**: `$HOME/opt/cross/bin/`

#### Quick Installation (Ubuntu/Debian)

```bash
# Install NASM and QEMU
sudo apt update
sudo apt install nasm qemu-system-x86 make gdb

# Build cross-compiler (this takes time!)
# Follow: https://wiki.osdev.org/GCC_Cross-Compiler
```

## Building

### Clone the Repository

```bash
git clone https://github.com/aadesh006/Kernix.git
cd Kernix
```

### Build the OS

```bash
# Make build script executable (first time only)
chmod +x build.sh

# Build
./build.sh
```

**Or use Make directly:**

```bash
make clean  # Clean previous builds
make all    # Build OS
```

### Build Output

The build process creates:
- `./bin/boot.bin` - Bootloader (512 bytes)
- `./bin/kernel.bin` - Kernel binary
- `./bin/os.bin` - Complete bootable OS image

## Running

### Using QEMU

```bash
qemu-system-i386 -hda ./bin/os.bin
```

### Debugging with GDB

The project includes a pre-configured `.gdbinit` file:

```bash
# Terminal 1: Start QEMU with GDB server
qemu-system-i386 -hda ./bin/os.bin -S -gdb tcp::1234

# Terminal 2: Connect GDB
gdb
```

**GDB commands:**
```gdb
(gdb) break kernel_main
(gdb) continue
(gdb) layout asm
(gdb) step
```

## Technical Details

### Boot Process

1. **BIOS loads bootloader** at `0x7C00` (512-byte boot sector)
2. **Bootloader configures GDT** with code and data segments
3. **Switch to 32-bit protected mode**
4. **Load kernel from disk** using ATA LBA read (100 sectors)
5. **Jump to kernel entry point** at `0x100000`
6. **IDT initialized**, PIC remapped, interrupts enabled
7. **Paging initialized and enabled** (identity-mapped region, CR3 loaded, PG bit set in CR0)
8. **Heap created** on top of the mapped region
9. **Keyboard driver and shell** started; boot-to-shell time reported via PIT tick count

### Memory Layout

| Address | Purpose |
|---------|---------|
| `0x7C00` | Bootloader location |
| `0x100000` (1MB) | Kernel load address |
| `0x200000` | Stack pointer |
| `0x400000`–`0x800000` | Heap region (block-based allocator, 4KB blocks) |
| `0xB8000` | VGA text mode video memory |

### Memory Management

- **PMM** (`memory/pmm/`): tracks physical frames with a bitmap, one bit per 4KB frame.
- **Paging** (`memory/paging/`): one page directory + page table(s), currently identity-mapping a fixed low-memory region. `CR3` is loaded with the page directory's physical address and the PG bit is set in `CR0` to enable paging.
- **Heap** (`memory/heap/`): a block-based allocator (4KB granularity) with a status-entry table (`FREE`/`TAKEN`, `IS_FIRST`/`HAS_NEXT` flags for chaining multi-block allocations). Exposed via `heap_malloc`/`heap_free`, initialized once in `kernel_main` and reachable elsewhere in the kernel via `kernel_get_heap()`.
- **Page Fault Handling**: ISR 14 is wired up (`idt.c`/`idt.asm`) to read the faulting address from `CR2` and report it before halting — no demand paging or recovery yet, this is diagnostic only.

Known limitation: paging currently identity-maps a fixed region sized to cover the kernel and heap; there is no dynamic mapping, no demand paging, and no per-process address space yet.

### Interrupt Handling

The IDT supports 512 interrupt vectors. Key interrupts currently wired up:

- **INT 0**: Divide-by-zero exception
- **INT 14**: Page fault (reads `CR2`, reports faulting address, halts)
- **INT 32**: Timer interrupt (IRQ0) — also used to report approximate boot-to-shell time
- **INT 33**: Keyboard interrupt (IRQ1)

### Shell Commands

The built-in shell supports:

| Command | Description |
|---------|-------------|
| `help` | Display available commands |
| `clear` | Clear the terminal screen |
| `echo` | Echo test command |
| `version` | Print kernel version info |
| `memtest` | Allocate and free memory via the heap allocator to confirm it's live |
| `pagetest` | Deliberately access an unmapped address to trigger and demonstrate the page fault handler |

## Current Status

### Implemented

- ✓ Bootloader with protected mode transition
- ✓ GDT and IDT initialization
- ✓ VGA text mode terminal
- ✓ Programmable Interrupt Controller (PIC)
- ✓ Keyboard driver with circular buffer
- ✓ Basic shell with command parsing
- ✓ Line editing (backspace support)
- ✓ Low-level memory operations (memset, strncmp)
- ✓ Bitmap physical memory manager (PMM)
- ✓ Paging / virtual memory manager (identity-mapped, CR3/CR0 enabled)
- ✓ Page fault handler (ISR 14, reads CR2)
- ✓ Heap allocator (block-based, malloc/free)
- ✓ Approximate boot-time reporting (PIT tick-based)

### In Progress / Planned

- [ ] GRUB2 / Multiboot2 boot integration
- [ ] File system support (FAT32)
- [ ] Multitasking and process scheduling
- [ ] System call interface
- [ ] Advanced keyboard features (shift, caps lock)
- [ ] Disk I/O operations
- [ ] Dynamic/on-demand paging, per-process address spaces
- [ ] Cycle-accurate (rdtsc-based) interrupt latency measurement

## Development

### Adding New Features

1. **Create source files** in appropriate `src/` subdirectory
2. **Update Makefile** - Add object file to `FILES` variable
3. **Add build rules** following existing patterns
4. **Include headers** in relevant files
5. **Test thoroughly** with QEMU

### Code Style Guidelines

- **Assembly**: NASM syntax, Intel format
- **C**: GNU99 standard, freestanding environment
- **Compiled with**: `-ffreestanding` (no standard library)
- **Naming**: Lowercase with underscores (`snake_case`)

### Example: Adding a New Module

```makefile
# In Makefile, add to FILES:
FILES = ... \
        ./build/mymodule/mymodule.o

# Add build rule:
./build/mymodule/mymodule.o: ./src/mymodule/mymodule.c
	mkdir -p ./build/mymodule
	i686-elf-gcc $(INCLUDES) -I ./src/mymodule $(FLAGS) -c $< -o $@
```

## Testing

### Manual Testing

```bash
# Build and run
./build.sh
qemu-system-i386 -hda ./bin/os.bin

# Test keyboard input
# Type commands in the QEMU window

# Test the heap allocator
memtest

# Test the page fault handler
pagetest

# Test interrupts
# Trigger divide-by-zero in kernel code
```

## Resources

### Operating System Development

- [OSDev.org](https://wiki.osdev.org/) - Comprehensive OS development wiki
- [OSDev Forums](https://forum.osdev.org/) - Community support
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) - Complete processor reference

### Tools Documentation

- [NASM Documentation](https://www.nasm.us/docs.php) - Assembly reference
- [GCC Documentation](https://gcc.gnu.org/onlinedocs/) - Compiler manual
- [QEMU Documentation](https://www.qemu.org/docs/master/) - Emulator guide

## Author

**Aadesh Chaudhari**
GitHub: [@aadesh006](https://github.com/aadesh006)

## Acknowledgments

- The OSDev.org community for extensive documentation
- QEMU developers for an excellent testing platform
- The x86 architecture designers for creating such a well-documented platform

---

**Note**: This is an educational project created for learning operating system development. It is not intended for production use.

---

⭐ **Star this repository** if you find it helpful!
