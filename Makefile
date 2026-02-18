FILES = ./build/multiboot.o \
        ./build/kernel.asm.o \
        ./build/kernel.o \
        ./build/idt/idt.asm.o \
        ./build/idt/idt.o \
        ./build/memory/memory.o \
        ./build/keyboard/keyboard.o \
        ./build/terminal/line.o \
        ./build/shell/shell.o

INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

# GRUB BUILD TARGET (MAIN)
all: ./bin/kernel.elf

./bin/kernel.elf: $(FILES)
	mkdir -p ./bin
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.elf -ffreestanding -O0 -nostdlib ./build/kernelfull.o

# Assembly & C build rules
./build/kernel.asm.o: ./src/kernel.asm
	mkdir -p ./build
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	mkdir -p ./build
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	mkdir -p ./build/idt
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	mkdir -p ./build/idt
	i686-elf-gcc $(INCLUDES) -I ./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o: ./src/memory/memory.c
	mkdir -p ./build/memory
	i686-elf-gcc $(INCLUDES) -I ./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/keyboard/keyboard.o: ./src/keyboard/keyboard.c
	mkdir -p ./build/keyboard
	i686-elf-gcc $(INCLUDES) -I ./src/keyboard $(FLAGS) -std=gnu99 -c ./src/keyboard/keyboard.c -o ./build/keyboard/keyboard.o

./build/terminal/line.o: ./src/terminal/line.c
	mkdir -p ./build/terminal
	i686-elf-gcc $(INCLUDES) -I ./src/terminal $(FLAGS) -c $< -o $@

./build/shell/shell.o: ./src/shell/shell.c
	mkdir -p ./build/shell
	i686-elf-gcc $(INCLUDES) -I ./src/shell $(FLAGS) -c $< -o $@

./build/multiboot.o: ./src/boot/multiboot.asm
	mkdir -p ./build
	nasm -f elf ./src/boot/multiboot.asm -o ./build/multiboot.o

clean:
	rm -rf ./bin/*
	rm -rf ./build/*