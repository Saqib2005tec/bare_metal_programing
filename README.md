STM32F401RE Bare-Metal + Renode

A register-level bare-metal STM32F401RE project running on the Renode emulator.

The goal of this project is to understand STM32 development by working directly with the reference manual, datasheet, linker script, startup assembly, memory map, and peripheral registers instead of using STM32 HAL.

Features

STM32F401RE / Cortex-M4

Bare-metal C

ARM GNU Toolchain

Custom linker script

Custom Cortex-M4 startup assembly

Register-level peripheral configuration

RCC clock configuration

GPIO configuration

GPIO input and output configuration

USART2 configuration

PA2 → AF7 → USART2_TX

115200 baud UART

SysTick-based delay

EXTI5 external interrupt

SYSCFG EXTI line configuration

Rising-edge interrupt detection

NVIC interrupt configuration

Custom interrupt vector table

EXTI9_5_IRQHandler

PA6 output toggling from the interrupt handler

UART output from the interrupt handler

Renode simulation

Make-based build system

Requirements

Linux environment with:

ARM GNU Embedded Toolchain

Renode

Make

GCC

Git

Check the ARM compiler:

arm-none-eabi-gcc --version

Check Renode:

renode --version

Clone the Repository

git clone https://github.com/Saqib2005tec/bare_metal_programing
cd bare_metal_programing/stm32/stm32_datast

Project Structure

stm32_datast/
│
├── build/
│   ├── main.o
│   ├── startup_stm32f401xe.o
│   ├── stm32f401re.elf
│   ├── stm32f401re.bin
│   └── stm32f401re.map
│
├── linker/
│   └── stm32f401re.ld
│
├── renode/
│   └── stm32f401re.resc
│
├── src/
│   ├── main.c
│   └── stm32f401re.h
│
├── startup/
│   └── startup_stm32f401xe.s
│
└── Makefile

Build

Clean the previous build:

make clean

Build the project:

make

The resulting files will be generated in:

build/

The main output is:

build/stm32f401re.elf

The build also produces:

build/stm32f401re.bin
build/stm32f401re.map

The ELF file is the image loaded into Renode.

Run on Renode

Start Renode:

renode --console

Then inside the Renode monitor:

start @renode/stm32f401re.resc

The STM32F401RE firmware will be loaded into the simulated MCU.

The project can then be interacted with through the simulated GPIO and UART peripherals.

UART Configuration

USART2 is configured directly through its registers.

The UART configuration is:

Baud rate  : 115200
Data bits  : 8
Stop bits  : 1
Parity     : None
TX         : PA2
AF         : AF7

The configuration path is:

RCC
 │
 ├── AHB1
 │     └── GPIOA clock
 │
 └── APB1
       └── USART2 clock

GPIOA
 │
 └── PA2
      │
      └── Alternate Function 7
             │
             └── USART2_TX

Register-Level Configuration

RCC

Important registers:

RCC_AHB1ENR
RCC_APB1ENR

RCC_AHB1ENR is used to enable the GPIOA peripheral clock.

RCC_APB1ENR is used to enable the USART2 peripheral clock.

GPIOA

Important registers:

GPIOA_MODER
GPIOA_OTYPER
GPIOA_OSPEEDR
GPIOA_PUPDR
GPIOA_IDR
GPIOA_ODR
GPIOA_AFRL

Examples of configuration performed in the project:

PA2 → Alternate Function
PA2 → AF7 → USART2_TX

PA5 → Input
PA5 → Pull-down

PA6 → Output

USART2

Important registers:

USART2_SR
USART2_DR
USART2_BRR
USART2_CR1
USART2_CR2

UART transmission is implemented without using a driver library.

The basic transmission process is:

Check USART2_SR
      ↓
Wait for TXE
      ↓
Write character to USART2_DR
      ↓
USART2 transmits character

A simple string transmission function is implemented using this register-level UART routine.

SysTick

The project uses the Cortex-M4 SysTick peripheral to implement a millisecond delay.

Important registers:

SYSTICK_CTRL
SYSTICK_LOAD
SYSTICK_VAL

The delay flow is:

Set reload value
      ↓
Clear current counter
      ↓
Enable SysTick
      ↓
Wait for COUNTFLAG
      ↓
Repeat for required milliseconds

The current implementation uses the simulated 16 MHz clock to generate approximately 1 ms SysTick periods.

GPIO Input

PA5 is configured as a GPIO input with a pull-down resistor.

Configuration:

GPIOA_MODER
    ↓
00 → Input mode

GPIOA_PUPDR
    ↓
10 → Pull-down

The input state can be read through:

GPIOA_IDR

For PA5:

uint32_t state = GPIOA_IDR & (1U << 5);

This checks whether GPIOA pin 5 is currently high or low.

External Interrupts

The project also implements an external interrupt using EXTI5.

The interrupt path is:

PA5
 │
 ↓
SYSCFG
 │
 └── EXTI5
       │
       ↓
     EXTI
       │
       ├── IMR
       ├── RTSR
       └── PR
       │
       ↓
      NVIC
       │
       ↓
   IRQ 23
       │
       ↓
EXTI9_5_IRQHandler

SYSCFG

The SYSCFG peripheral is used to connect GPIOA pin 5 to EXTI line 5.

Conceptually:

GPIOA Pin 5
     ↓
  EXTI Line 5

The EXTI line number is based on the GPIO pin number.

Therefore:

PA5 → EXTI5
PB5 → EXTI5
PC5 → EXTI5
...

Only one GPIO port can be mapped to a particular EXTI line at a time.

EXTI

Important registers:

EXTI_IMR
EXTI_RTSR
EXTI_FTSR
EXTI_PR

The current implementation uses:

EXTI_IMR

to unmask EXTI5.

EXTI_RTSR

to enable rising-edge detection.

EXTI_PR

to identify and clear the pending interrupt.

The interrupt configuration follows:

PA5 input
   ↓
EXTI5
   ↓
Unmask EXTI5
   ↓
Enable rising edge
   ↓
NVIC IRQ 23

NVIC Configuration

The Cortex-M4 NVIC is responsible for enabling and dispatching interrupts to the processor.

For STM32F401RE:

EXTI9_5
   ↓
IRQ number = 23

The interrupt is enabled through:

NVIC_ISER0

using:

NVIC_ISER0 |= (1U << 23);

This enables interrupt request number 23 in the NVIC.

Interrupt Vector Table

A custom vector table is implemented in the startup assembly file.

The vector table begins at:

0x08000000

The first entries contain:

Initial Stack Pointer
Reset Handler
NMI Handler
HardFault Handler
MemManage Handler
BusFault Handler
UsageFault Handler
...

After the Cortex-M system exception vectors, the external interrupt vectors are stored sequentially.

For EXTI9_5:

IRQ number = 23
Vector index = 16 + 23
              = 39

Each vector entry is 4 bytes:

39 × 4 = 156 = 0x9C

Therefore the EXTI9_5 vector is located at:

0x08000000 + 0x9C
= 0x0800009C

The vector table contains:

.word EXTI9_5_IRQHandler

at that position.

This connects the hardware interrupt to the C interrupt handler.

Interrupt Handler

The interrupt handler is:

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_PR & (1U << 5))
    {
        EXTI_PR = (1U << 5);

        GPIOA_ODR ^= (1U << 6);

        uart_print("EXTI5 Interrupt Triggered\r\n");
    }
}

The sequence is:

PA5 rising edge
      ↓
EXTI5 detects edge
      ↓
EXTI pending bit is set
      ↓
NVIC IRQ 23
      ↓
CPU looks up vector table
      ↓
EXTI9_5_IRQHandler()
      ↓
Check EXTI_PR bit 5
      ↓
Clear pending flag
      ↓
Toggle PA6
      ↓
Transmit message through USART2

This provides a complete example of how a peripheral event travels through the STM32 interrupt system and eventually reaches software.

Renode GPIO Testing

The GPIO peripheral can be inspected from the Renode monitor.

List GPIO connections:

gpioPortA GetGPIOs

For example, PA5 can be driven from Renode:

gpioPortA OnGPIO 5 true

and:

gpioPortA OnGPIO 5 false

This simulates a change on GPIOA pin 5.

When the correct interrupt configuration is active, a rising transition on PA5 can trigger:

EXTI9_5_IRQHandler

The handler then toggles PA6 and sends:

EXTI5 Interrupt Triggered

through USART2.

This allows the complete interrupt flow to be tested without physical STM32 hardware.

Debugging and Verification

The project uses Renode monitor commands and ARM GNU binutils to verify the firmware.

For example, the address of the interrupt handler can be checked with:

arm-none-eabi-nm -n build/stm32f401re.elf | grep EXTI9_5

The vector table can be inspected with:

arm-none-eabi-objdump -s -j .isr_vector build/stm32f401re.elf

Peripheral registers can be inspected through Renode:

sysbus ReadDoubleWord 0x40020010

For example:

0x40020010

is the GPIOA input data register:

GPIOA_IDR

and:

0x40020014

is:

GPIOA_ODR

These checks help verify that the firmware is actually accessing the intended memory-mapped registers.

Linker Script

The project uses a custom linker script:

linker/stm32f401re.ld

The linker script defines the MCU memory regions and controls where sections are placed.

The important memory regions are:

FLASH
SRAM

The resulting image contains sections such as:

.isr_vector
.text
.data
.bss

The vector table is placed at the beginning of Flash so that the Cortex-M4 can use it after reset.

Startup Assembly

The startup code is implemented manually in:

startup/startup_stm32f401xe.s

It provides:

Interrupt vector table

Reset handler

Initial stack pointer

.data initialization

.bss initialization

Default interrupt handler

Weak exception handlers

The reset sequence is approximately:

Reset
  ↓
Load initial stack pointer
  ↓
Reset_Handler
  ↓
Copy .data from Flash → RAM
  ↓
Clear .bss
  ↓
Call main()

The project therefore does not depend on a vendor startup file.

Learning Approach

This project is intentionally implemented without STM32 HAL.

The configuration is derived by following this process:

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
Custom Startup
    ↓
Custom Linker Script
    ↓
ARM GCC
    ↓
ELF
    ↓
Renode
    ↓
Peripheral Event
    ↓
Interrupt Controller
    ↓
Vector Table
    ↓
ISR
    ↓
Verify Result

The main purpose is to understand the relationship between:

CPU
 │
 ├── Memory Map
 │
 ├── MMIO Registers
 │
 ├── Peripherals
 │
 ├── NVIC
 │
 └── Interrupt Vector Table

rather than treating the microcontroller as a black box.

Current Status

ARM toolchain setup

Renode setup

STM32F401RE Renode platform

Custom linker script

Custom startup assembly

Cortex-M4 vector table

Reset handler

.data initialization

.bss initialization

RCC configuration

GPIOA clock enable

GPIOA configuration

GPIO input

GPIO output

PA2 alternate-function configuration

AF7 → USART2_TX

USART2 clock enable

USART2 configuration

115200 baud UART

UART transmission

SysTick delay

Renode UART verification

EXTI5 configuration

SYSCFG EXTI routing

Rising-edge interrupt

NVIC configuration

IRQ 23 identification

Custom EXTI9_5 vector

EXTI9_5_IRQHandler

EXTI pending-bit handling

PA6 output toggle from ISR

UART output from ISR

USART reception

Timer peripheral

Interrupt-based SysTick

More STM32 peripherals

More Renode peripheral experiments

References

STM32F401RE Datasheet

STM32F401xC/E Reference Manual

ARM Cortex-M4 Technical Reference Manual

ARM Cortex-M4 Devices Generic User Guide

Renode documentation

License

This project is intended for educational and research purposes.