
#include "stdafx.h"

#include "random.h"

/*****************************************************************************/
/*                                                                           */
/*  randomnation()   Generate a random number between 0 and `choices' - 1.   */
/*                                                                           */
/*  This is a simple linear congruential random number generator.  Hence, it */
/*  is a bad random number generator, but good enough for most randomized    */
/*  geometric algorithms.                                                    */
/*                                                                           */
/*****************************************************************************/
static unsigned long RandomSeed = 1;                 /* Current random number seed. */
static unsigned int Choices=100;

void  SetRandom(unsigned long randomSeed, unsigned int choices)
{
	RandomSeed = randomSeed;
	if( choices > 0 ) 
		Choices = choices;

	RandomSeed %= Choices;
}

unsigned long  Randomnation()
{
	RandomSeed = (RandomSeed * 1366l + 150889l) % 714025l;
	return RandomSeed / (714025l / Choices + 1);
}


unsigned long  randomnation(unsigned int choices)
{
	Choices = choices;

	RandomSeed = (RandomSeed * 1366l + 150889l) % 714025l;

	return RandomSeed / (714025l / Choices + 1);
}
