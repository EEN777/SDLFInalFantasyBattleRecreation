#pragma once

#include <stdlib.h>
#include <time.h>

inline int Roll(int min, int max)
{
	srand((unsigned int)time(0));
	int roll = rand() % max;
	if (roll < min)
	{
		roll = min;
	}
	if (roll > max)
	{
		roll = max;
	}

	return roll;
}