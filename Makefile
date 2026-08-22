TARGET = stm32f401re

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = cortex-m4

CFLAGS = -mcpu=$(CPU) \
         -mthumb \
         -mfloat-abi=soft \
         -ffreestanding \
         -fno-builtin \
         -Wall \
         -Wextra \
         -O0 \
         -g

LDFLAGS = -T linker/stm32f401re.ld \
          -mcpu=$(CPU) \
          -mthumb \
          -mfloat-abi=soft \
          -nostdlib \
          -Wl,--gc-sections \
          -Wl,-Map=build/$(TARGET).map

C_SOURCES = src/main.c

ASM_SOURCES = startup/startup_stm32f401xe.s

C_OBJECTS = build/main.o

ASM_OBJECTS = build/startup_stm32f401xe.o


all: build/$(TARGET).elf build/$(TARGET).bin

build/$(TARGET).elf: $(C_OBJECTS) $(ASM_OBJECTS)
	$(CC) $(C_OBJECTS) $(ASM_OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

build/$(TARGET).bin: build/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

build/startup_stm32f401xe.o: startup/startup_stm32f401xe.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f build/*.o
	rm -f build/*.elf
	rm -f build/*.bin
	rm -f build/*.map