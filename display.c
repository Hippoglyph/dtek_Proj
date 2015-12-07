#include "customPic32.h"
#include "tetris.h"
#include <stdint.h>



void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t OLED_send(uint8_t data) {
	while(!OLED_SPI2BUF_EMTY);
	OLED_SPI2BUF = data;
	while(!OLED_SPI2BUF_FULL);
	return OLED_SPI2BUF;
}

void displayInit(){

	OLED_COMMAND;
	quicksleep(10);
	OLED_VBAT_DISABLE;
	OLED_VDD_ENABLE;
	quicksleep(1000000);
	
	OLED_send(0xAE);
	OLED_RESET_ENABLE;
	quicksleep(10);
	OLED_RESET_DISABLE;
	quicksleep(10);
	
	OLED_send(0x8D);
	OLED_send(0x14);
	
	OLED_send(0xD9);
	OLED_send(0xF1);
	OLED_send(0xA7);
	
	OLED_VBAT_ENABLE;
	quicksleep(10000000);
	
	OLED_send(0xA0);
	OLED_send(0xC8);
	
	OLED_send(0xDA);
	OLED_send(0x20);
	
	OLED_send(0xAF);
	OLED_send(0xA4);
	OLED_send(0x20);
	OLED_send(0x1);


	OLED_send(0x21);
	OLED_send(0x0);
	OLED_send(127);

	OLED_send(0x22);
	OLED_send(0x0);
	OLED_send(3);

	OLED_DATA;
	int i;
	//
	for(i = 0; i < (4*128);i++)
	OLED_send(0x00);
}

char mirror(char cell){
	char newcell = 0;
	int i;
	for(i = 0; i < 8; i++){
		newcell |= ((cell>>(7-i))& 0x1)<<i;
	}
	return newcell;
}

char screen = 1;
void invertScreen(){
	OLED_COMMAND;
	if (screen){
		screen = 0;
		OLED_send(0xA6);
	}
	else{
		screen = 1;
		OLED_send(0xA7);
	}
}

void paintCanvas(int* canvas){
	int x, y;
	int i, j;
	char cell = 0;

	OLED_COMMAND;

	OLED_send(0x21);
	OLED_send(0x0);
	OLED_send(127);

	OLED_send(0x22);
	OLED_send(0);
	OLED_send(3);

	OLED_DATA;

	//OLED_send(mirror(0xA9));
	
	for(y = 2; y < 45; y++){
		int paintRow = 0;
		for(x = 9; x >= 0; x--){
			int block = (((canvas[y]>>2)>>x) & 0x1);
			paintRow |= ((block<<2|block<<1|block)<<(x*3))<<1;
		}
		paintRow |= 0x80000001;
		
		for(j = 0; j<3; j++){
			for(i = 0; i < 4; i++){
				cell = paintRow>>((3-i)*8)&0xFF;

				OLED_send(mirror(cell));
			}
		}
	}

	//... ... ... ... ... ... ... ... ... ...
	//111 000 000 111 000 000 111 000 111 000
	// 1001001010
	// 0000000111
	// 111 000 000 111 000 000 111 000 111 000

	//x |= x<<1 | x<<2	3x3
}