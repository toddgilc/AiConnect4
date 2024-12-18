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
  //outputs board by looping through every pos and drawing its related colour
}

void State::makeMove(GameAction newAction) //faking the "gravity"
{
    action = newAction;

    for (int i = boardY; i >= 0; i--)
    {
        if (board.board[action.position][i] == BOARD_SQUARE_STATE::NONE)    //if there is space open comes up from bottom of board and finds lowest space it can fit in
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
           
                if (checkDown(i, j, 4) || checkSides(i, j, 4) || checkDiag(i, j, 4)) //loop through whole board and if theres lines of 4 in each direction the piece is returned as winning
                {
                    return board.board[i][j];
                }
            }
        }
    }

    return BOARD_SQUARE_STATE::NONE;
}


bool State::checkDown(int x, int y, int numRow) //num row is used so this function can be used to check for 3s in a row for ai 
{
    int vertCount = 1;

    if (y + 4 > boardY + 1) { return false; }   //dont check if the column would end up out of board boundaries

    for (int i = 1; i <= numRow; i++)
    {
        if (board.board[x][y] == board.board[x][y + i]) //if token below is same increment, check max of 3 times if a token doesnt match return and reset values
        {
            vertCount++;    
        }
        else { break; }
    }

    return vertCount >= numRow; 
}

bool State::checkSides(int x, int y, int numRow)
{
    int horzCount = 1;

    for (int i = 1; i <= numRow; i++)
    {
         if (x + i <= boardX + 1 && board.board[x][y] == board.board[x + i][y]) //checks right if within bounds
         {
             horzCount++;
         }
         else { break; }
    }



    for (int i = 1; i <= numRow; i++)
    {
         if (x - i >= boardX + 1 && board.board[x][y] == board.board[x - i][y]) //checks left
         {
             horzCount++;
         }
         else { break; }
    }
  
    return horzCount >= numRow;
}


bool State::checkDiag(int x, int y, int numRow)
{
    int diagonalUpRightCount = 1;  
    int diagonalDownRightCount = 1; 

    for (int i = 1; i <= numRow; i++) { //upwards diag (-i as board goes from 6 bottom to 0 top)
       
        if (x + i < boardX + 1 && y - i >= 0 && board.board[x][y] == board.board[x + i][y - i]) { //checks both x and y bounds as diag,
            diagonalUpRightCount++; //seperate value count as line cant be the different diagonal unlike sides
        }
        else {
            break; 
        }
    }

    for (int j = 1; j <= numRow; j++) {
      
        if (x + j < boardX + 1 && y + j < boardY + 1 && board.board[x][y] == board.board[x + j][y + j]) {   //downwards diag check
            diagonalDownRightCount++;
        }
        else {
            break;  
        }
    }

    return (diagonalUpRightCount >= numRow || diagonalDownRightCount >= numRow);
}

bool State::getTwoThrees(int pos)   //used to encourage ai to go for situations where it has 2 rows of 3 to potentially complete
{
    int threesCount = 0;

    for (int i = 0; i < boardY + 1; i++) //checking if the input position creates a situation where there can be 4
    {
        if (board.board[i][pos] != NONE) {

            if (checkDown(i, pos, 3) || checkSides(i, pos, 3) || checkDiag(i, pos, 3))  //using 3 as the count as dont want wins yet
            {
                threesCount++;
            }
        }
    }


    for (int i = 0; i < boardY + 1; i++)    //checks if rest of board has an AI 3 as well 
    {
        for (int j = 0; j < boardX + 1; j++)
        {
            if (board.board[i][j] != NONE) {

                if (checkDown(i, j, 3) || checkSides(i, j, 3) || checkDiag(i, j, 3))    
                {
                    threesCount++;
                }
            }
        }
    }


    return threesCount >= 2;
}



std::vector<int>State::getPossibleMoves() //possible moves = board x spaces - where the rows reach the top
{
    std::vector<int> possibleMoves;

        for (int i = 0; i < boardX + 1; i++)
        {
            if (board.board[i][0] == BOARD_SQUARE_STATE::NONE)
            {
                possibleMoves.push_back(i); //adding to vector
            }
        }
    
    return possibleMoves;
}