#include "tetris.h"
#include <pic32mx.h>

int notes1[] = {0x08084020, 0x10102040, 0x80008020, 0x08081020,
0x40004020, 0x10100808, 0x20208000, 0x80804020,
0x10001004, 0x01010204, 0x08000820, 0x08081020,
0x40004020, 0x10100808, 0x20208000, 0x80800000};

int notes2[] = {0x04040404, 0x20202020, 0x10101010, 0x40404040,
0x20202020, 0x80808080, 0x81818181, 0x40400000, 
0x40404042, 0x02020201, 0x01010104, 0x04040401, 
0x01004040, 0x10101010, 0x30303030, 0x00000000};

int tickCounter = 0;
int noteSet = 0;
int shiftCounter = 3;
int* currentTrack = notes1;

void musicInit(){
	TRISE &= ~0xff;
	TMR2 = 0;
	PR2 = 31250;
	T2CON = 0x8070;
}

int trackId = 0;
void nextTrack(){
	trackId = (trackId+1) % 3;

	if(trackId == 0 || trackId == 1)
		currentTrack = notes1;
	else
		currentTrack = notes2;
}

void musicUpdate(){
	if(IFS(0) & 0x100){
		IFS(0) &= ~0x100;
		tickCounter++;
	}
	else
		return;

	if(tickCounter <= 1)
		return;

	tickCounter = 0;

	if(shiftCounter < 0){
		shiftCounter = 3;
		noteSet++;
		//if((sizeof(currentTrack) / sizeof(currentTrack[0])) <= noteSet){
		if(noteSet >= 16){
			noteSet = 0;
			nextTrack();
		}
	}

	PORTE = (currentTrack[noteSet]>>(shiftCounter*8)) & 0xff;

	shiftCounter--;
	return;
}