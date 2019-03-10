#include "regs.h"
#include "interrupts.h"

extern INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern unsigned int GETCPSR ( void );


void uart_irq_handler(){
    unsigned int rb,rc;

    rb=GET32(AUX_MU_IIR_REG);
    if((rb&1)==1) return; //no more interrupts
    if((rb&6)==4){
        //receiver holds a valid byte
        rc=GET32(AUX_MU_IO_REG); //read byte from rx fifo
        rxbuffer[rxhead]=rc&0xFF;
        rxhead=(rxhead+1)&RXBUFMASK;
    }
}

unsigned int uart_lcr ( void )
{
    return(GET32(AUX_MU_LSR_REG));
}

unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    return(GET32(AUX_MU_IO_REG)&0xFF);
}

void uart_send ( unsigned int c )
{
	int i;
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x20) break;
    }
    //for(i=0;i<0x7FF;i++);
    PUT32(AUX_MU_IO_REG,c);
}

void uart_flush ( void )
{
    while(1)
    {
        if((GET32(AUX_MU_LSR_REG)&0x100)==0) break;
    }
}

void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

void uart_init ( void )
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0x5);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);
    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    PUT32(GPFSEL1,ra);

    /*
    change GPPUD need sop:
    1.change GPPUD
    2.wait 150 CLK
    3.change GPPUDCLK0/1,only CLK change GPIO will change
    4.wait 150 CLK
    5.clear GPPUD
    6.clear GPPUDCLK0/1
    */
    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);
    PUT32(AUX_MU_CNTL_REG,3);

	//RegisterInterrupt(83,uart_irq_handler,(void *)0);
    //EnableInterrupt(83);
}

void uart_int_init(){
    DisableInterrupt(29);
    for(int i=0 ;i< RXBUFMASK+1; i++){
        rxbuffer[i] = 0;
    }
    
    rxhead = 0;
    rxtail = 0;

    g_VectorTable[29].pfnHandler = uart_irq_handler;

    EnableInterrupt(29);
}

void uart_string (char* s)
{
	while(*s!=0)
    {
    	uart_send((unsigned int)*s);
    	s++;
    }
	uart_send(0x0D);
	uart_send(0x0A);
}

void uart_string_noN (char* s)
{
	while(*s!=0)
    {
    	uart_send((unsigned int)*s);
    	s++;
    }
}

void printCPSR(){
    unsigned int ra;
    
    ra = GETCPSR();

    uart_string_noN("CPSR:");
    hexstring(ra);

}

