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


Connect4AiNode* Connect4AiNode::Select(float explorationVal)
{
	if (branches.size() == 0 || availableMoves.size() > 0) { return this; }
	else {
		int randomNum = rand() % 10;
		if (randomNum == 1) {
			int randomBranch = rand() % branches.size();
			return branches[randomBranch]->Select(explorationVal);
		}
		else {
			Connect4AiNode* highest = FindHighestRankingChild(explorationVal);
			return highest->Select(explorationVal);
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
			/*int randomMove = rand() % possibleMoves.size();
			GameAction newAction(possibleMoves[randomMove], playerTurn);
			copyOfGameState.makeMove(newAction);
			possibleMoves.clear();*/
			int chosenMove = 0;
			float weightTotal = 0;
			float weightFinal = 0;
			std::vector<float> weights = { 1,1,1,1,1,1,1 };

			for (int i = 0; i < possibleMoves.size(); i++)
			{
				if (i == 3)
				{
					weights[i] += 20;
				}
				if (i == 2 || i == 4)
				{
					weights[i] += 6;
				}
			}

			weightTotal = weights[0] + weights[1] + weights[2] + weights[3] + weights[4] + weights[5] + weights[6];

			for (float& i : weights)
			{
				i /= weightTotal;
			}

			float randomMove = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			for (int i = 0; i < possibleMoves.size(); i++)
			{
				weightFinal += weights[i];

				if (randomMove <= weightFinal) {
					chosenMove = i;
					break;
				}
			}



			GameAction newAction(possibleMoves[chosenMove], playerTurn);
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


Connect4AiNode* Connect4AiNode::FindHighestRankingChild(float explorationVal) //seen multiple versions of the UCB formula so using the one from week 6 ppt
{
	if (branches.size() == 0)
	{
		return NULL;
	}

	State state;
	float nodeVisits;
	float nodeWins;
	float explorationParameter = explorationVal;
	float nodeParentVisits = visits;
	float UCBVal;

	float maxRanking = 0;
	int maxIndex = 0;

	bool canAIWinNextMove;

	for (int i = 0; i < branches.size(); i++)
	{
		//float nodeWinRate = branches[i]->getRanking() / branches[i]->visits;
		nodeVisits = branches[i]->getVisits();
		nodeWins = branches[i]->getRanking();
		
		state = getGameState();
		state.makeMove(branches[i]->worldState.action);
		std::vector<int> possibleAIMoves = state.getPossibleMoves();
		
		for (int j = 0; j < possibleAIMoves.size(); j++)
		{
			GameAction newAction(possibleAIMoves[j], BLUE);
			if (state.checkWin() == BLUE)
			{
				return branches[i];
			}
		}

		UCBVal = (nodeWins/nodeVisits) + explorationParameter * sqrt((log(nodeParentVisits) / nodeVisits));

		if (UCBVal > maxRanking) 
		{
			std::vector<int> possibleOppMoves = state.getPossibleMoves();
			bool setNewMoveAllowed = true;
			for (int j = 0; j < possibleOppMoves.size(); j++)
			{
				GameAction newAction(possibleOppMoves[j], RED);
				state.makeMove(newAction);
				if (state.checkWin() == RED)
				{
					setNewMoveAllowed = false;
				}
			}

			if (setNewMoveAllowed) {
				maxIndex = i;
				maxRanking = UCBVal;
			}
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


