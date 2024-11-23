/*
 * keypad4x4.c
 *
 *  Created on: Aug 5, 2022
 *      Author: user
 */

#include "keypad4x4.h"
#include "main.h"

// #define WAIT_UNTIL_RELEASED

char read_keypad(void)
{
	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	HAL_Delay(TRANSIENT_DELAY);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '1';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '2';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '3';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return 'A';
	}

	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	HAL_Delay(TRANSIENT_DELAY);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '4';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '5';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '6';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return 'B';
	}

	/* Make ROW 3 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	HAL_Delay(TRANSIENT_DELAY);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '7';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '8';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '9';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return 'C';
	}

	/* Make ROW 4 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);  // Pull the R4 High
	HAL_Delay(TRANSIENT_DELAY);

	if (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C1_PORT, C1_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '*';
	}

	if (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C2_PORT, C2_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '0';
	}

	if (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C3_PORT, C3_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return '#';
	}

	if (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
#ifdef WAIT_UNTIL_RELEASED
		while (!(HAL_GPIO_ReadPin(C4_PORT, C4_PIN)))
		{
			;   // wait till the button is pressed
		}
#endif
		return 'D';
	}

	return '\0';

}

void Keypad4x4Keycodes(char key, uint8_t KeyboardReportBuffer[8])
{
	// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-14.html
	// https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
	switch (key)
	{
	case '1':
		KeyboardReportBuffer[2] = 30;
		break;
	case '2':
		KeyboardReportBuffer[2] = 31;
		break;
	case '3':
		KeyboardReportBuffer[2] = 32;
		break;
	case '4':
		KeyboardReportBuffer[2] = 33;
		break;
	case '5':
		KeyboardReportBuffer[2] = 34;
		break;
	case '6':
		KeyboardReportBuffer[2] = 35;
		break;
	case '7':
		KeyboardReportBuffer[2] = 36;
		break;
	case '8':
		KeyboardReportBuffer[2] = 37;
		break;
	case '9':
		KeyboardReportBuffer[2] = 38;
		break;
	case '0':
		KeyboardReportBuffer[2] = 39;
		break;
	case 'A':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 4;
		break;
	case 'B':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 5;
		break;
	case 'C':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 6;
		break;
	case 'D':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 7;
		break;
	case '#':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 32;
		break;
	case '*':
		KeyboardReportBuffer[0] = 0x02; // Left Shift
		KeyboardReportBuffer[2] = 37;
		break;
	default:
		KeyboardReportBuffer[0] = 0x00;
		KeyboardReportBuffer[2] = 0;
		break;
	}

}
