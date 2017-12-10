
run: build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel myos.elf

build:
	./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o
	./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra
	./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
	./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-objcopy myos.elf -O binary myos.bin

clean:
	rm *.o
	rm *.bin
	rm *.elf
