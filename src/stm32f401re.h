//base adress 

#ifndef STM32F401RE_H
#define STM32F401RE_H

#include <stdint.h>

#define RCC_BASE     0x40023800UL
#define GPIOA_BASE   0x40020000UL
#define USART2_BASE  0x40004400UL

//gpio pa6 as output register

#define GPIOA_IDR     0x40020010


#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE + 0x10UL))

//gpio LED reg 
#define GPIOA_BASE       0x40020000UL

#define GPIOA_MODER      (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_OTYPER     (*(volatile uint32_t *)(GPIOA_BASE + 0x04UL))
#define GPIOA_OSPEEDR    (*(volatile uint32_t *)(GPIOA_BASE + 0x08UL))
#define GPIOA_PUPDR      (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
#define GPIOA_ODR        (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))


#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE + 0x10UL))  //gpio input data register


// NVIC register
#define NVIC_ISER0 (*(volatile uint32_t *)0xE000E100) // NVIC Interrupt Set-Enable Register 0
#define EXTI_IMR (*(volatile uint32_t *)0x40013C00) // EXTI Interrupt Mask Register
#define EXTI_RTSR (*(volatile uint32_t *)0x40013C08) // EXTI Rising Trigger Selection Register
#define EXTI_PR (*(volatile uint32_t *)0x40013C14) // EXTI Pending Register




#endif

// defining the register 

#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x40UL))
#define RCC_APB2ENR (*(volatile uint32_t *)0x40023844) // RCC APB2 peripheral clock enable register for EXTI



#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_AFRL    (*(volatile uint32_t *)(GPIOA_BASE + 0x20UL))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))

#define USART2_SR     (*(volatile uint32_t *)(USART2_BASE + 0x00UL))
#define USART2_DR     (*(volatile uint32_t *)(USART2_BASE + 0x04UL))
#define USART2_BRR    (*(volatile uint32_t *)(USART2_BASE + 0x08UL))
#define USART2_CR1    (*(volatile uint32_t *)(USART2_BASE + 0x0CUL))
#define USART2_CR2    (*(volatile uint32_t *)(USART2_BASE + 0x10UL))



// systic timer 
#define SYSTICK_CTRL  (*(volatile uint32_t *)0xE000E010UL)
#define SYSTICK_LOAD  (*(volatile uint32_t *)0xE000E014UL)
#define SYSTICK_VAL   (*(volatile uint32_t *)0xE000E018UL)