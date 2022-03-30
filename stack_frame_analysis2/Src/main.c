#include <stdint.h>
#include <stdio.h>

void UsageFault_Handler_c(uint32_t *p_Base_stack_frame);

int fun_divide(int x, int y)
{
	return x/y;
}

int main(void)
{
    //1. enable all configurable exceptions i.e. UsageFault, MemManageFault, and BusFault
	uint32_t *p_SHCRS = (uint32_t*)0xE000ED24;


	*p_SHCRS |= ( 1 << 16 ); // MemMangeFault
	*p_SHCRS |= ( 1 << 17 ); // BusFault
	*p_SHCRS |= ( 1 << 18 ); // UsageFault


	//2. enable divide by 0 trap
	uint32_t *p_CCR = (uint32_t*)0xE000ED14;
	*p_CCR |= ( 1 << 4 );

	//3. attempt to divide by zero
	fun_divide(10, 0);

	//4. Analyse the fault

	for(;;);
}

//2. implement the fault handlers
void HardFault_Handler(void)
{
	printf("Exception : HardFault\n");
	while(1);
}

void MemManage_Handler(void)
{
	printf("Exception : MemManageFault\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}

__attribute__ ((naked)) void UsageFault_Handler(void)
{
	//here we extracted the value of MSP which happens to be the
	// base address of the stack frame which got saved during the exception entry
	// from thread mode to handler mode
	__asm ("MRS r0,MSP");
	__asm ("B UsageFault_Handler_c");
}

void UsageFault_Handler_c(uint32_t *p_Base_stack_frame)
{
	uint32_t *p_UFSR = (uint32_t*)0xE000ED2A;
	printf("Exception : UsageFault\n");
	printf("UFSR = %lx\n", (*p_UFSR) & 0xFFFF);
	printf("p_Base_stack_frame = %p\n", p_Base_stack_frame);

	printf("Value of R0   = %lx\n",   p_Base_stack_frame[0]);
	printf("Value of R1   = %lx\n",   p_Base_stack_frame[1]);
	printf("Value of R2   = %lx\n",   p_Base_stack_frame[2]);
	printf("Value of R3   = %lx\n",   p_Base_stack_frame[3]);
	printf("Value of R12  = %lx\n",   p_Base_stack_frame[4]);
	printf("Value of LR   = %lx\n",   p_Base_stack_frame[5]);
	printf("Value of PC   = %lx\n",   p_Base_stack_frame[6]);
	printf("Value of xPSR = %lx\n",   p_Base_stack_frame[7]);
	while(1);
}



















