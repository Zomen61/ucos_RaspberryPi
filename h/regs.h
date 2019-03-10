
#ifndef REGS_H_
#define REGS_H_

#define PBASE 0x3F000000

#define GPFSEL0         ((volatile unsigned int*)(PBASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(PBASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(PBASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(PBASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(PBASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(PBASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(PBASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(PBASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(PBASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(PBASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(PBASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(PBASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(PBASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(PBASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(PBASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(PBASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(PBASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(PBASE+0x0020009C))

#define AUX_ENABLES     (PBASE+0x00215004)
#define AUX_MU_IO_REG   (PBASE+0x00215040)
#define AUX_MU_IER_REG  (PBASE+0x00215044)
#define AUX_MU_IIR_REG  (PBASE+0x00215048)
#define AUX_MU_LCR_REG  (PBASE+0x0021504C)
#define AUX_MU_MCR_REG  (PBASE+0x00215050)
#define AUX_MU_LSR_REG  (PBASE+0x00215054)
#define AUX_MU_MSR_REG  (PBASE+0x00215058)
#define AUX_MU_SCRATCH  (PBASE+0x0021505C)
#define AUX_MU_CNTL_REG (PBASE+0x00215060)
#define AUX_MU_STAT_REG (PBASE+0x00215064)
#define AUX_MU_BAUD_REG (PBASE+0x00215068)

//interrupt control regs
#define BASE_INTC			(PBASE+0x0000B200)

typedef struct {
	unsigned long	IRQBasic;	// Pending 0
	unsigned long	Pending1;
	unsigned long	Pending2;
	unsigned long	FIQCtrl;
	unsigned long	Enable1;
	unsigned long	Enable2;
	unsigned long	EnableBasic;
	unsigned long	Disable1;
	unsigned long	Disable2;
	unsigned long	DisableBasic;
} INTC_REGS;

static volatile INTC_REGS * const intcRegs = (INTC_REGS *) (BASE_INTC);

//timer regs
#define portTIMER_BASE ( (unsigned long ) (PBASE+0x0000B400) )

typedef struct _BCM2835_TIMER_REGS {
	unsigned long LOD;
	unsigned long VAL;
	unsigned long CTL;
	unsigned long CLI;
	unsigned long RIS;
	unsigned long MIS;
	unsigned long RLD;
	unsigned long DIV;
	unsigned long CNT;
} TIMER_REGS;

static volatile TIMER_REGS * const timerRegs = (TIMER_REGS *) (portTIMER_BASE);

//uart rx queue
volatile unsigned int rxhead;
volatile unsigned int rxtail;
#define RXBUFMASK 0xFFF
volatile unsigned char rxbuffer[RXBUFMASK+1];

#endif /* REGS_H_ */
