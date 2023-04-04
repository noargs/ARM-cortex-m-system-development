CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -O0
# LDFLAGS= -mcpu=$(MACH) -mfloat-abi=soft --specs=nano.specs -T stm32_ls.ld -Wl,-Map=final.map
LDFLAGS_SEMIHOSTING= -mcpu=$(MACH) -mfloat-abi=soft --specs=rdimon.specs -T stm32_ls.ld -Wl,-Map=final.map

all: main.o led.o stm32_startup.o syscalls.o final.elf
semihosting: main.o led.o stm32_startup.o syscalls.o final_semihosting.elf

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^
	
led.o: led.c
	$(CC) $(CFLAGS) -o $@ $^	
	
stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^
	
syscalls.o: syscalls.c
	$(CC) $(CFLAGS) -o $@ $^
	
final.elf: main.o led.o stm32_startup.o	syscalls.o
	$(CC) $(LDFLAGS) -o $@ $^	
	
final_semihosting.elf: main.o led.o stm32_startup.o
	$(CC) $(LDFLAGS_SEMIHOSTING) -o $@ $^		
	
clean: 
	rm -rf *.o *.elf

load:
		openocd -f board/stm32f4discovery.cfg