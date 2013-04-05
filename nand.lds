SECTIONS{
	first 0x00000000 : AT(0) {Start.o init.o nand.o}	
	second 0x30000000 : AT(4096) {led.o} 
}
