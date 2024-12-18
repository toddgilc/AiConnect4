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
		delete branch; //clears vector of nodes
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


Connect4AiNode* Connect4AiNode::Select(float explorationVal) //recursive function to chose a leaf node to expand from
{
	if (branches.size() == 0 || availableMoves.size() > 0) { return this; }
	else {
		int randomNum = rand() % 10;
		if (randomNum == 1) { //10% will test a random branch 
			int randomBranch = rand() % branches.size();
			return branches[randomBranch]->Select(explorationVal);
		}
		else {	//otherwise will go on to expand the best 
			Connect4AiNode* highest = FindHighestRankingChild(explorationVal);
			return highest->Select(explorationVal);
		}
	}
}


Connect4AiNode* Connect4AiNode::Expand() //creates a new leaf node from the previous to create a new state
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


void Connect4AiNode::Simulate(BOARD_SQUARE_STATE startingTurn, bool weightsOnOff) //finds the value of a node by simulating the rest of play (either until win or forced end state)
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
			if (weightsOnOff) {
				int chosenMove = 0;
				float weightTotal = 0;
				float weightFinal = 0;
				std::vector<float> weights = { 1,1,1,1,1,1,1 }; 

				for (int i = 0; i < possibleMoves.size(); i++) //applies weights to certain situations from the posible moves
				{
					if (i == 3)
					{	//centre column is weighted as it is the best position on the board
						weights[i] += 12;
					}
					if (i == 2 || i == 4)
					{	//columns adjecent are also more valuable
						weights[i] += 6;
					}
					if (copyOfGameState.getTwoThrees(i))	//a move that creates 2 rows of three heavily weighted as it is an almost guaranteed win
					{
						weights[i] += 20;
					}
				}

				weightTotal = weights[0] + weights[1] + weights[2] + weights[3] + weights[4] + weights[5] + weights[6]; //sum the weights

				for (float& i : weights)
				{
					i /= weightTotal;
				}	//normalises weights values into all summed = 1

				float randomMove = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //find a random value between 0 and 1

				for (int i = 0; i < possibleMoves.size(); i++) //loop checking to find the first highest weighted move above the random value so our play is encouraging specific outcomes
				{
					weightFinal += weights[i];

					if (randomMove <= weightFinal) {
						chosenMove = i;
						break; //escapes to make the move once the first is found
					}
				}


				GameAction newAction(possibleMoves[chosenMove], playerTurn);
				copyOfGameState.makeMove(newAction);
				possibleMoves.clear();

			}

			else if (!weightsOnOff) {

				int randomMove = rand() % possibleMoves.size();
				GameAction newAction(possibleMoves[randomMove], playerTurn);
				copyOfGameState.makeMove(newAction);
				possibleMoves.clear();
			}
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
		Backpropagate(1); //tells backpropagate states where ai wins are the outcome we want to increase rank
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


void Connect4AiNode::Backpropagate(int result) //updates vlaue of state and the states leading into it
{
	visits++; 

	if (result > 0) { //if positive result improve its ranking
		ranking += result;
	}

	// if we're not at the root, update the parent with the result
	if (this->parent != NULL)
		this->parent->Backpropagate(result);
}


Connect4AiNode* Connect4AiNode::FindHighestRankingChild(float explorationVal) //uses ucb formula to calculate the confidence in a node
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
		}	//makes sure the ai will take scenarios where it can make a 4 regardless

		UCBVal = (nodeWins/nodeVisits) + explorationParameter * sqrt((log(nodeParentVisits) / nodeVisits));	//confidence value in the states odds

		if (UCBVal > maxRanking) 
		{
			std::vector<int> possibleOppMoves = state.getPossibleMoves();
			bool setNewMoveAllowed = true;

			for (int j = 0; j < possibleOppMoves.size(); j++)	//check for if the state has the opponent able to play a move rthat will win the game and disallow that from being the selected branch
			{
				GameAction newAction(possibleOppMoves[j], RED);
				state.makeMove(newAction);
				if (state.checkWin() == RED)
				{
					setNewMoveAllowed = false;
				}
			}

			if (setNewMoveAllowed) {	//updates the current best option if allowed and is better than previous best
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


