/* STM32F401RE linker script
 *
 * Flash: 512 KB
 * SRAM : 96 KB
 */

ENTRY(Reset_Handler)

/* Initial stack pointer */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Minimum heap and stack sizes */
_Min_Heap_Size  = 0x000;
_Min_Stack_Size = 0x400;

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    /* Interrupt vector table and program code */
    .isr_vector :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        . = ALIGN(4);
    } > FLASH


    /* Program code */
    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text*)

        . = ALIGN(4);

        _etext = .;
    } > FLASH


    /* Read-only data */
    .rodata :
    {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH


    /*
     * Initialized data.
     *
     * The initial values are stored in Flash,
     * but variables themselves live in RAM.
     */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH

    /* Flash address containing the initial .data values */
    _sidata = LOADADDR(.data);


    /*
     * Zero-initialized data.
     */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > RAM


    /*
     * Heap and stack reservation.
     */
    ._user_heap_stack :
    {
        . = ALIGN(8);

        PROVIDE(end = .);
        PROVIDE(_end = .);

        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;

        . = ALIGN(8);
    } > RAM


    /*
     * ARM exception/unwind information.
     */
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH


    /*
     * Remove unused standard-library sections.
     */
    /DISCARD/ :
    {
        *(.note.GNU-stack)
        *(.comment)
    }
}