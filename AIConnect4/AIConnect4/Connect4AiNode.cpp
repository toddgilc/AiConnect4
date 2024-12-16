#include "Connect4AiNode.h"



BOARD_SQUARE_STATE Connect4AiNode::getOppositeMove(BOARD_SQUARE_STATE state)
{
	// Flip to the other player
	if (state == BOARD_SQUARE_STATE::RED)
	{
		return BOARD_SQUARE_STATE::BLUE;
	}
	else if (state == BOARD_SQUARE_STATE::BLUE)
	{
		return BOARD_SQUARE_STATE::RED;
	}
	else
	{
		return BOARD_SQUARE_STATE::NONE;
	}
}


Connect4AiNode::Connect4AiNode()
{
	srand(time(0));
	isEndState = false;
	parent = nullptr;
	ranking = 0;
	activePlayer = BOARD_SQUARE_STATE::NONE;
}

Connect4AiNode::~Connect4AiNode()
{
	parent = nullptr;
	for (auto branch : branches)
	{
		delete branch;
	}
	branches.clear();
	availableMoves.clear();
}

void Connect4AiNode::setGameState(State newState)
{
	worldState = newState;
	generatePossibleMoves();
}

void Connect4AiNode::generatePossibleMoves()
{
	availableMoves = worldState.getPossibleMoves();

	if (availableMoves.size() == 0)
	{
		isEndState = true;
	}

	if (worldState.checkWin() != BOARD_SQUARE_STATE::NONE)
	{
		isEndState = true;
	}
}


Connect4AiNode* Connect4AiNode::Select()
{
	if (branches.size() == 0 || availableMoves.size() > 0) { return this; }
	else {
		int randomNum = rand() % 10;
		if (randomNum < 2) {
			int randBranch = rand() % branches.size();
			return branches[randBranch]->Select();
		}
		else {
			Connect4AiNode* highest = FindHighestRankingChild(false);
			return highest->Select();
		}
	}
}


Connect4AiNode* Connect4AiNode::Expand()
{
	if (isEndState || availableMoves.size() == 0)
		return NULL; // if this is game end state, we cannot expand
	else
	{
		// pick a random move from the unexplored ones
		int randomMove = rand() % availableMoves.size();
		int move = availableMoves[randomMove];

		// delete the selected move from vector
		availableMoves.erase(availableMoves.begin() + randomMove);

		// create a new child node 
		Connect4AiNode* childNode = new Connect4AiNode();
		childNode->setParent(this);

		// flip the marker to other player
		BOARD_SQUARE_STATE nextTurn = getOppositeMove(activePlayer);
		childNode->setCurrentPlayer(nextTurn);

		// create the new world state
		State newWorldState;
		newWorldState.board = this->worldState.board;
		GameAction newAction(move, nextTurn);

		// apply the move to the board and set the child's state
		newWorldState.makeMove(newAction);
		childNode->setGameState(newWorldState);

		// add the newly created node as a child of current node		
		branches.push_back(childNode);

		// reutrn node, for simulation
		return childNode;
	}
}


void Connect4AiNode::Simulate(BOARD_SQUARE_STATE startingTurn)
{
	// craete a copy of the node's game state as the starting point for the simulation
	State copyOfGameState = getGameState();

	BOARD_SQUARE_STATE playerTurn = startingTurn;
	bool endState = false;

	// check to see if this newly expanded node is a win state 
	BOARD_SQUARE_STATE winner = copyOfGameState.checkWin();

	if (winner != BOARD_SQUARE_STATE::NONE)
	{
		// we have a winner from the expanded state
		CalcResult(winner);
		return;
	}

	// repeat until we reach an end state
	do {

		// get possible moves from the current simulation state
		std::vector<int> possibleMoves = copyOfGameState.getPossibleMoves();

		if (possibleMoves.size() == 0)
		{
			// no possible moves, game must be a draw
			endState = true;
			CalcResult(BOARD_SQUARE_STATE::NONE);
			return;

		}
		else
		{
			// pick a random move and apply it to the simulation state
			int randomMove = rand() % possibleMoves.size();
			GameAction newAction(possibleMoves[randomMove], playerTurn);
			copyOfGameState.makeMove(newAction);
			possibleMoves.clear();
		}

		// check if the last move was a winning one
		BOARD_SQUARE_STATE winner = copyOfGameState.checkWin();

		if (winner != BOARD_SQUARE_STATE::NONE)
		{
			// end state, game has been won
			CalcResult(winner);
			endState = true;
			return;
		}

		// flip marker for next move
		playerTurn = getOppositeMove(playerTurn);

	} while (!endState);

}


void Connect4AiNode::CalcResult(BOARD_SQUARE_STATE winner)
{
	if (winner == BOARD_SQUARE_STATE::BLUE)
	{
		Backpropagate(1);
	}
	else if (winner == BOARD_SQUARE_STATE::RED)
	{
		Backpropagate(-1);
	}
	else
	{
		Backpropagate(0);
	}
}


void Connect4AiNode::Backpropagate(int result)
{
	visits++;

	if (result > 0) {
		ranking += result;
	}

	// if we're not at the root, update the parent with the result
	if (this->parent != NULL)
		this->parent->Backpropagate(result);
}


Connect4AiNode* Connect4AiNode::FindHighestRankingChild(bool report) //seen multiple versions of the UCB formula so using the one from week 6 ppt
{
	if (branches.size() == 0)
	{
		return NULL;
	}

	//if ()
	//{

	//}

	float nodeVisits;
	float nodeWins;
	float explorationParameter = 1.1;
	float newExplorationParameter;
	float nodeParentVisits = visits;
	float UCBVal;

	float maxRanking = 0;
	int maxIndex = 0;

	for (int i = 0; i < branches.size(); i++)
	{
		//float nodeWinRate = branches[i]->getRanking() / branches[i]->visits;
		nodeVisits = branches[i]->getVisits();
		nodeWins = branches[i]->getRanking();
		
		UCBVal = (nodeWins/nodeVisits) + explorationParameter * sqrt((log(nodeParentVisits) / nodeVisits));
		

		if (UCBVal > maxRanking) 
		{
			maxIndex = i;
			maxRanking = UCBVal;
		}
	}

	return branches[maxIndex];
}


void Connect4AiNode::resetNode()
{
	branches.clear();
	ranking = 0;
	availableMoves.clear();
	visits = 0;
}


