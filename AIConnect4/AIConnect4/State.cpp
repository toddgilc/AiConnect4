#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::displayBoard()
{
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

    for (int i = boardY; i > -1; i--)
    {
        if (board.board[newAction.position][i] == BOARD_SQUARE_STATE::NONE)    //aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
        {
            board.board[newAction.position][i] = newAction.playerMove;
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
            if (checkDown(i, j))
            {
                return board.board[i][j];
            }
            else if (checkSides(i, j))
            {
                return board.board[i][j];
            }
            else if (checkDiag(i, j))
            {
                return board.board[i][j];
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
        if (board.board[x][y] != NONE && board.board[x][y] == board.board[x][y + i])
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

    if (!(x - 4 < boardX + 1)) {

        for (int i = 1; i <= 4; i++)
        {
            if (board.board[x][y] != NONE && board.board[x][y] == board.board[x + i][y])
            {
                horzCount++;
            }
        }

    }

    if (!(x + 4 > boardX + 1)) {

        for (int i = 1; i <= 4; i++)
        {
            if (board.board[x][y] != NONE && board.board[x][y] == board.board[x + i][y])
            {
                horzCount++;
            }
        }

    }
  
    return horzCount >= 4;
}

bool State::checkDiag(int x, int y) //update with new method
{
    int diagCount = 1;
    bool left = true;
    bool right = true;

    for (int i = 1; i <= 4; i++)
    {
        if (board.board[x][y] != NONE && board.board[x][y] == board.board[x + i][y + i] && right)
        {
            diagCount++;
        }
        else { right = false; }

        if (board.board[x][y] != NONE && board.board[x][y] == board.board[x - i][y - i] && left)
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
        if (board.board[x][y] != NONE && board.board[x][y] == board.board[x + i][y - i] && right)
        {
            diagCount++;
        }
        else { right = false; }

        if (board.board[x][y] != NONE && board.board[x][y] == board.board[x - i][y + i] && left)
        {
            diagCount++;
        }
        else { left = false; }

        if (!right && !left) { break; }
    }

    return diagCount >= 4;
}

