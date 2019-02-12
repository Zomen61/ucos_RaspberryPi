
ARMGNU ?= arm-none-eabi

INCLUDEPATH ?= "./h"

<<<<<<< HEAD
COPS = -Wall -O2 -lm -lgcc -ffreestanding -std=gnu99 -mcpu=cortex-a53 -I $(INCLUDEPATH)
=======
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -I $(INCLUDEPATH)
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724

gcc : kernel.img

OBJS = build/startup.o 

OBJS += build/uart.o
OBJS += build/timer.o
OBJS += build/interrupts.o

OBJS += build/OS_Cpu_a.o
OBJS += build/OS_Cpu_c.o

OBJS += build/ucos_ii.o

OBJS += build/main.o
OBJS += build/userApp.o

OBJS += lib/libc.a
OBJS += lib/libgcc.a 

clean :
	rm -f build/*.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f build/*.bc

build/%.o : port/%.s
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c -o $@ $<
	
build/%.o : init/%.s
	$(ARMGNU)-gcc $(COPS) -D__ASSEMBLY__ -c -o $@ $<
	
build/%.o : port/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
		
build/%.o : bsp/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<
	
build/%.o : usrApp/%.c
	$(ARMGNU)-gcc $(COPS)  -c -o $@ $<

build/ucos_ii.o : ucos/ucos_ii.c
	$(ARMGNU)-gcc $(COPS) ucos/ucos_ii.c -c -o build/ucos_ii.o

kernel.img : raspberrypi.ld $(OBJS)
<<<<<<< HEAD
	$(ARMGNU)-ld $(OBJS) -T raspberrypi.ld -o ucos_bcm2837.elf 
	$(ARMGNU)-objdump -D ucos_bcm2837.elf > ucos_bcm2837.list
	$(ARMGNU)-objcopy ucos_bcm2837.elf -O ihex ucos_bcm2837.hex
	$(ARMGNU)-objcopy ucos_bcm2837.elf -O binary ucos_bcm2837.bin
	$(ARMGNU)-objcopy ucos_bcm2837.elf -O binary ucos_bcm2837.img
=======
	$(ARMGNU)-ld $(OBJS) -T raspberrypi.ld -o ucos_bcm2835.elf 
	$(ARMGNU)-objdump -D ucos_bcm2835.elf > ucos_bcm2835.list
	$(ARMGNU)-objcopy ucos_bcm2835.elf -O ihex ucos_bcm2835.hex
	$(ARMGNU)-objcopy ucos_bcm2835.elf -O binary ucos_bcm2835.bin
	$(ARMGNU)-objcopy ucos_bcm2835.elf -O binary kernel.img
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
