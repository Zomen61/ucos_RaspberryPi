
#include "interrupts.h"
#include "regs.h"
#include "uart.h"

INTERRUPT_VECTOR g_VectorTable[BCM2835_INTC_TOTAL_IRQ];

/**
 *	Enables all IRQ's in the CPU's CPSR register.
 **/
static void irqEnable() {
	__asm volatile("mrs 	r0,cpsr");		// Read in the cpsr register.
	__asm volatile("bic		r0,r0,#0x80");	// Clear bit 8, (0x80) -- Causes IRQs to be enabled.
	__asm volatile("msr		cpsr_c, r0");	// Write it back to the CPSR register
}

static void irqDisable() {
	__asm volatile("mrs		r0,cpsr");		// Read in the cpsr register.
	__asm volatile("orr		r0,r0,#0x80");	// Set bit 8, (0x80) -- Causes IRQs to be disabled.
	__asm volatile("msr		cpsr_c, r0");	// Write it back to the CPSR register.

}

#define clz(a) \
 ({ unsigned long __value, __arg = (a); \
     asm ("clz\t%0, %1": "=r" (__value): "r" (__arg)); \
     __value; })


static void handleRange (unsigned long pending, const unsigned int base)
{
	while (pending)
	{
		// Get index of first set bit:
		unsigned int bit = 31 - __builtin_clz(pending);

		// Map to IRQ number:
		unsigned int irq = base + bit;

		// Call interrupt handler, if enabled:
		if (g_VectorTable[irq].pfnHandler)
			g_VectorTable[irq].pfnHandler(irq, g_VectorTable[irq].pParam);

		// Clear bit in bitfield:
		pending &= ~(1UL << bit);
	}
}
/**
 *	This is the global IRQ handler  on this platform!
 *	It is based on the assembler code found in the Broadcom datasheet.
 *
 **/
void OS_CPU_IRQ_ISR_Handler() {
	/**
	 *	WARNING:
	 *		Compiler optimisations will likely prevent this code from working!
	 *		
	 *	This will be fixed later!
	 **/

	register unsigned long ulMaskedStatus;
	register unsigned long irqNumber;
	register unsigned long tmp;

	ulMaskedStatus = intcRegs->IRQBasic;
	tmp = ulMaskedStatus & 0x00000300;			// Check if anything pending in pr1/pr2.   

	// uart_string_noN("pending IRQBasic:");
	// hexstring(ulMaskedStatus);
	// uart_string_noN("pending 1:");
	// hexstring(intcRegs->Pending1);
	// uart_string_noN("pending 2:");
	// hexstring(intcRegs->Pending2);

	if(ulMaskedStatus & ~0xFFFFF300) {			// Note how we mask out the GPU interrupt Aliases.
		handleRange(ulMaskedStatus & 0xFF & intcRegs->EnableBasic, 64);
	}

	if(tmp & 0x100) {
		handleRange(intcRegs->Pending1 & intcRegs->Enable1, 0);
	}

	if(tmp & 0x200) {
		handleRange(intcRegs->Pending2 & intcRegs->Enable2, 32);			
	}

	return;

// emit_interrupt:

// 	tmp = ulMaskedStatus - 1;
// 	ulMaskedStatus = ulMaskedStatus ^ tmp;

// 	unsigned long lz = clz(ulMaskedStatus);

// 	//irqNumber = irqNumber - 

// 	//__asm volatile("clz	r7,r5");				// r5 is the ulMaskedStatus register. Leaving result in r6!
// 	//__asm volatile("sub r6,r7");

// 	//uart_string(irqNumber-lz);

// 	if(g_VectorTable[irqNumber-lz].pfnHandler) {
// 		g_VectorTable[irqNumber-lz].pfnHandler(irqNumber, g_VectorTable[irqNumber].pParam);
// 	}

}


static void stubHandler(int nIRQ, void *pParam) {
	/**
	 *	Actually if we get here, we should probably disable the IRQ,
	 *	otherwise we could lock up this system, as there is nothing to 
	 *	ackknowledge the interrupt.
	 **/   
	uart_send(nIRQ);
	uart_send(0x0D);
    uart_send(0x0A);
}

int InitInterruptController() {
	int i;
	for(i = 0; i < BCM2835_INTC_TOTAL_IRQ; i++) {
		g_VectorTable[i].pfnHandler 	= stubHandler;
		g_VectorTable[i].pParam			= (void *) 0;
	}
	return 0;
}



int RegisterInterrupt(int nIRQ, FN_INTERRUPT_HANDLER pfnHandler, void *pParam) {

	irqDisable();
	{
		g_VectorTable[nIRQ].pfnHandler = pfnHandler;
		g_VectorTable[nIRQ].pParam		= pParam;
	}
	irqEnable();
	return 0;
}

int EnableInterrupt(int nIRQ) {

	if(nIRQ >=0 && nIRQ<=31){
		intcRegs->Enable1 |= 1 << nIRQ;
	}else if(nIRQ >=32 && nIRQ<=63){
		intcRegs->Enable2 |= 1 << (nIRQ - 32);
	}else if(nIRQ >= 64 && nIRQ <= 72) {	// Basic IRQ enables
		intcRegs->EnableBasic = 1 << (nIRQ - 64);
	}

	// Otherwise its a GPU interrupt, and we're not supporting those...yet!

	return 0;
}

int DisableInterrupt(int nIRQ) {

	if(nIRQ >=0 && nIRQ<=31){
		intcRegs->Disable1 &= (1 << nIRQ);
	}else if(nIRQ >=32 && nIRQ<=63){
		intcRegs->Disable1 &= (1 << (nIRQ - 32));
	}else if(nIRQ >= 64 && nIRQ <= 72) {
		intcRegs->DisableBasic &= (1 << (nIRQ - 64));
	}

	// I'm currently only supporting the basic IRQs.

	return 0;
}

int EnableInterrupts() {
	irqEnable();
	return 0;
}

int DisableInterrupts() {
	irqDisable();
	return 0;
}
