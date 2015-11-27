#include "tetris.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct block{
	unsigned int block:16;
	unsigned int posX:4;
	unsigned int posY:5;
	unsigned int rotation:2;
	unsigned int valid:1;
}pack;

struct block currentBlock;

int *canvas = NULL;
unsigned int blockMatrix[4][7];


void blockMatrixInit(){
	blockMatrix[0][0] = 0x4E00;
	blockMatrix[0][1] = 0x4640;
	blockMatrix[0][2] = 0x0E40;
	blockMatrix[0][3] = 0x4C40;
	blockMatrix[1][0] = 0x6600;
	blockMatrix[1][1] = 0x6600;
	blockMatrix[1][2] = 0x6600;
	blockMatrix[1][3] = 0x6600;
	blockMatrix[2][0] = 0x8E00;
	blockMatrix[2][1] = 0x6440;
	blockMatrix[2][2] = 0x0E20;
	blockMatrix[2][3] = 0x4460;
	blockMatrix[3][0] = 0x2E00;
	blockMatrix[3][1] = 0x4460;
	blockMatrix[3][2] = 0x0E80;
	blockMatrix[3][3] = 0xC440;
	blockMatrix[4][0] = 0x6C00;
	blockMatrix[4][1] = 0x4620;
	blockMatrix[4][2] = 0x06C0;
	blockMatrix[4][3] = 0x8C40;
	blockMatrix[5][0] = 0xC600;
	blockMatrix[5][1] = 0x2640;
	blockMatrix[5][2] = 0x0C60;
	blockMatrix[5][3] = 0x4C80;
	blockMatrix[6][0] = 0x0F00;
	blockMatrix[6][1] = 0x2222;
	blockMatrix[6][2] = 0x00F0;
	blockMatrix[6][3] = 0x4444;
	
}

void canvasInit(){
	canvas = (int*)malloc((23)*sizeof(int));
	int i;
	for(i = 0; i < 22; i++){
		canvas[i] = 0x801;
	}
	canvas[22] = ~(0x801) & 0xFFF;
}

void printBlockMatrix(){
	int i;
	for(i = 0; i < 7;i++){
		int j;
		for(j = 0; j < 4; j++){
			int block = blockMatrix[i][j];
			int n;
			for(n = 0; n < 4; n++){
				int row = block >> 4*(3-n);
				row = row & 0xF;
				printf("%d%d%d%d \n", (row & 0x8) >> 3, (row & 0x4) >> 2, (row & 0x2) >> 1, row & 0x1);  
			}
			printf("________\n");
		}
		printf(">>>>>>>>>>>>\n");
	}
}

void printCanvas(){
	int i;
	for(i = 0; i < 23;i++){
		int row = canvas[i];
		int j;
		for(j = 0; j < 12;j++){
			printf("%2d", (row >> (11-j)) & 0x1);
		}
		printf("\n");
	}
}

void gameInit(){
	canvasInit();
	blockMatrixInit();
	printBlockMatrix();
	printCanvas();
	//free(canvas);
}

void fetchNewBlock(){
	srand(time(NULL));
	int rnd = rand()%7;
	currentBlock.block = blockMatrix[rnd][0];
	currentBlock.posX = 4;
	currentBlock.posY = 0;
	currentBlock.valid = 1;
}


void gameTimeUpdate(){
	if(!currentBlock.valid){
		fetchNewBlock();
	}
	
}