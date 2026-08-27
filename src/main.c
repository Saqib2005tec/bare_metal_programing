#include "stm32f401re.h"


static void uart_putc(char c);
static void uart_print(const char *str);
static void delay_ms(uint32_t ms);


void enavle_NVIC_EXTI(void){
    NVIC_ISER0 |= (1U << 23); // Enable EXTI line 5 interrupt in NVIC
    EXTI_IMR |= (1U << 5); // Unmask EXTI line 5 interrupt in EXTI_IMR
    EXTI_RTSR |= (1U << 5); // Enable rising edge trigger for EXTI line 5 in EXTI_RTSR



} 
void EXTI5_init(void) //gpt 
{
    /* Enable SYSCFG clock */
    RCC_APB2ENR |= (1U << 14);

    /*
     * EXTI5 source = PA5
     *
     * SYSCFG_EXTICR2 reset value is 0,
     * which selects GPIOA.
     */

    /* Unmask EXTI5 */
    EXTI_IMR |= (1U << 5);

    /* Rising edge trigger */
    EXTI_RTSR |= (1U << 5);

    /* Enable IRQ23 in NVIC */
    NVIC_ISER0 |= (1U << 23);
}

void GPIO_init(void) //GPT 
{
    /* Enable GPIOA clock */
    RCC_AHB1ENR |= (1U << 0);

    /* PA6 -> Output */
    GPIOA_MODER &= ~(3U << 12);
    GPIOA_MODER |=  (1U << 12);

    /* PA6 -> Push-pull */
    GPIOA_OTYPER &= ~(1U << 6);

    /* PA6 -> Low speed */
    GPIOA_OSPEEDR &= ~(3U << 12);

    /* PA6 -> No pull-up/pull-down */
    GPIOA_PUPDR &= ~(3U << 12);

    /* PA5 -> Input */
    GPIOA_MODER &= ~(3U << 10);

    /* PA5 -> Pull-down */
    GPIOA_PUPDR &= ~(3U << 10);
    GPIOA_PUPDR |=  (2U << 10);
}



void EXTI9_5_IRQHandler(void)
{
    if (EXTI_PR & (1U << 5))
    {
        EXTI_PR = (1U << 5);

        GPIOA_ODR ^= (1U << 6);

        uart_print("EXTI5 Interrupt Triggered\r\n");


    }
}
//gpio init function
void GPIO_LED_init_ip(void)
{
    //gpio pa6 as output register
    GPIOA_MODER &= ~(3U << 12);
    GPIOA_MODER |=  (1U << 12);

    /* PA5 → General purpose output */
    GPIOA_MODER &= ~(3U << 10);
    GPIOA_MODER |= (1U << 10);

    /* PA5 → Push-pull */
    GPIOA_OTYPER &= ~(1U << 5);

    /* PA5 → Low speed */
    GPIOA_OSPEEDR &= ~(3U << 10);

    /* PA5 → No pull-up / pull-down */
    GPIOA_PUPDR &= ~(3U << 10);
}

//gpio output function
void GPIO_LED_init(void){
    GPIOA_MODER &= ~(3U << 10);   // 00 → Input

    GPIOA_PUPDR &= ~(3U << 10);   // clear field    
    GPIOA_PUPDR |=  (2U << 10);   // 10 → Pull-down

}

// UART function

static void uart_putc(char c)
{
    while (!(USART2_SR & (1U << 7)))
    {
    }

    USART2_DR = c;
}

// delay function

static void delay_ms(uint32_t ms)
{
    SYSTICK_LOAD = 15999;
    SYSTICK_VAL = 0;

    SYSTICK_CTRL = (1U << 2) | (1U << 0);

    for (uint32_t i = 0; i < ms; i++)
    {
        while (!(SYSTICK_CTRL & (1U << 16)))
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

// void GPIO_init(void)
// {

//     RCC_AHB1ENR |= (1U << 0);  // enable clock of GPIOA
//     RCC_APB1ENR |= (1U << 17); // enable clock of USART2

//     // PA2 -> Alternate Function mode
//     GPIOA_MODER &= ~(3U << 4); // clear the

//     GPIOA_MODER |= (2U << 4); // set PA2 as alternating function

//     // PA2 -> AF7 -> USART2_TX
//     GPIOA_AFRL &= ~(0xFU << 8); // clear bits

//     GPIOA_AFRL |= (7U << 8);
// }

void UART_init(void)
{
    USART2_BRR = 0x008B; // setting the buad rate to 115200 , from its 16 MHz peripheral clock.

    USART2_CR1 &= ~(1U << 12); // M = 0
    USART2_CR2 &= ~(3U << 12); // STOP = 00

    // Enable USART transmitter

    USART2_CR1 |= (1U << 3);  // TE = 1
    USART2_CR1 |= (1U << 13); // UE = 1
}


int main(void)
{
    GPIO_init();
    UART_init();

    GPIO_LED_init_ip();
    EXTI5_init();

    while (1)
    {
    }
    return 0;
}

