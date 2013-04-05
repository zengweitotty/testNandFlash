@***********************************
@File: Start.s
@Description: first boot section
@***********************************
.text
.global _start
_start:
	ldr sp,=4096 @use C function,so need stack
	bl disable_watchdog
	bl memsetup
	bl nandinit
	ldr sp,=0x34000000
	ldr pc,=0x30000000
halt_loop:
	b halt_loop
