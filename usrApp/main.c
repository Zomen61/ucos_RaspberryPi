#include "uart.h"
#include "regs.h"
#include "timer.h"
#include "interrupts.h"
#include "lfb.h"
#include "ucos/includes.h"
#include "mmu.h"

OS_STK  userAppTaskStk1[1000];
OS_STK  userAppTaskStk2[1000];
OS_STK  userAppTaskStk3[1000];
OS_STK  uartAppTaskStk[1000];

extern void userApp1(void *);
extern void userApp2(void *);
extern void userApp3(void *);
extern void uartrxApp(void *);

extern INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];

void print_ADDR_value(unsigned int addr){
	hexstrings(addr);
	uart_string_noN("\t");
	hexstring(GET32(addr));
}

void testMMU(){
	unsigned int ra;

	uart_string("ADDR\t\tValue");

    PUT32(0x00045678,0x00045678);
    PUT32(0x00145678,0x00145678);
    PUT32(0x00245678,0x00245678);
    PUT32(0x00345678,0x00345678);

    PUT32(0x00346678,0x00346678);
    PUT32(0x00146678,0x00146678);

    PUT32(0x0AA45678,0x12345678);
    PUT32(0x0BB45678,0x12345678);
    PUT32(0x0CC45678,0x12345678);
    PUT32(0x0DD45678,0x12345678);

    print_ADDR_value(0x00045678);
    print_ADDR_value(0x00145678);
    print_ADDR_value(0x00245678);
    print_ADDR_value(0x00345678);
    uart_send(0x0D); uart_send(0x0A);

    for(ra=0;;ra+=0x00100000)
    {
        mmu_section(ra,ra,0x0000);
        if(ra==0xFFF00000) break;
    }

    mmu_section(0x00000000,0x00000000,0x0000|8|4);
    // mmu_section(0x00100000,0x00100000,0x0000);
    // mmu_section(0x00200000,0x00200000,0x0000);
    // mmu_section(0x00300000,0x00300000,0x0000);

    //peripherals
    mmu_section(0x3F000000,0x3F000000,0x0000); //NOT CACHED!
    mmu_section(0x3F200000,0x3F200000,0x0000); //NOT CACHED!

    start_mmu(MMUTABLEBASE,0x00000001|0x1000|0x0004); //[23]=0 subpages enabled = legacy ARMv4,v5 and v6

	uart_string("test 1(Section)");
	uart_string("origin data:");
	uart_string("ADDR\t\tValue");
    print_ADDR_value(0x00145678);
    print_ADDR_value(0x00245678);
    uart_send(0x0D); uart_send(0x0A);

	uart_string("VA 0x00100000 and VA 0x00200000 point to PA 0x00100000");
    mmu_section(0x00100000,0x00100000,0x0000);
    mmu_section(0x00200000,0x00100000,0x0000);
    invalidate_tlbs();

	uart_string("read");
	uart_string("ADDR\t\tValue");
    print_ADDR_value(0x00145678);
    print_ADDR_value(0x00245678);
	
	uart_string("write 0x00145678 to 0x00000000");
	uart_string("ADDR\t\tValue");
	PUT32(0x00145678,0);
	print_ADDR_value(0x00145678);
    print_ADDR_value(0x00245678);
    uart_send(0x0D); uart_send(0x0A);

	uart_string("write 0x00245678 to 0x00000001");
	uart_string("ADDR\t\tValue");
	PUT32(0x00245678,1);
	print_ADDR_value(0x00145678);
    print_ADDR_value(0x00245678);
    uart_send(0x0D); uart_send(0x0A);

	uart_string("test 2(small)");
	uart_string("origin data:");
	uart_string("ADDR\t\tValue");
    print_ADDR_value(0x00145678);
    uart_send(0x0D); uart_send(0x0A);

	uart_string("VA 0xAA45000 and VA 0xBB45000 point to PA 0x00145000(small page)");
    mmu_small(0x0AA45000,0x00145000,0,0x00000400);
    mmu_small(0x0BB45000,0x00145000,0,0x00000800);
    //put these back
    mmu_section(0x00100000,0x00100000,0x0000);
    invalidate_tlbs();

	uart_string("small page read");
	uart_string("ADDR\t\tValue");
    print_ADDR_value(0x0AA45678);
    print_ADDR_value(0x0BB45678);
    uart_send(0x0D); uart_send(0x0A);

	uart_string("section read");
	uart_string("ADDR\t\tValue");
    print_ADDR_value(0x00145678);
    uart_send(0x0D); uart_send(0x0A);

    //access violation.

    // mmu_section(0x00100000,0x00100000,0x0020);
    // invalidate_tlbs();

    // hexstring(GET32(0x00045678));
    // hexstring(GET32(0x00145678));
    // hexstring(GET32(0x00245678));
    // hexstring(GET32(0x00345678));
    // uart_send(0x0D); uart_send(0x0A);
}

void main(){
	
	InitInterruptController();

	DisableInterrupts();

	uart_init();

	uart_int_init();

	timer_init();

	//enableMBoxInt();
	lfb_init();

	lfb_print_Pos(10, 5, "Hello World!!");

	testMMU();

	OSInit();

	OSTaskCreate(userApp1, (void *) 0, &userAppTaskStk1[1000-1],5);

	OSTaskCreate(userApp2, (void *) 0, &userAppTaskStk2[1000-1],6);

	OSTaskCreate(userApp3, (void *) 0, &userAppTaskStk3[1000-1],7);

	OSTaskCreate(uartrxApp, (void *) 0, &uartAppTaskStk[1000-1],8);

	lfb_clear();

	OSStart();

	while(1);
}
