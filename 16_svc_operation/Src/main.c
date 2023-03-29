#include <stdint.h>
#include <stdio.h>

/*
 *
 * Write a program to add, subtract, multiply, and divide 2 operands using
 * SVC Handler and return the result to the thread mode code and print
 * the result. Thread mode code should pass 2 operands via the stack frame
 *
 * Use the SVC number to decide the operation
 *
 *     [Service number]            [Operation]
 *           36                      Addition
 *           37                      Subtraction
 *           38                      Multiplication
 *           39                      Division
 */

void SVC_Handler_c(uint32_t *p_Base_stack_frame);

int32_t add_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #36");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t sub_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #37");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t mul_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #38");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}
int32_t div_numbers(int32_t x, int32_t y)
{
	int32_t res;
	__asm volatile("SVC #39");
	__asm volatile("MOV %0,R0": "=r"(res) ::);
	return res;
}

int main(void)
{
	int32_t res;

	res = add_numbers(20, -30);
	res = sub_numbers(81, 20);
	res = mul_numbers(2, 200);
	res = div_numbers(100, 5);


	for(;;);
}

__attribute__ (( naked )) void SVC_Handler( void )
{
	__asm ("MRS r0,MSP");
	__asm ("B SVC_Handler_c");
}

void SVC_Handler_c(uint32_t *p_Base_stack_frame)
{
	int32_t arg0, arg1, res;

	uint8_t *p_Return_addr_PC = (uint8_t*) p_Base_stack_frame[6];

	//2. decrement the return address by 2 to point to
	//opcode of the SVC instruction in the program memory
	p_Return_addr_PC -= 2;

	//3. extract the SVC number (LSByte of the opcode)
	uint8_t svc_number = *p_Return_addr_PC;

	arg0 = p_Base_stack_frame[0];   // register R0 contain argument 1 of the function as per AAPC
	arg1 = p_Base_stack_frame[1];   // register R1 contain argument 2 of the function as per AAPC

	switch(svc_number)
	{
	case 36:
		res = arg0 + arg1;
		break;
	case 37:
		res = arg0 - arg1;
		break;
	case 38:
		res = arg0 * arg1;
		break;
	case 39:
		res = arg0 / arg1;
	default:
		printf("invalid svc code\n");
	}

	p_Base_stack_frame[0] = res;

}
