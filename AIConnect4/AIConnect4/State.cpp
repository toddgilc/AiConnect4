#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::displayBoard()
{
    for (int y = 0; y < boardX + 1; y++)
    {
        std::cout << y << "     ";
    }
    std::cout << "" << std::endl;
    for (int y = 0; y < boardY + 1; y++)
    {
      
        for (int x = 0; x < boardX + 1; x++)
        {
            switch (board.board[x][y])
            {
            case BOARD_SQUARE_STATE::NONE:
                std::cout << BGcol << ".";
                break;
            case BOARD_SQUARE_STATE::RED:
                std::cout << redCol << BGcol << "0";
                break;
            case BOARD_SQUARE_STATE::BLUE:
                std::cout << blueCol << BGcol << "0";
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

void State::makeMove(GameAction newAction)
{
    action = newAction;

    for (int i = boardY; i >= 0; i--)
    {
        if (board.board[action.position][i] == BOARD_SQUARE_STATE::NONE)    //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        {
            board.board[action.position][i] = action.playerMove;
            return; //so only draws 1 piece not whole row
        }
    }
}

BOARD_SQUARE_STATE State::checkWin() //break at 0 only goes to 3
{
    for (int i = 0; i < boardY + 1; i++)
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            if (board.board[i][j] != NONE) {
           
                if (checkDown(i, j) || checkSides(i, j) || checkDiag(i, j))
                {
                    return board.board[i][j];
                }
            }
        }
    }

    return BOARD_SQUARE_STATE::NONE;
}


bool State::checkDown(int x, int y)
{
    int vertCount = 1;

    if (y + 4 > boardY + 1) { return false; }

    for (int i = 1; i <= 4; i++)
    {
        if (board.board[x][y] == board.board[x][y + i])
        {
            vertCount++;
        }
        else { break; }
    }

    return vertCount >= 4;
}

bool State::checkSides(int x, int y)
{
    int horzCount = 1;

    for (int i = 1; i <= 4; i++)
    {
         if (x + i <= boardX + 1 && board.board[x][y] == board.board[x + i][y])
         {
             horzCount++;
         }
         else { break; }
    }


    for (int i = 1; i <= 4; i++)
    {
         if (x - i >= boardX + 1 && board.board[x][y] == board.board[x - i][y])
         {
             horzCount++;
         }
         else { break; }
    }
  
    return horzCount >= 4;
}


bool State::checkDiag(int x, int y)
{
    int diagonalUpRightCount = 1;  
    int diagonalDownRightCount = 1; 

    for (int i = 1; i <= 4; i++) {
       
        if (x + i < boardX + 1 && y - i >= 0 && board.board[x][y] == board.board[x + i][y - i]) {
            diagonalUpRightCount++;
        }
        else {
            break; 
        }
    }

    for (int j = 1; j <= 4; j++) {
      
        if (x + j < boardX + 1 && y + j < boardY + 1 && board.board[x][y] == board.board[x + j][y + j]) {
            diagonalDownRightCount++;
        }
        else {
            break;  
        }
    }

    return (diagonalUpRightCount >= 4 || diagonalDownRightCount >= 4);
}

bool State::getTwoThrees(int pos)
{




    return false;
}



std::vector<int>State::getPossibleMoves()
{
    std::vector<int> possibleMoves;

        for (int i = 0; i < boardX+1; i++)
        {
            if (board.board[i][0] == BOARD_SQUARE_STATE::NONE)
            {
                possibleMoves.push_back(i);
            }
        }
    
    return possibleMoves;
}