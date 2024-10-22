#include "emb2calc.h"
#include "oled.h"
#include "stm32f4xx_hal.h"

#include <stdlib.h>

static CALC_t calc = {0, 0, '+', 0, 1};

void calcReset(void) {
	calc.num1 = 0;
	calc.num2 = 0;
	calc.operation = '+';
	calc.stage = 0;
	calc.layout = 1;
}

void printCalc(void) {
	char num1[18] = "";
	char num2[18] = "";

	itoa(calc.num1, num1, 10);
	itoa(calc.num2, num2, 10);

	oled_Fill(Black);
	oled_SetCursor(0, 0);

	oled_WriteString(num1, Font_7x10, White);
	oled_NextLine(Font_7x10);
	oled_WriteChar(calc.operation, Font_7x10, White);
	oled_NextLine(Font_7x10);
	oled_WriteString(num2, Font_7x10, White);

	oled_UpdateScreen();
}

void printRes(void) {

	char num1[18] = "";
	itoa(calc.num1, num1, 10);

	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_WriteString(num1, Font_7x10, White);
	oled_UpdateScreen();

	HAL_Delay(3000);
}

uint8_t getLayout(void) {
	return calc.layout;
}

void pErr(char* str) {
	oled_Fill(Black);
	oled_SetCursor(0, 0);
	oled_WriteString(str, Font_7x10, White);
	oled_UpdateScreen();
	HAL_Delay(1000);
}

void opErr(void) {
	pErr("Enter first number!");
}

void keyPressed(char ch) {
	switch (ch) {
	case '+':
		calc.operation = ch;
		calc.num1 = calc.num1 + calc.num2;
		calc.num2 = 0;
		calc.stage = 1;
		break;
	case '-':
		calc.operation = ch;
		calc.num1 = calc.num1 - calc.num2;
		calc.num2 = 0;
		calc.stage = 1;
		break;
	case '*':
		calc.operation = ch;
		if (calc.stage == 1) {
			calc.num1 = calc.num1 * calc.num2;
			calc.num2 = 0;
		}
		calc.stage = 1;
		break;
	case '/':
		calc.operation = ch;
		if (calc.stage == 1) {
			if (calc.num2 == 0) {
				pErr("Dividing by zero is not allowed!");
				break;
			}
			calc.num1 = calc.num1 / calc.num2;
			calc.num2 = 0;
		}
		calc.stage = 1;
		break;
	case '#':
		calc.layout = calc.layout ? 0 : 1;
		break;
	case '<':
		if (!calc.stage) {
			calc.num1 = calc.num1 / 10;
		} else {
			calc.num2 = calc.num2 / 10;
		}
		break;
	case 'C':
		calcReset();
		break;
	case '.':
		break;
	case '=':
		keyPressed(calc.operation);
		printRes();
		break;
	default:
		if (!calc.stage) {
			calc.num1 = calc.num1 * 10 + (ch - '0');
		} else {
			calc.num2 = calc.num2 * 10 + (ch - '0');
		}
	}
}
