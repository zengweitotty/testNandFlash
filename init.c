/*
    File Name:      init.c
	Author:         zengweitotty
    Version:        V1.0
    Data:           2013/04/01
    Email:          zengweitotty@gmail.com
    Description     Initialize TQ2440 hardware
*/

#define WTCON	(*(volatile unsigned long *)0x53000000)
#define MEM_CTL_BASE	((volatile unsigned long *)0x48000000)

void disable_watchdog(void){
	WTCON = (unsigned long)0x00;
}

void memsetup(void){
	//copy from TQ2440 u-boot,to initialize SDRAM
	unsigned long const mem_cfg_val[] = { 0x22011110,/*BWSCON*/
											0x00000700,/*BANKCON0*/
											0x00000700,/*BANKCON1*/
											0x00000700,/*BANKCON2*/
											0x00001F4C,/*BANKCON3*/
											0x00000700,/*BANKCON4*/
											0x00000700,/*BANKCON5*/
											0x00018005,/*BANKCON6 with SDRAM*/
											0x00018005,/*BANKCON7 with SDRAM*/
											0x008E04F4,/*REFRESH*/
											0x00000032,/*BANKSIZE*/
											0x00000030,/*MRSRB6*/
											0x00000030,/*MRSRB7*/
										};
	int index = 0;
	volatile unsigned long *p = (volatile unsigned long *)0x48000000;
	for(index = 0;index < 13;index++){
		p[index] = mem_cfg_val[index];	//initialize SDRAM
	}
}
