#include "Board.h"

Board::Board()
{
}

Board::~Board()
{
}

void Board::startGame() 
{
    for (int i = 0; i < boardY + 1; i++)
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            board[i][j] = NONE; //setting up a blank 7x6 board
        }
        std::cout << std::endl;
    }
}

void Board::displayBoard() 
{
    for (int i = 0; i < boardY + 1; i++)
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            switch (board[i][j])
            {
            case BOARD_SQUARE_STATE::NONE:
                std::cout << BGcol << ".";
                break;
            case BOARD_SQUARE_STATE::RED:
                std::cout << redCol << "0";
                break;
            case BOARD_SQUARE_STATE::YELLOW:
                std::cout << yellowCol << "0";
                break;
            default:
                break;
            }
            std::cout << underlinecol << "     ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }
    std::cout << resetcol << ".";
}

void Board::makeMove()
{

}

BOARD_SQUARE_STATE Board::checkWin()
{
	return BOARD_SQUARE_STATE();
}
