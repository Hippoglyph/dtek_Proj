#include "tetris.h"
#include <pic32mx.h>
//#include <stdlib.h>

#define TMR3 PIC32_R(0x00A10)
#define PR3 PIC32_R(0x00A20)
#define T3CON PIC32_R(0x00A00)

struct block{
	unsigned int block:16;
	unsigned int posX:4;
	unsigned int posY:6;
	unsigned int rotation:2;
	unsigned int ID:3;
	unsigned int valid:1;
}pack;

struct block currentBlock;

char buttonLock = 0;


unsigned int blockMatrix[7][4];
int* canvas = &blockMatrix[7][4]+sizeof(int);


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
	blockMatrix[2][3] = 0x44C0;
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
	//int *canvas = (int*)malloc((23)*sizeof(int));
	int i;
	for(i = 0; i < 44; i++){
		canvas[i] = 0x1002;
	}
	canvas[44] = ~(0x1002) & 0xFFFF;
	//canvas[0]=~0;
}

/*
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
*/
/*
void printCanvas(){
	//system("clear");
	int i;
	for(i = 0; i < 23;i++){
		int row = canvas[i];
		int j;
		for(j = 0; j < 12;j++){
			printf("%2d", (row >> (12-j)) & 0x1);
		}
		printf("\n");
	}
	printf(">>>>>>>>>>>>\n");
}
*/

int analogRead(char analogPIN){
    AD1CHS = analogPIN << 16;       // AD1CHS<16:19> controls which analog pin goes to the ADC
 
    AD1CON1SET = 0x2;           // Begin sampling
    while( AD1CON1 & 0x2 );      // wait until acquisition is done
    while( ! AD1CON1 & 0x1 );    // wait until conversion done
 
    return ADC1BUF0;                // result stored in ADC1BUF0
}

void adcConfigureManual(){
    AD1CON1CLR = 0x8000;    // disable ADC before configuration
 
    AD1CON1 = 0x00E0;       // internal counter ends sampling and starts conversion (auto-convert), manual sample
    AD1CON2 = 0;            // AD1CON2<15:13> set voltage reference to pins AVSS/AVDD
    AD1CON3 = 0x0f01;       // TAD = 4*TPB, acquisition time = 15*TAD 

    AD1PCFGSET = 0x4;

    AD1CON1SET = 0x8000;
}

void gameInit(){
	canvasInit();
	blockMatrixInit();

	//printBlockMatrix();
	//printCanvas();
	//adcConfigureManual();

	/*
	TRISBSET = 0x4;
	AD1PCFGCLR = 0x4;
	AD1CHSSET = 0x4;
	AD1CON1CLR = 0x700;
	AD1CON2CLR = 0x1;
	AD1CON1SET = 0x80E0;
	//AD1CON1SET = 0x2;
	AD1CON2 = 0x0; //Everything zero
	AD1CON3 = 0x84; //Tad =1/ (40Mhz)*2*(4+1) = 250 nanosecs, SAMC = 1*Tad
	AD1PCFG = 0x4; //Using pin 15,14,13,12,11
	AD1CON1 = 0x80E0; //Enable the ADC, and set auto conversion
	*/
	TMR3 = 0;
	PR3 = 131071/16;
	T3CON = 0x8070;
}

void fetchNewBlock(){
	//srand(time(NULL));
	//int rnd = rand()%7;
	currentBlock.ID = (TMR2+TMR3)%7;
	currentBlock.block = blockMatrix[currentBlock.ID][0];//RNG HÄR
	currentBlock.posX = 5;
	currentBlock.posY = 0;
	currentBlock.valid = 1;
}

int collisionCheck(){
	int i;
	for(i = 0; i < 4; i++){
		if(canvas[currentBlock.posY + i] & (((currentBlock.block >> (3-i)*4))&0xf)<<(10-currentBlock.posX)){
			return 1;
		}
	}
	return 0;
}

void paintBlock(){
	int i;
	for(i = 0;i < 4; i++){
		canvas[currentBlock.posY + i] = canvas[currentBlock.posY + i]|((((currentBlock.block >> (3-i)*4))&0xf)<<(10-currentBlock.posX));
	}
}

void eraseBlock(){
	int i;
	for(i = 0;i < 4; i++){
		canvas[currentBlock.posY + i] = canvas[currentBlock.posY + i]&~((((currentBlock.block >> (3-i)*4))&0xf)<<(10-currentBlock.posX));
	}
}

void clearRow(){
	int i;
	for(i = 0; i < 4; i++){
		if (canvas[currentBlock.posY + i] == 0x1ffe){
			int j;
			for(j = currentBlock.posY + i; j > 0; j--){
				canvas[j] = canvas[j-1];
			}
			canvas[0] = 0x1002;
		}
	}
}

void moveRight(){
	if(currentBlock.posX >= 10)
		return;
	eraseBlock();
	currentBlock.posX++;
	if (collisionCheck())
		currentBlock.posX--;
	paintBlock();
}

void moveLeft(){
	if(currentBlock.posX <= 0)
		return;
	eraseBlock();
	currentBlock.posX--;
	if (collisionCheck())
		currentBlock.posX++;
	paintBlock();
	//printCanvas();
	//printf("Blockpos: %d\n", currentBlock.posX);
}

void rotateRight(){
	eraseBlock();
	if (currentBlock.rotation == 3)
		currentBlock.rotation = 0;
	else
		currentBlock.rotation++;
	currentBlock.block = blockMatrix[currentBlock.ID][currentBlock.rotation];
	if (collisionCheck()){
		if(currentBlock.rotation == 0)
			currentBlock.rotation = 3;
		else
			currentBlock.rotation--;
	}
	currentBlock.block = blockMatrix[currentBlock.ID][currentBlock.rotation];
	paintBlock();
}

void rotateLeft(){
	eraseBlock();
	if (currentBlock.rotation == 0)
		currentBlock.rotation = 3;
	else
		currentBlock.rotation--;
	currentBlock.block = blockMatrix[currentBlock.ID][currentBlock.rotation];
	if (collisionCheck()){
		if(currentBlock.rotation == 3)
			currentBlock.rotation = 0;
		else
			currentBlock.rotation++;
	}
	currentBlock.block = blockMatrix[currentBlock.ID][currentBlock.rotation];
	paintBlock();
}

int* gameIOUpdate(char in){
	if(!currentBlock.valid)
		return canvas;
	if (in == 0)
		moveLeft();
	else if (in == 1)
		moveRight();
	else if (in == 2)
		rotateLeft();
	else
		rotateRight();
	return canvas;
}

int* gameTimeUpdate(){
	//int speed = analogRead(4);
	//PR3 = (speed*100053)%30000;
	if(!currentBlock.valid){
		fetchNewBlock();
		if(collisionCheck())
			gameInit();
	}
	eraseBlock();
	currentBlock.posY++;
	if(collisionCheck()){
		currentBlock.posY--;
		currentBlock.valid = 0;
		paintBlock();
		clearRow();
	}
	else
		paintBlock();
	return canvas;
	//printCanvas();

	//printf("BlockValid: %d", (currentBlock.valid & 1));
}