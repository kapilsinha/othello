#include "player.hpp"
bool in_openings_book = true; // global variable for which game move we are on

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    this->side = side;
	root = new Node(side);
	current = root;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	delete root;
}

void Player::timedSearch(int ms)
{
	clock_t start = clock();
	
	do
	{
		vector<Node *> leaves = current->getLeaves();
		
		for(Node * leaf: leaves)
		{
			leaf->Search();
			if((clock() - start) / (double) CLOCKS_PER_SEC > ms / (double) 1000) break;
		}
	}
	while((clock() - start) / (double) CLOCKS_PER_SEC < ms / (double) 1000);

}

/*
 * Consults openings book to make a move if possible
 * @param: string move_history must match the format of the list below
 */
Move *Player::openingBook(string move_history)
{
	// Ghetto list for the win - let's just say we prioritize speed over cleanliness
	string openings_list[] = {"C4c3", "C4c3D3c5B2", "C4c3D3c5B3", "C4c3D3c5B3f4B5b4C6d6F5", "C4c3D3c5B4", "C4c3D3c5B4d2C2f4D6c6F5e6F7", "C4c3D3c5B4d2D6", "C4c3D3c5B4d2E2", "C4c3D3c5B4e3", "C4c3D3c5B5", "C4c3D3c5B6c6B5", "C4c3D3c5B6e3", "C4c3D3c5D6", "C4c3D3c5D6e3", "C4c3D3c5D6f4B4", "C4c3D3c5D6f4B4b6B5c6B3", "C4c3D3c5D6f4B4b6B5c6F5", "C4c3D3c5D6f4B4c6B5b3B6e3C2a4A5a6D2", "C4c3D3c5D6f4B4e3B3", "C4c3D3c5D6f4F5", "C4c3D3c5D6f4F5d2", "C4c3D3c5D6f4F5d2B5", "C4c3D3c5D6f4F5d2G4d7", "C4c3D3c5D6f4F5e6C6d7", "C4c3D3c5D6f4F5e6F6", "C4c3D3c5F6", "C4c3D3c5F6e2C6", "C4c3D3c5F6e3C6f5F4g5", "C4c3D3c5F6f5", "C4c3D3e3B2", "C4c3D3e3C2", "C4c3D3e3C2d6E2d2F3f4E6", "C4c3D3e3D2", "C4c3D3e3D2b4B3d6F4f3E6f5G6", "C4c3D3e3D2b4B5", "C4c3D3e3D2b4F4", "C4c3D3e3D2c5", "C4c3D3e3E2", "C4c3D3e3F2c5", "C4c3D3e3F2f3E2", "C4c3D3e3F4", "C4c3D3e3F4c5", "C4c3D3e3F4d6D2", "C4c3D3e3F4d6D2c5C2", "C4c3D3e3F4d6D2f2E2f3C2", "C4c3D3e3F4d6D2f2E2f3E6", "C4c3D3e3F4d6D2f3E2c2F2c5B3d1E1f1B4", "C4c3D3e3F4d6E6", "C4c3D3e3F4d6E6b4", "C4c3D3e3F4d6E6b4D7g4", "C4c3D3e3F4d6E6b4E2", "C4c3D3e3F4d6E6f5F3g4", "C4c3D3e3F4d6E6f5F6", "C4c3D3e3F6", "C4c3D3e3F6b5F3", "C4c3D3e3F6c5F3e6D6e7", "C4c3D3e3F6e6", "C4c3E6c5", "C4c3F5c5", "C4c5", "C4e3", "C4e3F4c5D6e6", "C4e3F4c5D6f3C6", "C4e3F4c5D6f3D3", "C4e3F4c5D6f3D3c3", "C4e3F4c5D6f3E2", "C4e3F4c5D6f3E6c3D3e2", "C4e3F4c5D6f3E6c3D3e2B5", "C4e3F4c5D6f3E6c3D3e2B5f5", "C4e3F4c5D6f3E6c3D3e2B5f5B3", "C4e3F4c5D6f3E6c3D3e2B5f5B4f6C2e7D2c7", "C4e3F4c5D6f3E6c3D3e2B6f5", "C4e3F4c5D6f3E6c3D3e2B6f5B4f6G5d7", "C4e3F4c5D6f3E6c3D3e2B6f5G5", "C4e3F4c5D6f3E6c3D3e2B6f5G5f6", "C4e3F4c5D6f3E6c3D3e2D2", "C4e3F4c5D6f3E6c6", "C4e3F4c5E6", "C4e3F5b4", "C4e3F5b4F3", "C4e3F5b4F3f4E2e6G5f6D6c6", "C4e3F5e6D3", "C4e3F5e6F4", "C4e3F5e6F4c5D6c6F7f3", "C4e3F5e6F4c5D6c6F7g5G6", "C4e3F6b4", "C4e3F6e6F5", "C4e3F6e6F5c5C3", "C4e3F6e6F5c5C3b4", "C4e3F6e6F5c5C3b4D6c6B5a6B6c7", "C4e3F6e6F5c5C3c6", "C4e3F6e6F5c5C3g5", "C4e3F6e6F5c5D3", "C4e3F6e6F5c5D6", "C4e3F6e6F5c5F4g5G4f3C6d3D6", "C4e3F6e6F5c5F4g5G4f3C6d3D6b3C3b4E2b6", "C4e3F6e6F5c5F4g6F7", "C4e3F6e6F5c5F4g6F7d3", "C4e3F6e6F5g6", "D3c3", "D3c3C4c5B2", "D3c3C4c5B3", "D3c3C4c5B3f4B5b4C6d6F5", "D3c3C4c5B4", "D3c3C4c5B4d2C2f4D6c6F5e6F7", "D3c3C4c5B4d2D6", "D3c3C4c5B4d2E2", "D3c3C4c5B4e3", "D3c3C4c5B5", "D3c3C4c5B6c6B5", "D3c3C4c5B6e3", "D3c3C4c5D6", "D3c3C4c5D6e3", "D3c3C4c5D6f4B4", "D3c3C4c5D6f4B4b6B5c6B3", "D3c3C4c5D6f4B4b6B5c6F5", "D3c3C4c5D6f4B4c6B5b3B6e3C2a4A5a6D2", "D3c3C4c5D6f4B4e3B3", "D3c3C4c5D6f4F5", "D3c3C4c5D6f4F5d2", "D3c3C4c5D6f4F5d2B5", "D3c3C4c5D6f4F5d2G4d7", "D3c3C4c5D6f4F5e6C6d7", "D3c3C4c5D6f4F5e6F6", "D3c3C4c5F6", "D3c3C4c5F6e2C6", "D3c3C4c5F6e3C6f5F4g5", "D3c3C4c5F6f5", "D3c3C4e3B2", "D3c3C4e3C2", "D3c3C4e3C2d6E2d2F3f4E6", "D3c3C4e3D2", "D3c3C4e3D2b4B3d6F4f3E6f5G6", "D3c3C4e3D2b4B5", "D3c3C4e3D2b4F4", "D3c3C4e3D2c5", "D3c3C4e3E2", "D3c3C4e3F2c5", "D3c3C4e3F2f3E2", "D3c3C4e3F4", "D3c3C4e3F4c5", "D3c3C4e3F4d6D2", "D3c3C4e3F4d6D2c5C2", "D3c3C4e3F4d6D2f2E2f3C2", "D3c3C4e3F4d6D2f2E2f3E6", "D3c3C4e3F4d6D2f3E2c2F2c5B3d1E1f1B4", "D3c3C4e3F4d6E6", "D3c3C4e3F4d6E6b4", "D3c3C4e3F4d6E6b4D7g4", "D3c3C4e3F4d6E6b4E2", "D3c3C4e3F4d6E6f5F3g4", "D3c3C4e3F4d6E6f5F6", "D3c3C4e3F6", "D3c3C4e3F6b5F3", "D3c3C4e3F6c5F3e6D6e7", "D3c3C4e3F6e6", "D3c3E6e3", "D3c3F5e3", "D3c5", "D3c5D6e3F4c6B5", "D3c5D6e3F4c6C4", "D3c5D6e3F4c6C4c3", "D3c5D6e3F4c6F3", "D3c5D6e3F4c6F5c3C4b5", "D3c5D6e3F4c6F5c3C4b5B4", "D3c5D6e3F4c6F5c3C4b5E2", "D3c5D6e3F4c6F5c3C4b5E2e6", "D3c5D6e3F4c6F5c3C4b5E2e6C2", "D3c5D6e3F4c6F5c3C4b5E2e6D2f6B3g5B4g3", "D3c5D6e3F4c6F5c3C4b5F2e6", "D3c5D6e3F4c6F5c3C4b5F2e6D2f6E7g4", "D3c5D6e3F4c6F5c3C4b5F2e6E7", "D3c5D6e3F4c6F5c3C4b5F2e6E7f6", "D3c5D6e3F4c6F5f3", "D3c5D6e3F4f5", "D3c5D6e3F5", "D3c5E6d2", "D3c5E6d2C6", "D3c5E6d2C6d6B5f5E7f6F4f3", "D3c5E6f5C4", "D3c5E6f5D6", "D3c5E6f5D6e3F4f3G6c6", "D3c5E6f5D6e3F4f3G6e7F7", "D3c5F6d2", "D3c5F6f5E6", "D3c5F6f5E6e3C3", "D3c5F6f5E6e3C3d2", "D3c5F6f5E6e3C3d2F4f3E2f1F2g3", "D3c5F6f5E6e3C3e7", "D3c5F6f5E6e3C3f3", "D3c5F6f5E6e3C4", "D3c5F6f5E6e3D6e7D7c6F3c4F4", "D3c5F6f5E6e3D6e7D7c6F3c4F4c2C3d2B5f2", "D3c5F6f5E6e3D6f7G6", "D3c5F6f5E6e3D6f7G6c4", "D3c5F6f5E6e3F4", "D3c5F6f5E6f7", "D3e3", "E6d6", "E6f4", "E6f4C3c4D3", "E6f4C3c4D3c2", "E6f4C3c4D3d6C5", "E6f4C3c4D3d6E3c2B3", "E6f4C3c4D3d6E3c2B3f5", "E6f4C3c4D3d6E3d2E2f3C6f5C5", "E6f4C3c4D3d6E3d2E2f3C6f5C5f7F6e7G4c7", "E6f4C3c4D3d6F5", "E6f4C3c4D3d6F6", "E6f4C3c4D3d6F6c6", "E6f4C3c4D3d6F6d2", "E6f4C3c4D3d6F6e7", "E6f4C3c4D3d6F6e7C5c6D7c8C7b6", "E6f4C3e7", "E6f4D3c4E3", "E6f4D3c4E3d6C5c6B3d2C2", "E6f4D3c4E3d6C5c6B3f3", "E6f4D3c4F5", "E6f4D3e7", "E6f4D3e7F3", "E6f4D3e7F3e3G4c4D2c3C5c6", "E6f4E3d6C4", "E6f4E3d6C5c4", "E6f4E3d6C5f3C4c6", "E6f4E3d6C5f3C4f6F5g4", "E6f4E3d6C5f3C4f6F5g4C7d3", "E6f4E3d6C5f3C4f6F5g4C7d3D2", "E6f4E3d6C5f3C4f6F5g4C7d3D2c3", "E6f4E3d6C5f3C4f6F5g4C7d3E7c3D2b5", "E6f4E3d6C5f3C4f6F5g4D7", "E6f4E3d6C5f3C4f6F5g4D7d3", "E6f4E3d6C5f3C4f6F5g4D7d3E7c3G6b4G5b6", "E6f4E3d6C5f3C4f6F5g4D7d3F7", "E6f4E3d6C5f3C4f6F5g4G5", "E6f4E3d6C5f3C6", "E6f4E3d6C5f3F5", "E6f4E3d6C5f3F5f6", "E6f4E3d6C5f3G4", "E6f6", "E6f6C4d6", "E6f6D3d6", "E6f6F5d6C3", "E6f6F5d6C3d3", "E6f6F5d6C3f4C6d3E3d2", "E6f6F5d6C3g4C6", "E6f6F5d6C5", "E6f6F5d6C5e3D3", "E6f6F5d6C5e3D3c4C3", "E6f6F5d6C5e3D3c4C6b5", "E6f6F5d6C5e3D3g5", "E6f6F5d6C5e3D3g5D7", "E6f6F5d6C5e3D3g5E2b5", "E6f6F5d6C5e3E7", "E6f6F5d6C5e3E7c6D7f7C7f4G6e8D8c8G5", "E6f6F5d6C5e3E7c7D7c6D3", "E6f6F5d6C5e3E7c7D7c6F7", "E6f6F5d6C5e3E7f4F7", "E6f6F5d6C5f4", "E6f6F5d6C7c6D7", "E6f6F5d6C7f4", "E6f6F5d6D7", "E6f6F5d6E7", "E6f6F5d6E7f4", "E6f6F5d6E7g5C5", "E6f6F5d6E7g5G4", "E6f6F5d6E7g5G6e3C5c6D3c4B3", "E6f6F5d6F7", "E6f6F5d6F7e3D7e7C6c5D3", "E6f6F5d6G7", "E6f6F5f4C3", "E6f6F5f4C3c4", "E6f6F5f4C3d6F3c4C5b4", "E6f6F5f4C3d7F3", "E6f6F5f4E3", "E6f6F5f4E3c5C4", "E6f6F5f4E3c5C4d3C3", "E6f6F5f4E3c5C4d3F3e2", "E6f6F5f4E3c5C4e7", "E6f6F5f4E3c5C4e7B5e2", "E6f6F5f4E3c5C4e7G4", "E6f6F5f4E3c5G5", "E6f6F5f4E3c5G5d6G6", "E6f6F5f4E3c5G5f3G4g6G3d6F7h5H4h3E7", "E6f6F5f4E3c5G5g3G4f3C4", "E6f6F5f4E3c5G5g3G4f3G6", "E6f6F5f4E3d6", "E6f6F5f4G3d6", "E6f6F5f4G3f3G4", "E6f6F5f4G4", "E6f6F5f4G5", "E6f6F5f4G5d6", "E6f6F5f4G5e7D7", "E6f6F5f4G5e7E3", "E6f6F5f4G5e7F7c5E3f3C4d3C2", "E6f6F5f4G6", "E6f6F5f4G6c5G4g5F3e3C4", "E6f6F5f4G7", "F5d6", "F5d6C3d3C4", "F5d6C3d3C4b3", "F5d6C3d3C4f4C5b3C2", "F5d6C3d3C4f4C5b3C2e6", "F5d6C3d3C4f4C5b4B5c6F3e6E3", "F5d6C3d3C4f4C5b4B5c6F3e6E3g6F6g5D7g3", "F5d6C3d3C4f4E3", "F5d6C3d3C4f4E6", "F5d6C3d3C4f4F6", "F5d6C3d3C4f4F6b4", "F5d6C3d3C4f4F6f3", "F5d6C3d3C4f4F6g5", "F5d6C3d3C4f4F6g5E3f3G4h3G3f2", "F5d6C3g5", "F5d6C4d3C5", "F5d6C4d3C5f4E3f3C2b4B3", "F5d6C4d3C5f4E3f3C2c6", "F5d6C4d3E6", "F5d6C4g5", "F5d6C4g5C6", "F5d6C4g5C6c5D7d3B4c3E3f3", "F5d6C5f4D3", "F5d6C5f4E3c6D3f3", "F5d6C5f4E3c6D3f6E6d7", "F5d6C5f4E3c6D3f6E6d7E7", "F5d6C5f4E3c6D3f6E6d7G3c4", "F5d6C5f4E3c6D3f6E6d7G3c4B4", "F5d6C5f4E3c6D3f6E6d7G3c4B4c3", "F5d6C5f4E3c6D3f6E6d7G3c4G5c3B4e2", "F5d6C5f4E3c6D3f6E6d7G4", "F5d6C5f4E3c6D3f6E6d7G4c4", "F5d6C5f4E3c6D3f6E6d7G4c4G5c3F7d2E7f2", "F5d6C5f4E3c6D3f6E6d7G4c4G6", "F5d6C5f4E3c6D7", "F5d6C5f4E3c6E6", "F5d6C5f4E3c6E6f6", "F5d6C5f4E3c6F3", "F5d6C5f4E3d3", "F5f4", "F5f6", "F5f6C4f4", "F5f6D3f4", "F5f6E6d6C3", "F5f6E6d6C3d3", "F5f6E6d6C3f4C6d3E3d2", "F5f6E6d6C3g4C6", "F5f6E6d6C5", "F5f6E6d6C5e3D3", "F5f6E6d6C5e3D3c4C3", "F5f6E6d6C5e3D3c4C6b5", "F5f6E6d6C5e3D3g5", "F5f6E6d6C5e3D3g5D7", "F5f6E6d6C5e3D3g5E2b5", "F5f6E6d6C5e3E7", "F5f6E6d6C5e3E7c6D7f7C7f4G6e8D8c8G5", "F5f6E6d6C5e3E7c7D7c6D3", "F5f6E6d6C5e3E7c7D7c6F7", "F5f6E6d6C5e3E7f4F7", "F5f6E6d6C5f4", "F5f6E6d6C7c6D7", "F5f6E6d6C7f4", "F5f6E6d6D7", "F5f6E6d6E7", "F5f6E6d6E7f4", "F5f6E6d6E7g5C5", "F5f6E6d6E7g5G4", "F5f6E6d6E7g5G6e3C5c6D3c4B3", "F5f6E6d6F7", "F5f6E6d6F7e3D7e7C6c5D3", "F5f6E6d6G7", "F5f6E6f4C3", "F5f6E6f4C3c4", "F5f6E6f4C3d6F3c4C5b4", "F5f6E6f4C3d7F3", "F5f6E6f4E3", "F5f6E6f4E3c5C4", "F5f6E6f4E3c5C4d3C3", "F5f6E6f4E3c5C4d3F3e2", "F5f6E6f4E3c5C4e7", "F5f6E6f4E3c5C4e7B5e2", "F5f6E6f4E3c5C4e7G4", "F5f6E6f4E3c5G5", "F5f6E6f4E3c5G5d6G6", "F5f6E6f4E3c5G5f3G4g6G3d6F7h5H4h3E7", "F5f6E6f4E3c5G5g3G4f3C4", "F5f6E6f4E3c5G5g3G4f3G6", "F5f6E6f4E3d6", "F5f6E6f4G3d6", "F5f6E6f4G3f3G4", "F5f6E6f4G4", "F5f6E6f4G5", "F5f6E6f4G5d6", "F5f6E6f4G5e7D7", "F5f6E6f4G5e7E3", "F5f6E6f4G5e7F7c5E3f3C4d3C2", "F5f6E6f4G6", "F5f6E6f4G6c5G4g5F3e3C4", "F5f6E6f4G7"};
	string opening_move = "0";
	for (int i = 0; i < 396; i++) {
		if (openings_list[i].length() > move_history.length()) {
			if (move_history == openings_list[i].substr(0, move_history.length())) {
				opening_move = openings_list[i].substr(move_history.length(), move_history.length() + 2);
				break;
			}
		}
	}
	if (opening_move == "0") {
		return nullptr;
	}
	// convert string to move
	if ('a' > opening_move[0]) {
		return new Move((int) (opening_move[0] - 'A'), (int) (opening_move[1] - '0') - 1);
	}
	return new Move((int) (opening_move[0] - 'a'), (int) (opening_move[1] - '0') - 1);
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
    if(current != root || oppMove != nullptr)
	{
		if(oppMove == nullptr)
		{
			current = current->playMove({-1, -1});
		}
		else
		{
			current = current->playMove(*oppMove);
		}
	}
	/**
	if (in_openings_book) {
		Move * opening = openingBook(current->printMoveHistory());
		if (opening == nullptr) {
			in_openings_book = false;
		}
		else {
			return opening;
		}
	}
    */
	timedSearch(50);
	
	// reasonable AI: plays best move according to basic heuristics
	vector<Node *> children = current->Search();
	double bestScore = -100.0;
	Node * bestMove = nullptr;
	for(Node * child: children)
	{
		if(child->score > bestScore)
		{
			bestScore = child->score;
			bestMove = child;
		}
	}
	
	Move * willPlay = new Move(bestMove->move.x, bestMove->move.y);
	current = current->playMove(*willPlay);
	
	Move tmp = {-1, -1};
	if(*willPlay == tmp) return nullptr;
	
	return willPlay;
	
}