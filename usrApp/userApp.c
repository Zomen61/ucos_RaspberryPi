#include "uart.h"
#include "regs.h"
#include "mbox.h"
#include "lfb.h"
#include "ucos/includes.h"

#define dlyCLK 1000


extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);


void userApp3(void * args)
{
	while(1)
	{
		// get the board's unique serial number with a mailbox call
		mbox[0] = 8*4;                  // length of the message
		mbox[1] = MBOX_REQUEST;         // this is a request message
		
		mbox[2] = MBOX_TAG_GETSERIAL;   // get serial number command
		mbox[3] = 8;                    // buffer size
		mbox[4] = 8;
		mbox[5] = 0;                    // clear output buffer
		mbox[6] = 0;

		mbox[7] = MBOX_TAG_LAST;

		// send the message to the GPU and receive answer
		if (mbox_call(MBOX_CH_PROP)) {
			uart_string_noN("My serial number is: ");
			hexstrings(mbox[6]);
			hexstring(mbox[5]);
		} else {
			uart_string("Unable to query serial!\n");
		}
		OSTimeDly(dlyCLK);
	}
}

void userApp2(void * args)
{
	while(1)
	{
		PUT32(GPSET0, 1 << 16);
		uart_string("in userApp2");
		OSTimeDly(dlyCLK);
	}
}

void userApp1(void * args)
{

	while(1)
	{
		uart_string("in userApp1");
		OSTimeDly(dlyCLK);
		PUT32(GPCLR0, 1 << 16);
	}
}

void uartrxApp(void * args){
	unsigned int rx;
	while(1){
		while(rxtail != rxhead){
            uart_send(rxbuffer[rxtail]);
			lfb_print(rxbuffer[rxtail]);
			if(rxbuffer[rxtail] == '\r'){
				lfb_print('\n');
			}
            rxtail=(rxtail+1)&RXBUFMASK;
            rx++;
        }
		OSTimeDly(10);
	}
}