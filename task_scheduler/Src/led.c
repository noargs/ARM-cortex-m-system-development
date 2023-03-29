#include <stdint.h>
#include "led.h"

void delay(uint32_t count)
{
	for(uint32_t i = 0 ; i < count ; i++);
}

void let_init_all(void)
{
	uint32_t *p_rcc_ahb1enr   = (uint32_t*) 0x40023830;
	uint32_t *p_gpio_mode_reg = (uint32_t*) 0x40020C00;

	*p_rcc_ahb1enr |= ( 1 << 3);

	// configure LED_GREEN
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_GREEN ));
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_ORANGE ));
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_RED ));
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_BLUE ));
}

