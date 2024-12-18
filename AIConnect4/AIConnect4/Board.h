#pragma once	//adapted from the MCTS game state + board headers
//this proj was made from a blank program and incoroporates the week 6 work where required so some of the excecution may be different

enum BOARD_SQUARE_STATE { NONE, RED, BLUE };	//each tile can be one of these three states

struct GameBoard
{
	BOARD_SQUARE_STATE board[7][6]{ { BOARD_SQUARE_STATE::NONE } }; //board size of 7x 6y (Y starts at 0 at top is 6 at bottom)
};

struct GameAction
{
	int position;
	BOARD_SQUARE_STATE playerMove;	//action created of where and who played

public:
	GameAction(int newPos, BOARD_SQUARE_STATE move)
	{
		position = newPos;
		playerMove = move;
	}

	GameAction()
	{
		playerMove = BOARD_SQUARE_STATE::NONE;
		position = 0;
	}

};




