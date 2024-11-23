/*
 * keypad4x4.h
 *
 *  Created on: Aug 5, 2022
 *      Author: user
 */

#ifndef INC_KEYPAD4X4_H_
#define INC_KEYPAD4X4_H_

#include "main.h"

#define TRANSIENT_DELAY 0 // ms
// MCU WritePins (Outputs)
#define R1_PORT KEYPAD_R1_GPIO_Port
#define R1_PIN  KEYPAD_R1_Pin

#define R2_PORT KEYPAD_R2_GPIO_Port
#define R2_PIN  KEYPAD_R2_Pin

#define R3_PORT KEYPAD_R3_GPIO_Port
#define R3_PIN  KEYPAD_R3_Pin

#define R4_PORT KEYPAD_R4_GPIO_Port
#define R4_PIN  KEYPAD_R4_Pin

// MCU ReadPins (Inputs)
#define C1_PORT KEYPAD_C1_GPIO_Port
#define C1_PIN  KEYPAD_C1_Pin

#define C2_PORT KEYPAD_C2_GPIO_Port
#define C2_PIN  KEYPAD_C2_Pin

#define C3_PORT KEYPAD_C3_GPIO_Port
#define C3_PIN  KEYPAD_C3_Pin

#define C4_PORT KEYPAD_C4_GPIO_Port
#define C4_PIN  KEYPAD_C4_Pin

char read_keypad(void);

void Keypad4x4Keycodes(char key, uint8_t KeyboardReportBuffer[8]);

#endif /* INC_KEYPAD4X4_H_ */
