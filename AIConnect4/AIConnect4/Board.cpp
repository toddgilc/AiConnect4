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
    for (int i = 0; i < boardY + 1; i++)
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            if (checkDown(i, j) || checkSides(i, j) || checkDiag(i, j))
            {
                return board[i][j];
            }
        }
    }

    return BOARD_SQUARE_STATE::NONE;
}



bool Board::checkDown(int x, int y)
{
    int vertCount = 1;

    for (int i = 1; i <= 4; i++)
    {
        if (board[x][y] != NONE && board[x][y] == board[x][y + i])
        {
            vertCount++;
        }
        else { break; }
    }

    return vertCount >= 4;
}

bool Board::checkSides(int x, int y)
{
    int horzCount = 1;
    bool left = true;
    bool right = true;


    for (int i = 1; i <= 4; i++)
    {
        if (board[x][y] != NONE && board[x][y] == board[x + i][y] && right)
        {
            horzCount++;
        }
        else { right = false; }

        if (board[x][y] != NONE && board[x][y] == board[x - i][y] && left)
        {
            horzCount++;
        }
        else { left = false; }

        if (!right && !left) { break; }
    }

    return horzCount >= 4;
}

bool Board::checkDiag(int x, int y)
{
    int diagCount = 1;
    bool left = true;
    bool right = true;

    for (int i = 1; i <= 4; i++)
    {
        if (board[x][y] != NONE && board[x][y] == board[x + i][y + i] && right)
        {
            diagCount++;
        }
        else { right = false; }

        if (board[x][y] != NONE && board[x][y] == board[x - i][y - i] && left)
        {
            diagCount++;
        }
        else { left = false; }

        if (!right && !left) { break; }
    }

    if (diagCount >= 4) { return true; }

    diagCount = 1;
    left = true;
    right = true;

    for (int i = 1; i <= 4; i++)
    {
        if (board[x][y] != NONE && board[x][y] == board[x + i][y - i] && right)
        {
            diagCount++;
        }
        else { right = false; }

        if (board[x][y] != NONE && board[x][y] == board[x - i][y + i] && left)
        {
            diagCount++;
        }
        else { left = false; }

        if (!right && !left) { break; }
    }

    return diagCount >= 4;
}

