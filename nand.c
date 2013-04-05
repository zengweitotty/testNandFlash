/*
    File Name:      nand.c
    Author:         zengweitotty
    Version:        V1.0
    Data:           2013/04/05
    Email:          zengweitotty@gmail.com
    Description     Using S3C2440 nandflash controller
*/

#define NFCONF	(*(volatile unsigned short *)0x4E000000)
#define NFCONT	(*(volatile unsigned short *)0x4E000004)
#define NFCMMD	(*(volatile unsigned short *)0x4E000008)
#define NFADDR	(*(volatile unsigned char *)0x4E00000C)
#define NFDATA	(*(volatile unsigned char *)0x4E000010)
#define NFSTAT	(*(volatile unsigned char *)0x4E000020)

#define TACLS	0
#define TWRPH0	3
#define TWRPH1	0

void nand_reset(void);
void nand_select_chip(void);
void nand_deselect_chip(void);
void write_cmd(unsigned short cmd);
void write_addr(unsigned long addr);
void wait_idle(void);
unsigned char read_data(void);
void nandinit(void);

void nand_reset(void){
	nand_select_chip();
	write_cmd(0xff);
	wait_idle();
	nand_deselect_chip();
}
void nand_select_chip(void){
	NFCONT &= ~(0x1 << 1);		
}
void nand_deselect_chip(void){
	NFCONT |= 0x1 << 1;		
}
void write_cmd(unsigned short cmd){
	NFCMMD = cmd;		
}
void write_addr(unsigned long addr){
	int i = 0;
	unsigned long col = 0;
	unsigned long page = 0;
	col = addr & 0x07ff;
	page = addr >> 11;
	/*A0 -- A7*/
	NFADDR = col;
	for(i = 0;i < 10;i++);
	/*A8 -- A11*/
	NFADDR = (col >> 8) | 0x0f;
	for(i = 0;i < 10;i++);
	/*A12 -- A19*/
	NFADDR = page;
	for(i = 0;i < 10;i++);
	/*A20 -- A27*/
	NFADDR = page >> 8;
	for(i = 0;i < 10;i++);
	/*A28 -- A29*/
	NFADDR = (page >> 16 | 0x03);
	for(i = 0;i < 10;i++);
}
void wait_idle(void){
	int i = 0;
	while(!NFSTAT & 0x01){
		for(i = 0;i < 10;i++){
			;;		
		}	
	}		
}
unsigned char read_data(void){
	return NFDATA;		
}

void nandinit(void){
	int i = 0;
	int j = 0;
	volatile unsigned char* pDst = (volatile unsigned char*)0x30000000;
	NFCONF = (TACLS << 12 | TWRPH0 << 8 | TWRPH1 << 4 );
	NFCONT = (0x01 << 4 | 0x01 << 1 | 0x01 << 0);
	nand_reset();
	nand_select_chip();
	for(i = 0;i < 4096;){
		write_cmd(0x00);
		write_addr(i + 4096);
		write_cmd(0x30);
		wait_idle();
		for(j = 0;j < 2048;i++,j++){
			*pDst++ = read_data();
		}
	}
	nand_deselect_chip();
}
