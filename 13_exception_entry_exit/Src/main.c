#include <stdint.h>
#include <stdio.h>

/* This function executes in THREAD MODE of the processor */
void generate_interrupt()
{
	uint32_t *p_stir  = (uint32_t*) 0xE000EF00;
	uint32_t *p_iser0 = (uint32_t*) 0xE000E100;

	// enable IRQ3 interrupt
	*p_iser0 |= (1 << 3);

	// generate an interrupt from software for IRQ3
	*p_stir = (3 & 0x1FF);
}


/* This function executes in THREAD MODE of the processor */
int main(void)
{
	int control_reg = 0x2;
	uint32_t psp_value = 0x20000000;

	__asm volatile ("MSR PSP, %0"::"r"(psp_value));
	__asm volatile ("MSR CONTROL, %0"::"r"(control_reg));

	printf("In thread mode: before interrupt\n");

	generate_interrupt();

	printf("In thread mode: after interrupt\n");

	for(;;);
}

/* This function (ISR) executes in Handler Mode of the processor */
void RTC_WKUP_IRQHandler(void)
{
	printf("In handler mode: ISR\n");
}

















































