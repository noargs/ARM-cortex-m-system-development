#include <stdint.h>
#include <stdio.h>

#define USART3_IRQNO     39

int main(void)
{

	//1. Manually pend the pending bit for the USART3 IRQ number in NVIC
	uint32_t *p_ISPR1 = (uint32_t*)0xE000E204;

	*p_ISPR1 |= ( 1 << (USART3_IRQNO % 32));

	//2. Enable the USART3 IRQ number in NVIC
	uint32_t *p_ISER1 = (uint32_t*)0xE000E104;
	*p_ISER1 |= ( 1 << (USART3_IRQNO % 32));

	for(;;);
}

// USART3 ISR
void USART3_IRQHandler(void)
{
	printf("in USART3 isr\n");
}
