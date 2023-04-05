#include <stdio.h>
#include "main.h"
#include "led.h"


void task1_handler(void); // task1
void task2_handler(void); // task2
void task3_handler(void); // task3
void task4_handler(void); // task4 of the application


__attribute__((naked)) void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t sched_stack_start);
void init_tasks_stack();
void enable_processor_faults(void);
uint32_t get_psp_value(void);
__attribute__((naked)) void switch_sp_to_psp(void);
void save_psp_value(uint32_t current_psp_value);
void update_next_task(void);


//uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START, T2_STACK_START, T3_STACK_START, T4_STACK_START};
//uint32_t task_handlers[MAX_TASKS];

uint8_t current_task = 1; // task1 is running on the processor
uint32_t g_tick_count = 0;

void task_delay(uint32_t tick_count);

typedef struct
{
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t current_state;
	void (*task_handler)(void);
}TCB_t;

TCB_t user_tasks[MAX_TASKS];

int main(void)
{
	enable_processor_faults();

	init_scheduler_stack(SCHED_STACK_START);


//	task_handlers[0] = (uint32_t)task1_handler;
//	task_handlers[1] = (uint32_t)task2_handler;
//	task_handlers[2] = (uint32_t)task3_handler;
//	task_handlers[3] = (uint32_t)task4_handler;

	init_tasks_stack();

	led_init_all();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	for(;;);
}

void idle_task(void)
{
	while(1);
}

void task1_handler(void)
{
	while(1)
	{
		led_on(LED_GREEN);
		delay(DELAY_COUNT_1S);
		led_off(LED_GREEN);
		delay(DELAY_COUNT_1S);

		//printf("This is task1\n");
	}
}

void task2_handler(void)
{
	while(1)
	{
		led_on(LED_ORANGE);
		delay(DELAY_COUNT_500MS);
		led_off(LED_ORANGE);
		delay(DELAY_COUNT_500MS);

		// printf("This is task2\n");
	}
}

void task3_handler(void)
{
	while(1)
	{
		led_on(LED_BLUE);
		delay(DELAY_COUNT_250MS);
		led_off(LED_BLUE);
		delay(DELAY_COUNT_250MS);

		// printf("This is task3\n");
	}
}

void task4_handler(void)
{
	while(1)
	{
		led_on(LED_RED);
		delay(DELAY_COUNT_125MS);
		led_off(LED_RED);
		delay(DELAY_COUNT_125MS);

		// printf("This is task4\n");
	}
}

void init_systick_timer(uint32_t tick_hz)
{
	uint32_t *p_srvr = (uint32_t*)0xE000E014;
	uint32_t *p_scsr = (uint32_t*)0xE000E010;


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


void init_tasks_stack(){

	user_tasks[0].current_state = TASK_READY_STATE;
	user_tasks[1].current_state = TASK_READY_STATE;
	user_tasks[2].current_state = TASK_READY_STATE;
	user_tasks[3].current_state = TASK_READY_STATE;
	user_tasks[4].current_state = TASK_READY_STATE;

	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;


	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	uint32_t *psp;

	for(int i=0; i < MAX_TASKS; i++){
		// psp = (uint32_t*) psp_of_tasks[i];
		psp = (uint32_t*) user_tasks[i].psp_value;

		psp--;
		*psp = DUMMY_XPSR; // xPSR 0x01000000

		psp--; // PC
		//*psp = task_handlers[i];
		*psp = (uint32_t) user_tasks[i].task_handler;

		psp--; // LR - page[2-28] ARM Cortex-M4 DUI0553
		*psp = 0xFFFFFFFD;

		for(int j=0; j < 13; j++){ // rest of 13 registers initialised to 0s
			psp--;
			*psp = 0;
		}

		//psp_of_tasks[i] = (uint32_t)psp;
		user_tasks[i].psp_value = (uint32_t)psp;

	}
}

void enable_processor_faults(void)
{
	uint32_t *shcr = (uint32_t*)0xE000ED24;

	*shcr |= ( 1 << 16 ); // mem manage
	*shcr |= ( 1 << 17 ); // bus fault
	*shcr |= ( 1 << 18 ); // usage fault
}

uint32_t get_psp_value(void)
{
	// return psp_of_tasks[current_task];
	return user_tasks[current_task].psp_value;
}

void save_psp_value(uint32_t current_psp_value)
{
	// psp_of_tasks[current_task] = current_psp_value;
	user_tasks[current_task].psp_value = current_psp_value;
}

void update_next_task(void)
{
	current_task++;
	current_task %= MAX_TASKS; // current_task = current_task % MAX_TASKS
}

__attribute__((naked)) void switch_sp_to_psp(void)
{
	//1. initialise the PSP with TASK1 stack start
	// get the value of psp of current_task
	__asm volatile ("PUSH {LR}");       // preserve LR which connects back to main()
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP,R0");      // initialise psp
	__asm volatile ("POP {LR}");        // pops back LR value

	//2. change SP to PSP using CONTROL register
	__asm volatile ("MOV R0,#0X02");
	__asm volatile ("MSR CONTROL,R0");
	__asm volatile ("BX LR");
}

void schedule(void)
{
	// pend the pendSV exception
	uint32_t *p_icsr = (uint32_t*)0xE000ED04;
	*p_icst |= (1 << 28);
}

void task_delay(uint32_t tick_count) {
	if(current_task){
		user_tasks[current_task].block_count = g_tick_count + tick_count;
		user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
		schedule();
	}
}

__attribute__((naked)) void PendSV_Handler(void) {
	/* Save the context of current task */

	//1. Get current running task's PSP value
	__asm volatile ("MRS R0,PSP");
	//2. Using that PSP value store SF2 (R4 to R11) as SF1 is stored by default
	__asm volatile ("STMDB R0!,{R4-R11}");

	// important to save LR value before running
	//   any BL instruction
	__asm volatile ("PUSH {LR}");

	//3. Save the current value of PSP
	__asm volatile ("BL save_psp_value");


	/* Retrieve the context of next task */
	//1. Decide next task to run
	__asm volatile ("BL update_next_task");
	//2. get its past PSP value
	__asm volatile ("BL get_psp_value");
	//3. Using that PSP value retrieve SF2 (R4 to R11) - Data movement from memory -> register
	__asm volatile ("LDMIA R0!,{R4-R11}");
	//4. update PSP and exit
	__asm volatile ("MSR PSP,R0");

	__asm volatile ("POP {LR}");

	__asm volatile ("BX LR");
}

void update_global_tick_count(void)
{
	g_tick_count++;
}

void unblock_tasks(void) {
	for (int i=1; i < MAX_TASKS; i++) {
		if (user_tasks[i].current_state != TASK_READY_STATE) {
			if (user_tasks[i].block_count == g_tick_count) {
				user_tasks[i].current_state = TASK_READY_STATE;
			}
		}
	}
}

void SysTick_Handler(void)
{
	uint32_t *p_icsr = (uint32_t*)0xE000ED04;

	update_global_tick_count();
	unblock_tasks();

	// pend the pendSV exception
	*p_icst |= (1 << 28);

}

//fault handlers
void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}
void MemManage_Handler(void)
{
	printf("Exception : MemMange\n");
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


