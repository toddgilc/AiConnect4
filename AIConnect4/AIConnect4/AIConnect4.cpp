#include <iostream>
#include "State.h"
#include "Board.h"

bool validatePosition(int placement, State gameState)
{
    if (placement < 0 || placement > 6) {

        std::cout << "Invalid position: Enter a value 0-6" << std::endl;
        return false;
    }
   
    //add check for full row
    if (gameState.getBoardAtPos(placement, 0) != BOARD_SQUARE_STATE::NONE) //inverted inputs here !
    {
        std::cout << "Row full: Enter a different position" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    State gameState;
    bool gameOver = false;

    gameState.displayBoard();
    int count = 0;
    do {

        if (count % 2 == 0)
        {
            gameState.setCurrentPlayer(1);
        }
        else { gameState.setCurrentPlayer(2); }
       
        std::cout << "Players move...." << std::endl;
        bool validMove = true;
        int placement;// = -1;

        do {

           // placement = -1;
            std::cout << "Enter your position: ";
            std::cin >> placement;


            // validate the numerical input 
            validMove = validatePosition(placement, gameState);
        } while (!validMove);
        
        gameState.makeMove(placement, gameState.getCurrentPlayer());
        gameState.displayBoard();

        count++;
        BOARD_SQUARE_STATE winner = gameState.checkWin();

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
