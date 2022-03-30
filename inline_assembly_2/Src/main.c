#include <stdint.h>


int main(void)
{
//	int val = 50;
//	__asm volatile("MOV R0,%0": : "r"(val));

	int control_reg;
	// read CONTROL register to control_reg
	__asm volatile("MRS %0,CONTROL" : "=r"(control_reg));

	for(;;);
}
