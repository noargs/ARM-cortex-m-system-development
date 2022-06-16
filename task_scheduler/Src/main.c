#include <stdio.h>
#include "main.h"


void task1_handler(void); // task1
void task2_handler(void); // task2
void task3_handler(void); // task3
void task4_handler(void); // task4 of the application


void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_stack_start);
void init_task_stack();


uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
uint32_t task_handlers[MAX_TASKS];

int main(void)
{
	init_scheduler_stack(SCHED_STACK_START);


	task_handlers[0] = (uint32_t)task1_handler;
	task_handlers[1] = (uint32_t)task2_handler;
	task_handlers[2] = (uint32_t)task3_handler;
	task_handlers[3] = (uint32_t)task4_handler;

	init_task_stack();

	init_systick_timer(TICK_HZ);

	for(;;);
}

void task1_handler(void)
{
	while(1)
	{
		printf("This is task1\n");
	}
}

void task2_handler(void)
{
	while(1)
	{
		printf("This is task2\n");
	}
}

void task3_handler(void)
{
	while(1)
	{
		printf("This is task3\n");
	}
}

void task4_handler(void)
{
	while(1)
	{
		printf("This is task4\n");
	}
}

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *p_scsr = (uint32_t*)0xE000E010;
	uint32_t *p_srvr = (uint32_t*)0xE000E014;

	// to generate exception every 100 clock cycles use 100 - 1 = 99 reload value for SRVR
	uint32_t count_value = (SYSTICK_TIM_CLK / tick_hz) - 1;

	// Clear the value of SVR
	*p_srvr &= ~(0x00FFFFFF);

	// load the value into SVR
	*p_srvr |= count_value;

	// do some settings
	*p_scsr |= (1 << 1); // Enable SysTick Exception Request
	*p_scsr |= (1 << 2); // Indicates the clock source, processor clock source

	//enable the systick
	*p_scsr |= (1 << 0); // Enables the counter

}

__attribute__((naked)) void init_scheduler_stack(uint32_t sched_stack_start){
	__asm volatile("MSR MSP,%0": : "r"(sched_stack_start) : );
	__asm volatile("BX LR");
}

void init_task_stack(){

	uint32_t *psp;
	for(int i=0; i < MAX_TASKS; i++){
		psp = (uint32_t*) psp_of_tasks[i];

		psp--;
		*psp = DUMMY_XPSR; // xPSR 0x00100000

		psp--; // PC
		*psp = task_handlers[i];

		psp--; // LR - page[2-28] ARM Cortex-M4 DUI0553
		*psp = 0xFFFFFFFD;

		for(int j=0; j < 13; j++){
			psp--;
			*psp = 0;
		}

		psp_of_tasks[i] = (uint32_t)psp;

	}
}

void SysTick_Handler(void)
{

}


