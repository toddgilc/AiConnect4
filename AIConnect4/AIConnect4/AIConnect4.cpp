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
    BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::BLUE;
    BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::RED;
    std::cout << "Connect 4!" << std::endl;

    gameState.displayBoard();
    int count = 0;

    do {

        std::cout << "Ai move...." << std::endl;
        bool validMove = true;
        int placement;
        GameAction aiAction;

        do {

            std::cout << "Enter your position: ";
            std::cin >> placement;


            // validate the numerical input 
            validMove = validatePosition(placement, gameState);
        } while (!validMove);

        aiAction.position = placement;
        aiAction.playerMove = aiMarker;

        gameState.makeMove(aiAction);
        gameState.displayBoard();

        count++;
        BOARD_SQUARE_STATE winner = gameState.checkWin();

        if (winner == BOARD_SQUARE_STATE::RED)
        {
            gameOver = true;
            std::cout << "RED WINS!" << std::endl;
            break;
        }
        else if (winner == BOARD_SQUARE_STATE::BLUE)
        {
            gameOver = true;
            std::cout << "BLUE WINS!" << std::endl;
            break;
        }



        ///////////////////////////////////////////////////

      
        std::cout << "Players move...." << std::endl;
        validMove = true;
        placement;
        GameAction playerAction;

        do {

            std::cout << "Enter your position: ";
            std::cin >> placement;


            // validate the numerical input 
            validMove = validatePosition(placement, gameState);
        } while (!validMove);
        
        playerAction.position = placement;
        playerAction.playerMove = playerMarker;

        gameState.makeMove(playerAction);
        gameState.displayBoard();

        count++;
        winner = gameState.checkWin();

        if (winner == BOARD_SQUARE_STATE::RED)
        {
            gameOver = true;
            std::cout << "RED WINS!" << std::endl;
            break;
        }
        else if (winner == BOARD_SQUARE_STATE::BLUE)
        {
            gameOver = true;
            std::cout << "BLUE WINS!" << std::endl;
            break;
        }
       

    } while (!gameOver);

    std::cout << "Game Over" << std::endl;
}
