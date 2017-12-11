RASPI_MODEL = 2

# Don't use normal gcc, use the arm cross compiler
CC = ./gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi-gcc

# Set any constants based on the raspberry pi model.  Version 1 has some differences to 2 and 3
ifeq ($(RASPI_MODEL),1)
	CPU = arm1176jzf-s
else
	CPU = cortex-a7
endif

CFLAGS= -mcpu=$(CPU) -fpic -ffreestanding
CSRCFLAGS= -O2 -Wall -Wextra
LFLAGS= -ffreestanding -O2 -nostdlib

# Location of the kernel source files
KER_SRC = ../src/kernel
KER_HEAD = ../include
OBJ_DIR = objects
CSOURCES = $(wildcard $(KER_SRC)/*.c)
ASMSOURCES = $(wildcard $(KER_SRC)/*.S)
OBJECTS = $(patsubst $(KER_SRC)/%.c, $(OBJ_DIR)/%.o, $(CSOURCES))
OBJECTS += $(patsubst $(KER_SRC)/%.S, $(OBJ_DIR)/%.o, $(ASMSOURCES))
HEADERS = $(wildcard $(KER_HEAD)/*.h)

IMG_NAME=kernel.img


build: $(OBJECTS) $(HEADERS)
	echo $(OBJECTS)
	$(CC) -T linker.ld -o $(IMG_NAME) $(LFLAGS) $(OBJECTS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -I$(KER_HEAD) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KER_SRC)/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(KER_SRC) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)
	rm $(IMG_NAME)

run: build
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel kernel.img
