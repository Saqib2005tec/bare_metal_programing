#include "stm32f401re.h"

//UART function 

static void uart_putc(char c)
{
    while (!(USART2_SR & (1U << 7)))
    {
    }

    USART2_DR = c;
}


//delay function 

static void delay_ms(uint32_t ms)
{
    SYSTICK_LOAD = 15999;
    SYSTICK_VAL  = 0;

    SYSTICK_CTRL = (1U << 2) | (1U << 0);

    for(uint32_t i = 0; i < ms; i++)
    {
        while(!(SYSTICK_CTRL & (1U << 16)))
        {
        }
    }

    SYSTICK_CTRL = 0;
}

static void uart_print(const char *str)
{
    while (*str)
    {
        uart_putc(*str++);
    }
}



int main(){

        // Enable GPIOA clock
RCC_AHB1ENR |= (1U << 0); //enable clock 
RCC_APB1ENR |= (1U << 17); // enable clock 


    // PA2 -> Alternate Function mode
GPIOA_MODER &= ~(3U << 4); // clear the 

GPIOA_MODER |= (2U << 4);  // set PA2 as alternating function 



    // PA2 -> AF7 -> USART2_TX
GPIOA_AFRL &= ~(0xFU << 8); // clear bits 

GPIOA_AFRL |= (7U << 8);



//UART  
USART2_BRR = 0x008B; // setting the buad rate to 115200 , from its 16 MHz peripheral clock.

USART2_CR1 &= ~(1U << 12);       // M = 0
USART2_CR2 &= ~(3U << 12);       // STOP = 00

//Enable USART transmitter

USART2_CR1 |= (1U << 3);     // TE = 1
USART2_CR1 |= (1U << 13);    // UE = 1


//Hello function

    while (1)
    {
        uart_print("Hello, World!\r\n");
        delay_ms(1000);
    }



}