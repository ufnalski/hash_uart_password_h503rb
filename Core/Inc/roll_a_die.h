/*
 * charlie_die.h
 *
 *  Created on: Aug 8, 2023
 *      Author: user
 */

#ifndef INC_ROLL_A_DIE_H_
#define INC_ROLL_A_DIE_H_

#include "main.h"

uint8_t RawRandomTo6FaceDie(uint32_t raw_random_value);
void AddToDieStats(uint32_t *die_face_stats, uint8_t die_face);


#endif /* INC_ROLL_A_DIE_H_ */
