#include <iostream>
#include "State.h"
#include "Board.h"
#include "Connect4AiNode.h"

const int MAX_RUNS = 5000;

bool validatePosition(int placement, State gameState)
{
    if (placement < 0 || placement > 6) {

        std::cout << "Invalid position: Enter a value 0-6" << std::endl;
        return false;
    }
   
    //add check for full row
    if (gameState.board.board[placement][0] != BOARD_SQUARE_STATE::NONE) //inverted inputs here !
    {
        std::cout << "Row full: Enter a different position" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::BLUE;
    BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::RED;
    Connect4AiNode* rootNode = new Connect4AiNode();
    State gameState;
    bool gameOver = false;

    std::cout << "Connect 4!" << std::endl;

    gameState.displayBoard();


    do {
        //std::cout << "Ai move...." << std::endl;

        //rootNode->setCurrentPlayer(playerMarker); // the AI will move first from empty state
        //rootNode->setGameState(gameState);
        //int runCount = 0;

        //do {

        //    Connect4AiNode* selectedNode = rootNode->Select();
        //    Connect4AiNode* expandedNode = selectedNode->Expand();

        //    if (!expandedNode == NULL)
        //    {
        //        expandedNode->Simulate(aiMarker);
        //    }

        //    runCount++;

        //} while (runCount < MAX_RUNS);

        //Connect4AiNode* highestChild = rootNode->FindHighestRankingChild(true);
        //GameAction bestAction = highestChild->getGameState().action;
        //bestAction.playerMove = aiMarker;
        //std::cout << "The AI selected move is " << bestAction.position << std::endl;
        //gameState.makeMove(bestAction);
        //gameState.displayBoard();

        BOARD_SQUARE_STATE winner = gameState.checkWin();

        //if (winner == BOARD_SQUARE_STATE::RED)
        //{
        //    gameOver = true;
        //    std::cout << "RED WINS!" << std::endl;
        //    break;
        //}
        //else if (winner == BOARD_SQUARE_STATE::BLUE)
        //{
        //    gameOver = true;
        //    std::cout << "BLUE WINS!" << std::endl;
        //    break;
        //}
        //if (gameState.getPossibleMoves().size() == 0)
        //{
        //    gameOver = true;
        //    std::cout << "DRAW" << std::endl;
        //    break;
        //}

        //////////////////////////////////////////////////////////////////////////////////////
      
        std::cout << "Players move...." << std::endl;
        bool validMove = true;
        int placement;
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
       

        rootNode->resetNode();

    } while (!gameOver);

    std::cout << "Game Over" << std::endl;
}
