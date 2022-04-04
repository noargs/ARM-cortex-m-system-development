#include <stdint.h>
#include <stdio.h>

/*
 *	Write a program to execute an SVC instruction from thread mode,
 * 	implement the SVC handler to print the SVC number used.
 * 	Also, increment the SVC number by 4 and return it to the thread mode code and print it
 *
 * 	Hints:
 * 	1) Write a main() function where you should execute the SVC instruction with an argument. let say SVC #0x5
 * 	2) Implement the SVC handler
 * 	3) In the SVC handler extract the SVC number and print it using printf
 * 	4) Increment the SVC number by 4 and return it to the thread mode
 *
 */

void SVC_Handler_c(uint32_t *p_Base_of_stack_frame);

int main(void)
{
	__asm ("SVC #8"); // or you can give #0x08

	//register uint32_t data __asm("r0");

	uint32_t data;
	__asm volatile ("MOV %0, R0": "=r"(data) ::);

	for(;;);
}

__attribute__ ((naked)) void SVC_Handler(void)
{
	//1. get the value of the MSP
	__asm ("MRS R0,MSP");
	__asm ("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *p_Base_of_stack_frame)
{
	//printf("in SVC Handler\n");

	uint8_t *p_Return_addr_PC = (uint8_t*) p_Base_of_stack_frame[6];

	//2. decrement the return address by 2 to point to
	//   opcode of the SVC instruction in the program memory
	p_Return_addr_PC -= 2;

	//3. extract the SVC number (LSByte of the Opcode)
	uint8_t svc_number = *p_Return_addr_PC;

	//printf("Svc number is : %d\n", svc_number);

	svc_number += 4;

	p_Base_of_stack_frame[0] = svc_number;
}
