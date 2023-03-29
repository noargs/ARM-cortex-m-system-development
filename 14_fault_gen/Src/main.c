#include <stdint.h>
#include <stdio.h>


int main(void)
{
    //1. enable all configurable exceptions i.e. UsageFault, MemManageFault, and BusFault
	uint32_t *p_SHCRS = (uint32*)0xE000ED24;


	*p_SHCRS |= ( 1 << 16 ); // MemMangeFault
	*p_SHCRS |= ( 1 << 17 ); // BusFault
	*p_SHCRS |= ( 1 << 18 ); // UsageFault


	//3. force the processor to execute undefined instruction
	uint32_t *p_SRAM = (uint32_t*)0x20010000;
	*p_SRAM = 0xFFFFFFFF;

	void (*random_address) (void);
	random_address = (void*)( ((uint32_t)p_SRAM) + 1 );
	random_address();

	//4. Analyse the fault

	for(;;);
}

//2. implement the fault handlers
void HardFault_Handler(void)
{
	uint32_t *p_UFSR = (uint32_t*)0xE000ED24;
	printf("Exception : HardFault\n");
	printf("UFSR = %lx\n", (*p_UFSR) & 0xFFFF);
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

void UsageFault_Handler(void)
{
	printf("Exception : UsageFault\n");
	while(1);
}
