#include "Board.h"

Board::Board()
{
}

Board::~Board()
{
}

void Board::startGame() 
{
    currentPlayer = 1;

    for (int i = 0; i < boardY + 1; i++)
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            board[i][j] = NONE; //setting up a blank 7x6 board
        }
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
                std::cout << redCol << BGcol << "0";
                break;
            case BOARD_SQUARE_STATE::YELLOW:
                std::cout << yellowCol << BGcol << "0";
                break;
            default:
                break;
            }
            std::cout << underlinecol << "     ";
        }
        std::cout << resetcol << "" << std::endl;
       
    }
    //std::cout << resetcol << "";
}

void Board::makeMove(int posChoice, int currentPlayer)
{
    BOARD_SQUARE_STATE piece = BOARD_SQUARE_STATE::NONE;

    if (currentPlayer == 1) { piece = BOARD_SQUARE_STATE::YELLOW; }
    else if (currentPlayer == 2) { piece = BOARD_SQUARE_STATE::RED; }   //setting players colour

    for (int i = boardY; i >= 0; i--)   
    {
        if (board[i][posChoice] == BOARD_SQUARE_STATE::NONE)
        {
            board[i][posChoice] = piece;
            break; //so only draws 1 piece not whole row
        }
    }
}

BOARD_SQUARE_STATE Board::checkWin()
{
	return BOARD_SQUARE_STATE();
}
