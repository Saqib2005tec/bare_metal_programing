# STM32F401RE Bare-Metal + Renode

A register-level bare-metal STM32F401RE project running on the Renode emulator.

The goal of this project is to understand STM32 development by working directly with the reference manual, datasheet, linker script, startup assembly, memory map, and peripheral registers instead of using STM32 HAL.

## Features

- STM32F401RE / Cortex-M4
- Bare-metal C
- ARM GNU Toolchain
- Custom linker script
- Custom Cortex-M4 startup assembly
- Register-level peripheral configuration
- RCC clock configuration
- GPIO configuration
- USART2 configuration
- PA2 → AF7 → USART2_TX
- 115200 baud UART
- SysTick-based delay
- Renode simulation
- Make-based build system


## Requirements

Linux environment with:

- ARM GNU Embedded Toolchain
- Renode
- Make
- GCC
- Git

Check the ARM compiler:

```bash
arm-none-eabi-gcc --version
```

Check Renode:

```bash
renode --version
```

## Clone the Repository

```bash
git clone https://github.com/Saqib2005tec/bare_metal_programing
cd stm32_datast
```

## Build

Clean the previous build:

```bash
make clean
```

Build the project:

```bash
make
```

The resulting files will be generated in:

```text
build/
```

The important output is:

```text
build/stm32f401re.elf
```

## Run on Renode

Start Renode:

```bash
renode --console
```

Then inside the Renode monitor:

```text
start @renode/stm32f401re.resc
```

The STM32F401RE firmware will be loaded into the simulated MCU.

The UART output should appear in the Renode terminal.

Expected output:

```text
Hello World
Hello World
Hello World
...
```

with approximately one second between messages.

## Hardware Configuration

The project configures:

```text
STM32F401RE
     │
     ├── RCC
     │
     ├── GPIOA
     │    └── PA2
     │          └── AF7
     │                └── USART2_TX
     │
     └── USART2
          ├── 115200 baud
          ├── 8 data bits
          └── 1 stop bit
```

## Register-Level Configuration

Important registers used by the project:

### RCC

```text
RCC_AHB1ENR
RCC_APB1ENR
```

### GPIOA

```text
GPIOA_MODER
GPIOA_AFRL
```

### USART2

```text
USART2_SR
USART2_DR
USART2_BRR
USART2_CR1
USART2_CR2
```

### SysTick

```text
SYSTICK_CTRL
SYSTICK_LOAD
SYSTICK_VAL
```

## Learning Approach

This project is intentionally implemented without STM32 HAL.

The configuration was derived by following this process:

```text
Datasheet
    ↓
Reference Manual
    ↓
Memory Map
    ↓
Peripheral Base Address
    ↓
Register Offset
    ↓
Bit / Field Definition
    ↓
Register-Level C
    ↓
Compile
    ↓
ELF
    ↓
Renode
    ↓
Verify UART Output
```

This makes the project useful for learning how MCU peripherals are actually configured at the hardware-register level.

## Current Status

- [x] ARM toolchain setup
- [x] Renode setup
- [x] STM32F401RE Renode platform
- [x] Custom linker script
- [x] Startup assembly
- [x] RCC configuration
- [x] GPIOA configuration
- [x] USART2 configuration
- [x] UART transmission
- [x] SysTick delay
- [x] `"Hello World"` every ~1 second
- [ ] USART reception
- [ ] GPIO input
- [ ] Timer peripheral
- [ ] Interrupt-based SysTick
- [ ] NVIC configuration
- [ ] More STM32 peripherals

## References

- STM32F401RE Datasheet
- STM32F401xC/E Reference Manual
- ARM Cortex-M4 Technical Reference Manual
- Renode documentation

## License

This project is intended for educational and research purposes.