#include <stdint.h>


int main(void)
{
	// Initialise R1 and R2 registers
	__asm volatile("LDR R1,=#0x20001000");
	__asm volatile("LDR R2,=#0x20001004");

	// we loaded two values from memory location R1 and R2
	__asm volatile("LDR R0,[R1]");
	__asm volatile("LDR R1,[R2]");
	__asm volatile("ADD R0,R0,R1");
	__asm volatile("STR R0,[R2]"); // store the result back to address pointed by R2

	for(;;);
}
