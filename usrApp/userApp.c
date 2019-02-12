#include "uart.h"
#include "ucos/includes.h"

<<<<<<< HEAD

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
#define GPSETO 0x3F20001C
#define GPCLR0 0x3F200028

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
void userApp2(void * args)
{
	while(1)
	{
<<<<<<< HEAD
		PUT32(GPSETO, 1 << 16);
=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
		uart_string("in userApp2");
		OSTimeDly(100);
	}
}

void userApp1(void * args)
{

	while(1)
	{
		uart_string("in userApp1");
		OSTimeDly(100);
<<<<<<< HEAD
		PUT32(GPCLR0, 1 << 16);
	}
}

void uartrxApp(void * args){
	unsigned int rx;
	while(1){
		while(rxtail!=rxhead){
            uart_send(rxbuffer[rxtail]);
            rxtail=(rxtail+1)&RXBUFMASK;
            rx++;
        }
	}
}
=======
	}
}
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
