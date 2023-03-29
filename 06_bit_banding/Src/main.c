#include <stdint.h>

#define ALIAS_BASE     0x22000000U
#define BITBAND_BASE   0x20000000U

int main(void)
{

	uint8_t *ptr = (uint8_t*)0x20000200;
	*ptr = 0xff;

	// NORMAL METHOD
	// clearing 7th bit position
	*ptr &= ~(1 << 7);

	// reset to 0xff
	*ptr = 0xff;

	//BIT BAND METHOD
	uint8_t *alias_addr = (uint8_t*) (ALIAS_BASE + (32 * (0x20000200 - BITBAND_BASE)) + 7 * 4);

	// clearing 7th bit of address 0x20000200
	*alias_addr = 0;
	for(;;);
}
