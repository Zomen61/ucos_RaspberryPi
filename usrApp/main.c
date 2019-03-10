#include "uart.h"
#include "regs.h"
#include "timer.h"
#include "interrupts.h"
#include "lfb.h"
#include "ucos/includes.h"

OS_STK  userAppTaskStk1[1000];
OS_STK  userAppTaskStk2[1000];
OS_STK  userAppTaskStk3[1000];
OS_STK  uartAppTaskStk[1000];

extern void userApp1(void *);
extern void userApp2(void *);
extern void userApp3(void *);
extern void uartrxApp(void *);

extern INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];

void main(){
	
	InitInterruptController();

	DisableInterrupts();

	uart_init();

	uart_int_init();

	timer_init();

	//enableMBoxInt();
	lfb_init();

	lfb_print_Pos(10, 5, "Hello World!!");

	// lfb_print('\r');
	// lfb_print('\n');

	// //lfb_clear();
	// for(int i=0;i<26;i++){
	// 	lfb_print('a'+i);
	// }
	// lfb_print('\r');
	// lfb_print('\n');

	OSInit();

	OSTaskCreate(userApp1, (void *) 0, &userAppTaskStk1[1000-1],5);

	OSTaskCreate(userApp2, (void *) 0, &userAppTaskStk2[1000-1],6);

	OSTaskCreate(userApp3, (void *) 0, &userAppTaskStk3[1000-1],7);

	OSTaskCreate(uartrxApp, (void *) 0, &uartAppTaskStk[1000-1],8);

	lfb_clear();

	OSStart();

	while(1);
}
