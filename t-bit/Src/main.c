#include <stdint.h>
#include <stdio.h>


void change_access_level_unpriv(void)
{
	//read
	__asm volatile("MRS R0,CONTROL");

	//modify
	__asm volatile("ORR R0,R0,#0x01");

	//write
	__asm volatile("MSR CONTROL,R0");

}


/* This function executes in THREAD MODE + PRIVILEDGED ACCESS LEVEL  of the processor */
int main(void)
{
	void (*fun_ptr)(void);

	fun_ptr = change_access_level_unpriv;

	fun_ptr();

	for(;;);
}

void HardFault_Handler(void)
{
	printf("Hard fault detected\n");
	while(1);

}

