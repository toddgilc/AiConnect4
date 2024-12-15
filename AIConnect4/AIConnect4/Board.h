#pragma once	//adapted from the MCTS game state + board headers


enum BOARD_SQUARE_STATE { NONE, RED, BLUE };	//each tile can be one of these three states

struct GameBoard
{
	BOARD_SQUARE_STATE board[7][6]{ { BOARD_SQUARE_STATE::NONE } }; //board size 
};

struct GameAction
{
	int position;
	BOARD_SQUARE_STATE playerMove;

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




