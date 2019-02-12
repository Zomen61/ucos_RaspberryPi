#include "uart.h"
#include "regs.h"
#include "timer.h"
#include "ucos/includes.h"

OS_STK  userAppTaskStk1[1000];
OS_STK  userAppTaskStk2[1000];
<<<<<<< HEAD
OS_STK  uartAppTaskStk[1000];

extern void userApp1(void *);
extern void userApp2(void *);
extern void uartrxApp(void *);

void main()
{
	
=======

extern void userApp1(void *);
extern void userApp2(void *);

void main()
{
	uart_init();

>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	InitInterruptController();

	DisableInterrupts();

<<<<<<< HEAD
	uart_init();

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	timer_init();

	OSInit();

<<<<<<< HEAD
	EnableInterrupts();

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	OSTaskCreate(userApp1, (void *) 0, &userAppTaskStk1[1000-1],5);

	OSTaskCreate(userApp2, (void *) 0, &userAppTaskStk2[1000-1],6);

<<<<<<< HEAD
	OSTaskCreate(uartrxApp, (void *) 0, &uartAppTaskStk[1000-1],7);

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	OSStart();

	while(1);
}
