.extern __bss_start
.extern __bss_end
.extern OS_CPU_IRQ_ISR//to be modified
.extern DisableInterrupts//to be modified
.extern main
	.section .init
	.globl _start
;; 
_start:
	;@ All the following instruction should be read as:
	;@ Load the address at symbol into the program counter.
	
	ldr	pc,reset_handler		;@ 	Processor Reset handler 		-- we will have to force this on the raspi!
	;@ Because this is the first instruction executed, of cause it causes an immediate branch into reset!
	
	ldr pc,undefined_handler	;@ 	Undefined instruction handler 	-- processors that don't have thumb can emulate thumb!
    ldr pc,swi_handler			;@ 	Software interrupt / TRAP (SVC) -- system SVC handler for switching to kernel mode.
    ldr pc,prefetch_handler		;@ 	Prefetch/abort handler.
    ldr pc,data_handler			;@ 	Data abort handler/
    ldr pc,unused_handler		;@ 	-- Historical from 26-bit addressing ARMs -- was invalid address handler.
    ldr pc,irq_handler			;@ 	IRQ handler
    ldr pc,fiq_handler			;@ 	Fast interrupt handler.

	;@ Here we create an exception address table! This means that reset/hang/irq can be absolute addresses
reset_handler:      .word reset
undefined_handler:  .word undefined_instruction
swi_handler:        .word swi//to be modified
prefetch_handler:   .word prefetch_abort
data_handler:       .word data_abort
unused_handler:     .word unused
irq_handler:        .word OS_CPU_IRQ_ISR//irqHandler to be modified
fiq_handler:        .word fiq

reset:
<<<<<<< HEAD
	;@ important , copy for FreeRTOS with raspberry pi 3
	;@ change to hyperVistor mode 
	mrs r0,cpsr
    bic r0,r0,#0x1F
    orr r0,r0,#0x13
    msr spsr_cxsf,r0
    add r0,pc,#4
    msr ELR_hyp,r0
    eret

=======
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	;@	In the reset handler, we need to copy our interrupt vector table to 0x0000, its currently at 0x8000

	mov r0,#0x8000								;@ Store the source pointer
    mov r1,#0x0000								;@ Store the destination pointer.

	;@	Here we copy the branching instructions
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Load multiple values from indexed address. 		; Auto-increment R0
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Store multiple values from the indexed address.	; Auto-increment R1

	;@	So the branches get the correct address we also need to copy our vector table!
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Load from 4*n of regs (8) as R0 is now incremented.
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}			;@ Store this extra set of data.


<<<<<<< HEAD
	;@ Set up the various STACK pointers for different CPU modes
	;@ Every cpu mode user self sp reg,so need set different position 
    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	;@ IRQ mode sp
=======
	;@	Set up the various STACK pointers for different CPU modes
    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
<<<<<<< HEAD
	;@ set FIQ mode sp
    mov r0,#0xD1
   	msr cpsr_c,r0
  	mov sp,#0x4000

    ;@ (PSR_SYS_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
	;@ set svc mode sp
    mov r0,#0xD3
    msr cpsr_c,r0
	mov sp,#0x8000000
=======
//    mov r0,#0xD1
//   msr cpsr_c,r0
//  mov sp,#0x4000

    ;@ (PSR_SYS_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xDF
    msr cpsr_c,r0
	mov sp,#0x4000
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724

	ldr r0, =__bss_start
	ldr r1, =__bss_end

	mov r2, #0

zero_loop:
	cmp 	r0,r1
<<<<<<< HEAD
	;@ it instruction like if ,it can put four instruction in if
	;@ lt is mean less than
	;@ there compare bss_start is less than bss_end 
	it		lt
	strlt	r2,[r0], #4
	blt		zero_loop
	bl 		DisableInterrupts //to be modified
	b main	;@ We're ready?? Lets start main execution!
=======
	it		lt
	strlt	r2,[r0], #4
	blt		zero_loop

	bl 		DisableInterrupts//to be modified
	
	
	;@ 	mov	sp,#0x1000000
	b main									;@ We're ready?? Lets start main execution!
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
	.section .text

undefined_instruction:
	b undefined_instruction

swi:
	b swi

prefetch_abort:
	b prefetch_abort

data_abort:
	b data_abort

unused:
	b unused

fiq:
	b fiq
	
hang:
	b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
<<<<<<< HEAD
    bx lr
=======
    bx lr
>>>>>>> d8ff05da3870937dbbeac3718388976c9f504724
