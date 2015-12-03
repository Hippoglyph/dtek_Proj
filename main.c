#include "tetris.h"
#include <pic32mx.h>

int main(void) {

	/* Set up peripheral bus clock */
        
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
     SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	/*FOR BUTTONS*/

	TRISFSET = 0x2;
	TRISDSET = 0xe0;

	
	displayInit();
	musicInit();
	gameInit();
	char flag = 1;
	char cool = 0;
	int coolCount = 0;
	while (1){
		if(getBtn()){
			if(flag==1){
				flag=0;
				cool=0;
				coolCount = 0;
				if(getBtn() == 0x1)
					paintCanvas(gameIOUpdate(0x3));
				else if(getBtn() == 0x2)
					paintCanvas(gameIOUpdate(0x2));
				else if(getBtn() == 0x4)
					paintCanvas(gameIOUpdate(0x1));
				else if(getBtn() == 0x8)
					paintCanvas(gameIOUpdate(0x0));
			}
		}
		else
			cool = 1;
		if(cool){
			coolCount++;
			if(coolCount>10000){
				cool=0;
				coolCount = 0;
				flag=1;
			}
		}

		if(IFS(0) & 0x1000){
			IFS(0) &= ~0x1000;
			paintCanvas(gameTimeUpdate());
		}
		musicUpdate();
	}
	return 0;
}

int getBtn(){
	return ((PORTD & 0xe0)>>4)|((PORTF & 0x2)>>1)&0xF;
}