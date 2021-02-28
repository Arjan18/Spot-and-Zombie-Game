/*
**A simple library of random numbers routines  
*/

#ifndef RandomUtilsH
#define RandomUtilsH 

#include <iostream>
#include <stdlib.h>		//for rand and srand
#include <ctime>		//for time used in random number routines
#include <cassert>		//for assert
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"		//Gets date and times
#include <fstream>			//saving data to and from files
//Seed() - seed the random number generator from current system time 
void Seed();

//Random(max) - produce a random number in range [1..max]
//pre-condition: max > 0
int Random(int max); 

#endif