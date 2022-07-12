#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#define MAX_TASKS			 4
#define DUMMY_XPSR			 0x01000000U

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

#define HSI_CLOCK            16000000U  // 1600000Hz OR 16Mhz
#define SYSTICK_TIM_CLK      HSI_CLOCK

#endif
