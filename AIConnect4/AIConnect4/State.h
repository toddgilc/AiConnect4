#pragma once
#include "Board.h"
#include <iostream>
#include <vector>

#define boardX 6
#define boardY 5	//-1 on both values for for loop usage can use + 1 due to loop indexing

class State
{

public:
	State();
	~State();

	GameBoard board;
	GameAction action;

	//functions for in play
	void displayBoard();	//draws to screen
	void makeMove(GameAction newAction); //move struct
	BOARD_SQUARE_STATE checkWin();	//check the winner of the game after each move
	bool checkDown(int y, int x);
	bool checkSides(int y, int x);
	bool checkDiag(int y, int x);	//functions used to seperate out check win (useful for testing)

	std::vector<int> getPossibleMoves(); //for ai to figure out where it can place coins 

private:

	int currentPlayer = 1; //first player is AI

	const char* resetcol = "\033[0m";	//rests
	const char* underlinecol = "\033[4;34m";	//underlines!
	const char* BGcol = "\033[47m"; //white
	const char* redCol = "\033[1;31m"; //red
	const char* blueCol = "\033[1;34m"; //blue
};