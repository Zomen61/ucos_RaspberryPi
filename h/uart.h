
#ifndef UART_H_
#define UART_H_

<<<<<<< HEAD
//uart rx queue
volatile unsigned int rxhead;
volatile unsigned int rxtail;
#define RXBUFMASK 0xFFF
volatile unsigned char rxbuffer[RXBUFMASK+1];

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
unsigned int uart_lcr ( void );
unsigned int uart_recv ( void );
void uart_send ( unsigned int );
void uart_flush ( void );
void hexstrings ( unsigned int );
void hexstring ( unsigned int );
void uart_init ( void );
void uart_string(char *);
<<<<<<< HEAD
void uart_irq_handler();
=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
#endif /* UART_H_ */
