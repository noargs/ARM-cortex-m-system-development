#include <stdint.h>
#include <stdio.h>

#define IRQNO_TIMER2    28
#define IRQNO_I2C1      31

uint32_t *p_NVIC_IPRBase  = (uint32_t*)0xE000E400;
uint32_t *p_NVIC_ISERBase = (uint32_t*)0xE000E100;
uint32_t *p_NVIC_ISPRBase = (uint32_t*)0xE000E200;


void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value)
{
	//1. find out iprx
	uint8_t iprx = irq_no / 4;
	uint32_t *ipr = p_NVIC_IPRBase + iprx;

	//2. position in iprx
	uint8_t position = (irq_no % 4) * 8;
	printf("positon %d\n", position);


	//3. configure the priority
	*ipr &= ~(0xFF << position); //clear
	*ipr |= (priority_value << position);

}

int main(void)
{
    //1. Configure the priority for the peripherals
	configure_priority_for_irqs(IRQNO_TIMER2, 0x80);
	configure_priority_for_irqs(IRQNO_I2C1, 0x80);

	//2. Set the interrupt pending bit in the NVIC PR
	*p_NVIC_ISPRBase |= (1 << IRQNO_TIMER2);

	//3. Enable the IRQs in NVIC ISER
	*p_NVIC_ISERBase |= (1 << IRQNO_I2C1);
	*p_NVIC_ISERBase |= (1 << IRQNO_TIMER2);


	for(;;);
}

// ISRs

void TIM2_IRQHandler(void)
{
	printf("[TIM2_IRQHandler]\n");
	*p_NVIC_ISPRBase |= (1 << IRQNO_I2C1);
	while(1);
}

void I2C1_EV_IRQHandler(void)
{
	printf("[I2C1_EV_IRQHandler]\n");
}
