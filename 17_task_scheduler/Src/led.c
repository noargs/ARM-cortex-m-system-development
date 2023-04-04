#include <stdint.h>
#include "led.h"

void delay(uint32_t count)
{
	for(uint32_t i = 0 ; i < count ; i++);
}

void led_init_all(void)
{
	uint32_t *p_rcc_ahb1enr   = (uint32_t*) 0x40023830;
	uint32_t *p_gpio_mode_reg = (uint32_t*) 0x40020C00;

	*p_rcc_ahb1enr |= ( 1 << 3);

	// configure LED_GREEN
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_GREEN ));  // #define LED_GREEN  12
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_ORANGE )); // #define LED_ORANGE 13
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_RED ));    // #define LED_RED    14
	*p_gpio_mode_reg |= ( 1 << ( 2 * LED_BLUE ));   // #define LED_BLUE   15

#if 0
	// configure
	*p_gpio_optype_reg |= (1 << (2 * LED_GREEN));
	*p_gpio_optype_reg |= (1 << (2 * LED_ORANGE));
	*p_gpio_optype_reg |= (1 << (2 * LED_RED));
	*p_gpio_optype_reg |= (1 << (2 * LED_BLUE));
#endif

	led_off(LED_GREEN);
	led_off(LED_ORANGE);
	led_off(LED_RED);
	led_off(LED_BLUE);
}

void led_on (uint8_t led_no)
{
	uint32_t *p_gpio_data_reg = (uint32_t*)0x40020C14;
	*p_gpio_data_reg |= (1 << led_no);
}

void led_off(uint8_t led_no)
{
	uint32_t *p_gpiod_data_reg = (uint32_t*)0x40020C14;
	*p_gpiod_data_reg &= ~(1 << led_no);
}

