//Routines that get the system date and time and produce them as a formatted strings
//
//	GetSystemTime: get time from system
//	TimeToString: convert the time to a string in 24-h digital clock format (00:00:00)
//	GetTime: return the current time in a string format (00:00:00)
//	GetSystemDate: get date from system 
//	DateToString: convert the date to a string in format (dd/mm/yyyy)
//	GetDate: return the current date in format (dd/mm/yyyy)
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef TimeUtilsH
#define TimeUtilsH

#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"		//Gets date and times
#include <fstream>			//saving data to and from files
#include <ctime>		//for time routines
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

void GetSystemTime(int& hrs, int& mins, int& secs); 
//get time from system

string TimeToString(int h, int m, int s); 
//convert the time to a string in 24-h digital clock format (00:00:00)

string GetTime(); 
//return the current time in a string format

void GetSystemDate(int& day, int& month, int& year); 
//get date from system

string DateToString(int day, int month, int year);
//convert the date to a string in format (dd/mm/yyyy)

string GetDate(); 
//return the current date in a string format

#endif