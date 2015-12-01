#include "tetris.h"

int main(void) {
	/*
	musicInit();
	while (1){
		musicUpdate();
	}
	return 0;
	*/
	gameInit();
	while(1){
		gameTimeUpdate();
		sleep(1);
	}

}