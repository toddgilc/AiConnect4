#include <iostream>
#include "Board.h"

bool validatePosition(int placement, Board gameBoard)
{
    if (placement < 0 || placement > 6) {

        std::cout << "Invalid position: Enter a value 0-6" << std::endl;
        return false;
    }
   
    //add check for full row
    if (gameBoard.getBoardAtPos(0, placement) != BOARD_SQUARE_STATE::NONE) //inverted inputs here !
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

    std::cout << "Hello World!\n";
    gameBoard.startGame();
    gameBoard.displayBoard();
    int count = 0;
    do {
       /* std::cout << "AI is thinking...." << std::endl;
        int aiChoice = rand() % 7;
        std::cout << "The AI selected move is " << aiChoice << std::endl;
        
        gameBoard.makeMove(aiChoice);

        gameBoard.displayBoard();
        BOARD_SQUARE_STATE winner = gameBoard.checkWin();

        if (winner == BOARD_SQUARE_STATE::RED)
        {
            gameOver = true;
            std::cout << "RED WINS!" << std::endl;
            break;
        }
        else if (winner == BOARD_SQUARE_STATE::YELLOW)
        {
            gameOver = true;
            std::cout << "YELLOW WINS!" << std::endl;
            break;
        }
         //need to if check get possible mvoes
*/     
        if (count % 2 == 0)
        {
            gameBoard.setCurrentPlayer(1);
        }
        else { gameBoard.setCurrentPlayer(2); }

        std::cout << "Players move...." << std::endl;
        bool validMove = true;
        int placement = -1;
        count++;
        do {

            placement = -1;
            std::cout << "Enter your position: ";
            std::cin >> placement;


            // validate the numerical input 
            validMove = validatePosition(placement, gameBoard);
        } while (!validMove);
        
        gameBoard.makeMove(placement, gameBoard.getCurrentPlayer());
        gameBoard.displayBoard();


        BOARD_SQUARE_STATE winner = gameBoard.checkWin();

        if (winner == BOARD_SQUARE_STATE::RED)
        {
            gameOver = true;
            std::cout << "RED WINS!" << std::endl;
        }
        else if (winner == BOARD_SQUARE_STATE::YELLOW)
        {
            gameOver = true;
            std::cout << "YELLOW WINS!" << std::endl;
        }
       

    } while (!gameOver);

    std::cout << "Game Over" << std::endl;
}
