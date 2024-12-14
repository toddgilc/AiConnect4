#include <iostream>
#include "Board.h"

bool validatePosition(int placement, Board gameBoard)
{
    if (placement < 0 || placement > 6) {

        std::cout << "Invalid position: Enter a value 0-6" << std::endl;
        return false;
    }
   
    //add check for full row
    if (gameBoard.getBoardAtPos(placement, 0) != BOARD_SQUARE_STATE::NONE) //inverted inputs here !
    {
        std::cout << "Row full: Enter a different position" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    Board gameBoard;
    bool gameOver = false;

    //gameBoard.startGame();

    gameBoard.displayBoard();
    int count = 0;
    do {

        if (count % 2 == 0)
        {
            gameBoard.setCurrentPlayer(1);
        }
        else { gameBoard.setCurrentPlayer(2); }
       
        std::cout << "Players move...." << std::endl;
        bool validMove = true;
        int placement;// = -1;

        do {

           // placement = -1;
            std::cout << "Enter your position: ";
            std::cin >> placement;


            // validate the numerical input 
            validMove = validatePosition(placement, gameBoard);
        } while (!validMove);
        
        gameBoard.makeMove(placement, gameBoard.getCurrentPlayer());
        gameBoard.displayBoard();

        count++;
        BOARD_SQUARE_STATE winner = gameBoard.checkWin();

        if (winner == BOARD_SQUARE_STATE::RED)
        {
            gameOver = true;
            std::cout << "RED WINS!" << std::endl;
        }
        else if (winner == BOARD_SQUARE_STATE::BLUE)
        {
            gameOver = true;
            std::cout << "BLUE WINS!" << std::endl;
        }
       

    } while (!gameOver);

    std::cout << "Game Over" << std::endl;
}
