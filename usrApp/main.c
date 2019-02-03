#include "uart.h"
#include "regs.h"
#include "timer.h"
#include "ucos/includes.h"

OS_STK  userAppTaskStk1[1000];
OS_STK  userAppTaskStk2[1000];
OS_STK  uartAppTaskStk[1000];

extern void userApp1(void *);
extern void userApp2(void *);
extern void uartrxApp(void *);

void main()
{
	
	InitInterruptController();

	DisableInterrupts();

	uart_init();

	timer_init();

	OSInit();

	EnableInterrupts();

	OSTaskCreate(userApp1, (void *) 0, &userAppTaskStk1[1000-1],5);

	OSTaskCreate(userApp2, (void *) 0, &userAppTaskStk2[1000-1],6);

	OSTaskCreate(uartrxApp, (void *) 0, &uartAppTaskStk[1000-1],7);

	OSStart();

	while(1);
}
