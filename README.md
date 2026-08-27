Here is a clean, well-structured, and GitHub-friendly Markdown version of your project documentation. You can copy and paste this directly into your `README.md` file.

---

# STM32F401RE Bare-Metal + Renode

A register-level bare-metal STM32F401RE project running on the [Renode](https://renode.io/) emulator.

The goal of this project is to understand STM32 development by working directly with the reference manual, datasheet, linker script, startup assembly, memory map, and peripheral registers instead of using the STM32 HAL.

---

## ✨ Features

* **Target:** STM32F401RE / Cortex-M4
* **Language:** Bare-metal C
* **Toolchain:** ARM GNU Toolchain
* **Custom Scripts:** Custom linker script & Cortex-M4 startup assembly
* **Peripherals:**
* Register-level peripheral configuration
* RCC clock configuration
* GPIO input and output configuration
* SysTick-based delay


* **UART (USART2):**
* PA2 → AF7 → USART2_TX
* 115200 baud UART


* **Interrupts (EXTI5):**
* SYSCFG EXTI line configuration
* Rising-edge interrupt detection
* NVIC interrupt configuration
* Custom interrupt vector table & `EXTI9_5_IRQHandler`
* PA6 output toggling & UART output from the interrupt handler


* **Environment:** Renode simulation & Make-based build system

---

## 🛠️ Requirements

A Linux environment with the following dependencies:

* ARM GNU Embedded Toolchain
* Renode
* Make
* GCC
* Git

Verify your installations:

```bash
# Check the ARM compiler
arm-none-eabi-gcc --version

# Check Renode
renode --version

```

---

## 🚀 Getting Started

### Clone the Repository

```bash
git clone https://github.com/Saqib2005tec/bare_metal_programing
cd bare_metal_programing/stm32/stm32_datast

```

### Build the Project

```bash
# Clean the previous build
make clean

# Build the project
make

```

The resulting files will be generated in the `build/` directory:

* `build/stm32f401re.elf` *(Main image loaded into Renode)*
* `build/stm32f401re.bin`
* `build/stm32f401re.map`

### Run on Renode

Start the Renode console:

```bash
renode --console

```

Inside the Renode monitor, load and start the simulation:

```text
(renode) start @renode/stm32f401re.resc

```

The STM32F401RE firmware will be loaded into the simulated MCU. You can interact with it through the simulated GPIO and UART peripherals.

---

## 📂 Project Structure

```text
stm32_datast/
├── build/
│   ├── main.o
│   ├── startup_stm32f401xe.o
│   ├── stm32f401re.elf
│   ├── stm32f401re.bin
│   └── stm32f401re.map
├── linker/
│   └── stm32f401re.ld
├── renode/
│   └── stm32f401re.resc
├── src/
│   ├── main.c
│   └── stm32f401re.h
├── startup/
│   └── startup_stm32f401xe.s
└── Makefile

```

---

## ⚙️ Hardware Configuration

### UART Configuration

USART2 is configured directly through its registers without a driver library.

* **Baud rate:** 115200
* **Data bits:** 8
* **Stop bits:** 1
* **Parity:** None
* **TX Pin:** PA2 (Alternate Function 7)

**Configuration Path:**

```text
RCC
 │
 ├── AHB1 ──> GPIOA clock
 │
 └── APB1 ──> USART2 clock

GPIOA
 │
 └── PA2 ──> Alternate Function 7 ──> USART2_TX

```

**Important Registers:** `USART2_SR`, `USART2_DR`, `USART2_BRR`, `USART2_CR1`, `USART2_CR2`.

```text
Check USART2_SR
      ↓
Wait for TXE
      ↓
Write character to USART2_DR
      ↓
USART2 transmits character

```

### GPIO & SysTick Configuration

* **PA2:** Alternate Function (AF7 → USART2_TX)
* **PA5:** Input (Pull-down)
* **PA6:** Output
* **SysTick:** Cortex-M4 SysTick peripheral used to implement a millisecond delay (simulated 16 MHz clock ≈ 1 ms periods).
* *Registers:* `SYSTICK_CTRL`, `SYSTICK_LOAD`, `SYSTICK_VAL`.



---

## ⚡ Interrupts (EXTI5 & NVIC)

The project implements an external interrupt using EXTI5 on PA5.

**Interrupt Routing Path:**

```text
PA5 ──> SYSCFG ──> EXTI5 ──> EXTI (IMR, RTSR, PR) ──> NVIC (IRQ 23) ──> EXTI9_5_IRQHandler

```

### Vector Table & Handler

The custom vector table is implemented in the startup assembly file starting at `0x08000000`.
For `EXTI9_5` (IRQ number 23):

* Vector index = 16 (System Exceptions) + 23 = 39
* Byte Offset = 39 × 4 bytes = 156 (`0x9C`)
* Address = `0x0800009C`

**C Interrupt Handler (`main.c`):**

```c
void EXTI9_5_IRQHandler(void)
{
    // Check if EXTI line 5 triggered the interrupt
    if (EXTI_PR & (1U << 5))
    {
        // Clear pending flag
        EXTI_PR = (1U << 5);

        // Toggle PA6
        GPIOA_ODR ^= (1U << 6);

        // Transmit message
        uart_print("EXTI5 Interrupt Triggered\r\n");
    }
}

```

---

## 🧪 Testing & Verification

### Renode GPIO Testing

You can simulate external hardware events directly from the Renode monitor:

```text
(renode) gpioPortA GetGPIOs
(renode) gpioPortA OnGPIO 5 true   # Simulates a rising edge on PA5
(renode) gpioPortA OnGPIO 5 false  # Resets PA5

```

When triggered, `EXTI9_5_IRQHandler` runs, toggles PA6, and sends `"EXTI5 Interrupt Triggered"` via USART2.

### Debugging with GNU Binutils

Verify memory addresses and sections:

```bash
# Check handler address
arm-none-eabi-nm -n build/stm32f401re.elf | grep EXTI9_5

# Inspect the vector table
arm-none-eabi-objdump -s -j .isr_vector build/stm32f401re.elf

```

Inspect peripheral registers directly in Renode:

```text
(renode) sysbus ReadDoubleWord 0x40020010  # Reads GPIOA_IDR
(renode) sysbus ReadDoubleWord 0x40020014  # Reads GPIOA_ODR

```

---

## 🧠 Learning Approach

This project bypasses vendor abstraction layers (like STM32 HAL) to map out exactly how hardware interacts with software:

> Datasheet → Reference Manual → Memory Map → Peripheral Base Address → Register Offset → Bit/Field Definition → Register-Level C → Custom Startup & Linker Script → ARM GCC → ELF → Renode → Peripheral Event → Interrupt Controller → Vector Table → ISR → Verify

---

## 📋 Project Status

### Completed

* [x] ARM toolchain & Renode setup
* [x] STM32F401RE Renode platform mapping
* [x] Custom linker script (`.ld`)
* [x] Custom startup assembly (`.s`)
* [x] Cortex-M4 vector table & Reset handler
* [x] `.data` and `.bss` initialization
* [x] RCC configuration & GPIO clock enables
* [x] GPIO Input/Output & AF7 setup (PA2, PA5, PA6)
* [x] USART2 configuration & 115200 baud transmission
* [x] SysTick delay
* [x] Renode UART verification
* [x] EXTI5 & SYSCFG EXTI routing
* [x] NVIC configuration (IRQ 23 identification)
* [x] Custom `EXTI9_5_IRQHandler` with pending-bit handling


---

## 📚 References

* STM32F401RE Datasheet
* STM32F401xC/E Reference Manual
* ARM Cortex-M4 Technical Reference Manual
* ARM Cortex-M4 Devices Generic User Guide
* [Renode Documentation](https://renode.readthedocs.io/)

---

## 📄 License

This project is intended for educational and research purposes.