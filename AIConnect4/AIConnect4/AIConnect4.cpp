#include <iostream>
#include "State.h"
#include "Board.h"
#include "Connect4AiNode.h"

const int MAX_RUNS = 5000; //default val changeable later

bool checkFullStack(int placement, State gameState) //was split into its own function originally to be used in other places but that code has been tidied
{
    if (gameState.board.board[placement][0] != BOARD_SQUARE_STATE::NONE) //checking if at the top of a row there is a piece or not already there
    {
        std::cout << "Column full: Enter a different position" << std::endl;
        return false;
    }
} 


bool validatePosition(int placement, State gameState)
{
    if (placement < 0 || placement > 6) {

        std::cout << "Invalid position: Enter a value 0-6" << std::endl;
        return false;
    }   //keeps the placement within the boards bounds
   
    if (!checkFullStack(placement, gameState))
    {
        return false;
    }

    return true;
}


int main()
{
    srand(time(0)); //creates actualrandom values using the time as a seed
   

    int AIGamesWon = 0;
    int randGamesWon = 0;
    int draws = 0;
    int playChoice;
    int gamesToPlay = 1;
    float explorationVal = 1.1;
    int maxRuns = 1000;
    bool weightsOn = false;
    int weightsOnOffInput = 1;  //AI parameters and output variables

    std::cout << "Please input exploration val (mostly tested as 1) " << std::endl;
    std::cin >> explorationVal;

    std::cout << "Please input max runs (mostly tested as 1000) " << std::endl;
    std::cin >> maxRuns;

    std::cout << "Please input if you would prefer weights applied to certain scenarios, 1 for yes, 2 for no " << std::endl;
    std::cin >> weightsOnOffInput;
    if (weightsOnOffInput == 1) { weightsOn = true; }
    else { weightsOn = false; }

    std::cout << "Play vs AI - 1, rand vs AI - 2 " << std::endl;
    std::cin >> playChoice;

    if (playChoice == 2)
    {
        std::cout << "Input games to play " << std::endl;
        std::cin >> gamesToPlay;
    }
 
    for (int i = 0; i < gamesToPlay; i++) //will run the whole game x numberof times for testing
    {
        BOARD_SQUARE_STATE aiMarker = BOARD_SQUARE_STATE::BLUE;
        BOARD_SQUARE_STATE playerMarker = BOARD_SQUARE_STATE::RED; //setting the enum value for each players / token
        Connect4AiNode* rootNode = new Connect4AiNode();    
        State gameState;
        bool gameOver = false;
        int boardFull = 0;

        std::cout << "Connect 4!" << std::endl;

        gameState.displayBoard();


        do {
            std::cout << "Ai move...." << std::endl;

            rootNode->setCurrentPlayer(playerMarker); // the AI will move first from empty state
            rootNode->setGameState(gameState);
            int runCount = 0;

            do {

                Connect4AiNode* selectedNode = rootNode->Select(explorationVal);  //recursive so only stops when maxruns is met, gives nodes value 
                Connect4AiNode* expandedNode = selectedNode->Expand();  //creates new nodes on the tree off of leaf nodes

                if (!expandedNode == NULL)
                {
                    expandedNode->Simulate(aiMarker, weightsOn);    //plays a random game from a node to estimate value, value estimate improves with more runs
                }

                runCount++;

            } while (runCount < maxRuns);

            Connect4AiNode* highestChild = rootNode->FindHighestRankingChild(explorationVal); //gets the best move from the tree
            GameAction bestAction = highestChild->getGameState().action;    //sets an action from that 
            bestAction.playerMove = aiMarker;
            std::cout << "The AI selected move is " << bestAction.position << std::endl;    
            gameState.makeMove(bestAction); //plays the chosen placement
            gameState.displayBoard();   

            BOARD_SQUARE_STATE winner = gameState.checkWin();   //checks sides down and diag for winner 

            if (winner == BOARD_SQUARE_STATE::RED)
            {
                randGamesWon++;
                gameOver = true;
                std::cout << "RED WINS!" << std::endl;
                break;
            }
            else if (winner == BOARD_SQUARE_STATE::BLUE)
            {
                AIGamesWon++;
                gameOver = true;
                std::cout << "BLUE WINS!" << std::endl;
                break;
            }
            if (gameState.getPossibleMoves().size() == 0)
            {
                draws++;
                gameOver = true;
                std::cout << "DRAW" << std::endl;
                break;
            }

            ////////////////////////////////////////////////////////////////////////////////////// player section

            std::cout << "Players move...." << std::endl;
            bool validMove = true;
            int placement;
            GameAction playerAction;

            do {
                if (playChoice == 1)
                {
                    std::cout << "Enter your position: ";
                    std::cin >> placement;
                }
                else { placement = rand() % 7 - 1; } //plays randomly or based off of input if chosen earlier

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
                randGamesWon++;
                gameOver = true;
                std::cout << "RED WINS!" << std::endl;
                break;
            }
            else if (winner == BOARD_SQUARE_STATE::BLUE)
            {
                AIGamesWon++;
                gameOver = true;
                std::cout << "BLUE WINS!" << std::endl;
                break;
            }
            if (gameState.getPossibleMoves().size() == 0) {
                draws++;
                gameOver = true;
                std::cout << "DRAW" << std::endl;
                break;
            }


            rootNode->resetNode(); 

        } while (!gameOver);

    }

    std::cout << "Game(s) Over" << std::endl;

    std::cout << "AI wins:  " << AIGamesWon << std::endl;
    std::cout << "Rand/Player wins: " << randGamesWon << std::endl;
    std::cout << "Draws: " << draws << std::endl;
}
