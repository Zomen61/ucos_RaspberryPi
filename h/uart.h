
#ifndef UART_H_
#define UART_H_

//uart rx queue
volatile unsigned int rxhead;
volatile unsigned int rxtail;
#define RXBUFMASK 0xFFF
volatile unsigned char rxbuffer[RXBUFMASK+1];

unsigned int uart_lcr ( void );
unsigned int uart_recv ( void );
void uart_send ( unsigned int );
void uart_flush ( void );
void hexstrings ( unsigned int );
void hexstring ( unsigned int );
void uart_init ( void );
void uart_int_init(void);
void uart_string(char *);
void uart_irq_handler();
void uart_string_noN (char* s);
void printCPSR();
#endif /* UART_H_ */
