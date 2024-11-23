/*
 * charlie_die.c
 *
 *  Created on: Aug 8, 2023
 *      Author: user
 */

#include "roll_a_die.h"

uint8_t RawRandomTo6FaceDie(uint32_t raw_random_value)
{
	if (raw_random_value < 0x20000000)
	{
		return 0;
	}
	else if (raw_random_value < 0x40000000)
	{
		return 1;
	}
	else if (raw_random_value < 0x60000000)
	{
		return 2;
	}
	else if (raw_random_value < 0x80000000)
	{
		return 3;
	}
	else if (raw_random_value < 0xA0000000)
	{
		return 4;
	}
	else if (raw_random_value < 0xC0000000)
	{
		return 5;
	}
	else if (raw_random_value < 0xE0000000)
	{
		return 6;
	}
	else
	{
		return 7;
	}
}

void AddToDieStats(uint32_t *die_face_stats, uint8_t die_face)
{
	if ((die_face >= 0) && (die_face <= 7))
	{
		*(die_face_stats + die_face) += 1;
	}
}
