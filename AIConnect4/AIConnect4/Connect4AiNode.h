#pragma once
#include <vector>
#include "Board.h"
#include "State.h"

class Connect4AiNode
{
public:
	Connect4AiNode();
	~Connect4AiNode();


	//getters + setters
	State getGameState() { return worldState; }
	void setGameState(State newState);
	
	bool getIsEndState() { return isEndState; }
	std::vector<Connect4AiNode*> getBranches() { return branches; }
	int getRanking() { return ranking; }
	int getVisits() { return visits; }

	BOARD_SQUARE_STATE getCurrentPlayer() { return activePlayer; };
	void setCurrentPlayer(BOARD_SQUARE_STATE actingPlayer) { activePlayer = actingPlayer; };

	void setParent(Connect4AiNode* newParent) { parent = newParent; }

	BOARD_SQUARE_STATE getOppositeMove(BOARD_SQUARE_STATE state);	//switches players

	//node functions for "traversing" the mcts tree
	Connect4AiNode* Select(float explorationVal);
	Connect4AiNode* Expand();
	void Simulate(BOARD_SQUARE_STATE playerTurn, bool weightsOnOff);
	void Backpropagate(int reward);

	Connect4AiNode* FindHighestRankingChild(float explorationVal);
	void resetNode();
	void CalcResult(BOARD_SQUARE_STATE winner);

private:
	int ranking;
	int visits = 0; //count per node how many times its visited
	std::vector<Connect4AiNode*> branches;	
	Connect4AiNode* parent;	
	State worldState;	
	bool isEndState;
	std::vector<int> availableMoves;
	BOARD_SQUARE_STATE activePlayer;

	void generatePossibleMoves();	
};

