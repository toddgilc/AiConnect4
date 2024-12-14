#pragma once	//adapted from the MCTS game state + board headers
#include <iostream>
#include <vector>

#define boardX 6
#define boardY 5	//-1 on both values for for loop usage can use + 1

enum BOARD_SQUARE_STATE { NONE, RED, YELLOW };	//each tile can be one of these three states

class Board
{

public:
	Board();
	~Board();
	
	int getCurrentPlayer() { return currentPlayer; };	
	void setCurrentPlayer(int chosenPlayer) { currentPlayer = chosenPlayer; };
	BOARD_SQUARE_STATE getBoardAtPos(int x, int y) { return board[x][y]; };

	//functions for in play
	void startGame();
	void displayBoard();
	void makeMove(int posChoice, int currentPlayer);
	BOARD_SQUARE_STATE checkWin();

	const char* resetcol = "\033[0m"; 
	const char* underlinecol = "\033[4;34m";
	const char* BGcol = "\033[47m"; //white
	const char* redCol = "\033[1;31m"; //red
	const char* yellowCol = "\033[1;33m"; //yellow
	BOARD_SQUARE_STATE board[6][7]{ { BOARD_SQUARE_STATE::NONE } }; //board size 
private:

	int currentPlayer = 1; //first player is AI

};