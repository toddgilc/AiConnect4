#pragma once

#include <vector>
#include <stack>
#include "GameBoard.h"
#include "GameState.h"

class AIConnect4Node
{
public:
	AIConnect4Node();
	~AIConnect4Node();


	//gets and sets
	void setGameState(GameState newState);
	GameState getGameState() { return worldState; }

	bool getIsEndState() { return isEndState; }
	std::vector<AIConnect4Node*> getBranches() { return branches; }
	int getRanking() { return ranking; }

	void setActivePlayer(BOARD_SQUARE_STATE active) { activePlayer = active; }
	BOARD_SQUARE_STATE getActivePlayer() { return activePlayer; }

	void setParent(AIConnect4Node* newParent) { parent = newParent; }


	// MCTS steps
	AIConnect4Node* Select();
	AIConnect4Node* Expand();
	void Simulate(BOARD_SQUARE_STATE playerTurn);
	void Backpropagate(int reward);

	// tree functions
	AIConnect4Node* FindHighestRankingChild(bool report);
	void resetNode();
	void CalcResult(BOARD_SQUARE_STATE winner);



private:
	int ranking; // ranking is updated via back propagation
	std::vector<AIConnect4Node*> branches; // child nodes
	AIConnect4Node* parent; // parent node
	GameState worldState; // game state at this node
	bool isEndState; // if eitehr game is won or no moves possible
	std::vector<std::pair<int, int>> availableMoves; // unexplored branches
	BOARD_SQUARE_STATE activePlayer; // the player who moved to create this state - child nodes should use opposite marker
	int visits = 0;

	void generatePossibleMoves();
};

