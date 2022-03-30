#include <stdint.h>
#include <stdio.h>

//#if !defined(__SOFT_FP__) && defined(__ARM_FP)
//  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
//#endif

void task1_handler(void); // task1
void task2_handler(void); // task2
void task3_handler(void); // task3
void task4_handler(void); // task4 of the application

/* stacks memory calculation */
#define SIZE_TASK_STACK      1024U
#define SIZE_SCHED_STACK     1024U

#define SRAM_START           0x20000000U
#define SIZE_SRAM            ( (128) * (1024) )
#define SRAM_END             ( (SRAM_START) + (SIZE_SRAM) )


#define T1_STACK_START       (SRAM_END)
#define T2_STACK_START       ( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START       ( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START       ( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define SCHED_STACK_START    ( (SRAM_END) - (4 * SIZE_TASK_STACK) )


#define TICK_HZ              1000U  // 1000Hz

#define HSI_CLOCK            16000000U  // 16000000Hz OR 16Mhz
#define SYSTICK_TIM_CLK      HSI_CLOCK


void init_systick_timer(uint32_t tick_hz);

int main(void)
{

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
	uint32_t *p_srvr = (uint32_t*) 0xE000E014;
	uint32_t count_value = (SYSTICK_TIM_CLK / tick_hz) - 1;

	// Clear the value of SVR
	*p_srvr &= ~(0x00FFFFFF);

	// load the value into SVR
	*p_srvt |= count_value;

	// do some settings


	//enable the systick

}


