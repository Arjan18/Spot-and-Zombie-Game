//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 26 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>

using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"		//Gets date and times
#include <fstream>			//saving data to and from files

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(20);    	//horizontal dimension
const int  SIZEY(14);		//vertical dimension

//defining symbols used for display of the grid and content
const char SPOT('@');   	//spot
const char ZOMBIE('Z');     //zombie
const char HOLE('0');		//hole
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char PILL('*');		//pill

//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow

//defining the other command letters
const char QUIT_U('Q');		//to end the game
const char QUIT_L('q');		//to end the game  

const int HOLES_COUNT = 12; 
const int PILLS_COUNT = 8;
const int ZOMBIES_COUNT = 4;

struct Item {
	int x, y;
	char symbol;
};

struct Zombie					//for the zombie item
{
	int default_x, default_y;
	int x, y;
	char symbol;
	bool alive;
	bool freeze;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot, Zombie zombies[]);                                                                                                         
	void paintGame(const char g[][SIZEX], string mess, const int lives, const int pre_score, string name, int pills, int zombies);                                                                
	bool wantsToQuit(const int key);                                                                                                                                                             
	bool isArrowKey(const int k);                                                                                                                                                                
	int  getKeyPress();                                                                                                                                                                         
	void updateGameData(const char g[][SIZEX], char maze[][SIZEX], Item& spot, const int key, string& mess, bool &cheated, int& lives, int& pills, bool &kill, bool &freeze, Zombie zombies[]); 
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot, Zombie zombies[]);                                                                                                 
	int zombiesAlive(const Zombie zombies[]);
	void setInitialMazeStructure(char maze[][SIZEX]);                                                                                                                                           
	void saveScore(string name, int lives);
	void loadScore(string name, int &pre_score);
	void endProgram();                                                                                                                                                                          

	//local variable declarations                                                                                                                                                              
	char grid[SIZEY][SIZEX];			//grid for display 
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol

	int pre_score = -1;
	int lives = 5;
	int pills = 8;
	int walls = 4;
	bool cheated = false;
	bool kill = false;
	bool freeze = true;
	Zombie zombies[ZOMBIES_COUNT];

	string name = "";
	string message("LET'S START...");	//current message to player

	//action...
	Seed();								//seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	
	system("color 80");
	initialiseGame(grid, maze, spot, zombies);	//initialise grid (incl. walls and spot)
	loadScore(name, pre_score); 
	paintGame(grid, message, lives, pre_score, name, pills, zombiesAlive(zombies));			//display game info, modified grid and messages
	int key;							//current key selected by player
	do {
		//TODO: command letters should not be case sensitive
		key = getKeyPress(); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			updateGameData(grid, maze, spot, key, message, cheated, lives, pills, kill, freeze, zombies);		//move spot in that direction
			updateGrid(grid, maze, spot, zombies);					//update grid information
		}
		else
			message = "INVALID KEY!";	//set 'Invalid key' message
		paintGame(grid, message, lives, pre_score, name, pills, zombiesAlive(zombies));		//display game info, modified grid and messages
	} while (!wantsToQuit(key) && lives > 0 && !(pills == 0 && zombiesAlive(zombies) == 0));		//while user does not want to quit and has atleast one life left
	
	if (pills == 0 && zombiesAlive(zombies) == 0) 
		if (lives > pre_score && !cheated)        
			saveScore(name, lives);               
	
	endProgram();						//display final message
	return 0;
}

int zombiesAlive(const Zombie zombies[])     
{
	int count = 0;
	for (int i(0); i < ZOMBIES_COUNT; i++)
	{
		if (zombies[i].alive)
			count++;
	}
	if (count == 0)
		count = 0;
	return count;                          ///////////////////////////////////////////////////////////
}


void saveScore(string name, int lives) /////////////////////////////////////////////////
{
	ofstream fout(name + ".txt");      //////////////////////////////////////////////
	fout << lives;                     //////////////////////////////////////////////
	fout.close();                                                                                    ///////////////////////////////////////////////
}

void loadScore(string name, int &pre_score)                                                        ///////////////////////////////////////////////
{
	ifstream fin(name + ".txt");                                                                  ///////////////////////////////////////////////
	if (fin)                                                                                     ///////////////////////////////////////////////
	{
		fin >> pre_score;                                                                       ///////////////////////////////////////////////
	}
	fin.close();                                                                               ////////////////////////////////////////////////
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Zombie zombies[])
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(const char maze[][SIZEX], Item& spot);
	void setZombiesInitialCoordinates(Zombie zombies[]);                                        /////////////////////////////////////////////////
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b, Zombie zombies[]);

	setInitialMazeStructure(maze);	 //initialise maze
	setZombiesInitialCoordinates(zombies);                                                      ///////////////////////////////////////////////////
	setSpotInitialCoordinates(maze, spot);

	updateGrid(grid, maze, spot, zombies);		//prepare grid
}

void setZombiesInitialCoordinates(Zombie zombies[])
{
	zombies[0].default_x = 1; zombies[0].x = 1;                                                 /////////////////////////////////////////////////////
	zombies[0].default_y = 1; zombies[0].y = 1;

	zombies[1].default_x = SIZEX - 2; zombies[1].x = SIZEX - 2;
	zombies[1].default_y = 1; zombies[1].y = 1;

	zombies[2].default_x = 1; zombies[2].x = 1;
	zombies[2].default_y = SIZEY - 2; zombies[2].y = SIZEY - 2;

	zombies[3].default_x = SIZEX - 2; zombies[3].x = SIZEX - 2;
	zombies[3].default_y = SIZEY - 2; zombies[3].y = SIZEY - 2;

	zombies[0].symbol = zombies[1].symbol = zombies[2].symbol = zombies[3].symbol = ZOMBIE;
	zombies[0].alive = zombies[1].alive = zombies[2].alive = zombies[3].alive = true;
	zombies[0].freeze = zombies[1].freeze = zombies[2].freeze = zombies[3].freeze = false;             //////////////////////////////////////////////////////////////
}

void setSpotInitialCoordinates(const char maze[][SIZEX], Item& spot)
{
	//set spot coordinates inside the grid at random at beginning of game
	int x, y;
	spot.x = -1;

	while (spot.x == -1)
	{
		x = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
		y = Random(SIZEX - 2);      //horizontal coordinate in range [1..(SIZEX - 2)]
		
		if (maze[y][x] == TUNNEL) // if there is no zombie or pill or hole on the current position
		{
			spot.y = y;
			spot.x = x;
		}
	}
}

void setInitialMazeStructure(char maze[][SIZEX])
{ 
	// randomly places "symbol" "count" times in the "maze" 
	void placeRandomly(char maze[][SIZEX], const char symbol, int count);                 ////////////////////////////////////////////////


	//set the position of the walls in the maze
	//TODO: initial maze configuration should be amended (size changed and inner walls removed)
	//initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#', '#', '#', '#', '#','#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ','#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ','#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#', '#', '#', '#', '#','#', '#', '#', '#' }, };
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used change
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			case '0': maze[row][col] = HOLE; break;
			case '*': maze[row][col] = PILL; break;
			case 'Z': maze[row][col] = ZOMBIE; break;
			}

	

	placeRandomly(maze, HOLE, 12);  // Places 12 holes in random locations on the maze

	
	placeRandomly(maze, PILL, 8);   // Places 8 pills in random locations in the maze

	placeRandomly(maze, WALL, 4);   // places 4 random inner walls

}

void placeRandomly(char maze[][SIZEX], const char symbol, int count)
{
	int i(0);
	while (i < count)
	{
		int row = Random(SIZEX - 2);
		int col = Random(SIZEY - 2);

		if (maze[row][col] == TUNNEL 
			&& !(row == 1 && col == 1)   // if it is not the upper left corner
			&& !(row == SIZEX - 2 && col == SIZEY - 2)  // if it is not the lower right corner
			&& !(row == SIZEX - 2 && col == 1)   // if it is not upper right corner
			&& !(row == 1 && col == SIZEY - 2))  // if it is not lower left corner
		{
			maze[row][col] = symbol;
			i++;
		}
	}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item spot,Zombie zombies[])                 ///////////////////////////////////////////////////////////
{ //update grid configuration after each move                                                                   //////////////////////////////////////////////////////////
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);                                                       /////////////////////////////////////////////////////////
	void placeItem(char g[][SIZEX], const Item spot);                                                           /////////////////////////////////////////////////////////
	void placeZombies(char g[][SIZEX], Zombie zombies[], const int count);                                      ////////////////////////////////////////////////////////

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeZombies(grid, zombies, ZOMBIES_COUNT);
	placeItem(grid, spot);	//set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

void placeZombies(char g[][SIZEX], Zombie zombies[], const int count)
{ 
	//place all zombies in grid
	for (int i(0); i<count; i++)
		if (zombies[i].alive)
			g[zombies[i].y][zombies[i].x] = zombies[i].symbol;
}

void placeInnerwalls(char g[][SIZEX])
{


}

//---------------------------------------------------------------------------
//----- move items on the grid
//---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], char maze[][SIZEX], Item& spot, 
	const int key, string& mess, bool &cheated,
	int &lives, int &pills, bool &kill, bool &freeze, Zombie zombies[])
{ 
	//move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void moveZombies(const char g[][SIZEX], Zombie zombies[], Item spot, int& lives);
	int zombiesAlive(const Zombie zombies[]);

	string tostring(int x);
	assert(isArrowKey(key));

	//reset message to blank
	mess = "                                         ";		//reset message to blank

	int dx(0), dy(0);

	if (key == 'X' || key == 'x')
	{
		for (int i(0); i < ZOMBIES_COUNT; i++)
		{
			zombies[i].alive = kill;
			zombies[i].x = zombies[i].default_x;
			zombies[i].y = zombies[i].default_y;
		}

		kill = !kill;
		cheated = true;
	}

	else if (key == 'F' || key == 'f')
	{
		for (int i(0); i < ZOMBIES_COUNT; i++)
		{
			zombies[i].freeze = freeze;
		}

		freeze = !freeze;
		cheated = true;
	}

	else if (key == 'E' || key == 'e')
	{
		for (int row(0); row < SIZEY; row++)
		{
			for (int col(0); col < SIZEX; col++)
			{
				if (maze[row][col] == PILL)
				{
					maze[row][col] = TUNNEL;
				}
			}
		}

		pills = 0;
	}

	else
	{
		//calculate direction of movement for given key
		setKeyDirection(key, dx, dy);
	}

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{			//...depending on what's on the target position in grid...
	
	case TUNNEL:		//can move
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;
	
	case WALL:  		//hit a wall and stay there
        //TODO: remove alarm when bumping into walls - too annoying
		//cout << '\a';	//beep the alarm
		mess = "CANNOT GO THERE!";
		break;
	
	case HOLE:
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		lives--;
		break;

	case PILL:
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		
		maze[spot.y][spot.x] = TUNNEL;
		lives++;
		pills--;

		break;

	case ZOMBIE:
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		
		lives--;
		break;
	}

	if (dx != 0 || dy != 0)
		moveZombies(g, zombies, spot, lives);
	
	if (lives == 0)
		mess = "YOU LOST!";

	if (pills == 0 && zombiesAlive(zombies) == 0)
		mess = "YOU WON WITH " + tostring(lives) + " LIVES LEFT!";
}

// move all zombies one position on the grid
void moveZombies(const char grid[][SIZEX], Zombie zombies[], Item spot, int& lives)
{
	for (int i(0); i < ZOMBIES_COUNT; i++)
	{
		if (zombies[i].alive && !zombies[i].freeze)
		{
			int dx(0), dy(0);
			if (zombies[i].x > spot.x) dx = -1;
			else if (zombies[i].x < spot.x) dx = 1;

			if (zombies[i].y > spot.y) dy = -1;
			else if (zombies[i].y < spot.y) dy = 1;

			if (zombies[i].x + dx == spot.x && zombies[i].y + dy == spot.y)
			{
				zombies[i].x = zombies[i].default_x;
				zombies[i].y = zombies[i].default_y;
				lives--;
				continue;
			}

			switch (grid[zombies[i].y + dy][zombies[i].x + dx])
			{
				//...depending on what's on the target position in grid...
			case HOLE:
				zombies[i].alive = false;
				break;

			default:
				zombies[i].x += dx;
				zombies[i].y += dy;
				break;
			}

			bool colide = false;
			for (int j(0); j < ZOMBIES_COUNT; j++)
			{
				if (zombies[j].x == zombies[i].x && zombies[i].y == zombies[j].y && zombies[j].alive && i != j)
				{
					zombies[j].x = zombies[j].default_x;
					zombies[j].y = zombies[j].default_y;
					colide = true;
				}
			}

			if (colide)
			{
				zombies[i].x = zombies[i].default_x;
				zombies[i].y = zombies[i].default_y;
			}
		}
	}
}

//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	//TODO: UP and DOWN arrow keys should work
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:  	//when LEFT arrow pressed...
		dx = 0;	
		dy = -1;  //decrease the Y coordinate
		break;
	case DOWN: 	//when RIGHT arrow pressed...
		dx = 0;	
		dy = +1;  //increase the Y coordinate
		break;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN) || (key == 'F') || (key == 'f') || (key == 'X') || (key == 'x') || (key == 'E') || (key == 'e');
}

bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return (key == QUIT_U || key == QUIT_L);
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x) 
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{	//display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}

void paintGame(const char g[][SIZEX], string mess, const int lives, const int pre_score, string name, int pills, int zombies)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(int x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX]);
	//TODO: Change the colour of the messages
	//display game title
	showMessage(clDarkGrey, clYellow, 0, 0, "___SPOT AND ZOMBIES GAME___");

	//TODO: Date and time should be displayed from the system
	showMessage(clDarkGrey, clYellow, 40, 0, "FoP Task 1c: February 2018");
	showMessage(clDarkGrey, clYellow, 40, 1, "Date: " + GetDate());
	showMessage(clDarkGrey, clYellow, 40, 2, "Time: " + GetTime());

	//display menu options available
	showMessage(clDarkGrey, clYellow, 40, 5, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clDarkGrey, clYellow, 40, 6, "TO FREEZE ZOMBIES PRESS 'F'");
	showMessage(clDarkGrey, clYellow, 40, 7, "TO KILL ZOMBIES PRESS 'X'");
	showMessage(clDarkGrey, clYellow, 40, 8, "TO EAT ALL PILLS PRESS 'E'");
	showMessage(clDarkGrey, clYellow, 40, 9, "TO QUIT ENTER 'Q'           ");

	showMessage(clDarkGrey, clYellow, 40, 12, "REMAINING LIVES: " + tostring(lives) + "  ");
	showMessage(clDarkGrey, clYellow, 40, 13, "ZOMBIES LEFT: " + tostring(zombies) + "  ");
	showMessage(clDarkGrey, clYellow, 40, 14, "PILLS LEFT: " + tostring(pills) + "  ");

	showMessage(clDarkGrey, clYellow, 40, 20, "PLAYER'S NAME: " + name + "  ");
	showMessage(clDarkGrey, clYellow, 40, 21, "PLAYER'S PREVIOUS BEST SCORE: " + tostring(pre_score) + "  ");

	//print auxiliary messages if any
	showMessage(clDarkGrey, clYellow, 40, 18, mess);	//display current message

	//TODO: Show your course, your group number and names on screen

	//display grid contents
	paintGrid(g);
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clDarkGrey);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
		{
			switch (g[row][col])
			{
			case WALL: SelectTextColour(clDarkGrey); cout << g[row][col];	break;
			case HOLE: SelectTextColour(clRed); cout << g[row][col];	break;
			case SPOT: SelectTextColour(clWhite); cout << g[row][col];	break;
			case PILL: SelectTextColour(clYellow); cout << g[row][col];	break;
			case ZOMBIE: SelectTextColour(clGreen); cout << g[row][col];	break;
			default: cout << g[row][col];	break;
			}
				
		}
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clGrey, clYellow, 40, 23, "");	
	system("pause");	//hold output screen until a keyboard key is hit
}
