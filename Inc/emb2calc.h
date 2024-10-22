#ifndef emb2calc
#define emb2calc

#include "stm32f4xx_hal.h"

typedef struct {
	uint32_t num1;
	uint32_t num2;
	char operation;
	uint8_t stage;
	uint8_t layout;
} CALC_t;

void printCalc(void);
uint8_t getLayout(void);
void keyPressed(char ch);
void calcReset(void);
void calcOut(void);


#endif
