#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    this->side = side;
	root = new Node();
	current = root;
	leaves = {&root};
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete root;
}

/*
 * Remove deleted leaves.
 */
void Player::pruneLeaves() {
	vector<Node **> newLeaves;
	for(Node ** leaf: leaves)
	{
		if(*leaf != nullptr) newLeaves.push_back(leaf);
	}
	leaves = newLeaves;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *oppMove, int msLeft)
{
	// we do not have the first move
    if(current != root)
	{
		if(oppMove == nullptr)
		{
			current = current->playMove({-1, -1});
		}
		else
		{
			current = current->playMove(*oppMove);
		}
		pruneLeaves();
	}
	
	// stupid AI: plays the first move found
	vector<Node *> children = current->Search();
	
	Move * willPlay = new Move(children[0]->move.x, children[0]->move.y);
	current = current->playMove(*willPlay);
	
	Move tmp = {-1, -1};
	if(*willPlay == tmp) return nullptr;
	
	return willPlay;
	
}