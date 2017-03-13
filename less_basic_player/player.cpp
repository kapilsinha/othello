#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 * We should take advantage of these 30 seconds to prepare whatever we can
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    this->side = side;
	started = false;
}

Player::Player(Side side, char data[]) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    this->side = side;
	game.setBoard(data);
	started = false;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
	// Nothing here!
}

/*
 * Heuristic that accounts for number of moves available and tokens, unless testing minimax. 
 */
int Player::NaiveHeuristic(Board simulator)
{
	if(testingMinimax)
	{
		return simulator.count(side) - simulator.count((Side) (1 - side));
	}
	else
	{
		return 8*simulator.getMoves(side).size() - 8*simulator.getMoves((Side) (1 - side)).size()
			+ simulator.count(side) - simulator.count((Side) (1 - side));
	}
}

/*
 * Performs Minimax with the NaiveHeuristic, and a depth of 2.
 */
Move * Player::Minimax()
{
	Move * bestMove = nullptr;
	int bestScore = -65;
	for(tuple<int, int> move: game.getMoves(side))
	{
		Board simulator1 = *game.copy();
		Move * testMove = new Move(get<0>(move), get<1>(move));
		int currScore = 65;
		simulator1.doMove(testMove, side);
		// assume the opponent will try to minimize currScore
		for(tuple<int, int> opp_move: simulator1.getMoves((Side) (1 - side)))
		{
			Board simulator2 = *simulator1.copy();
			Move testMove2 (get<0>(opp_move), get<1>(opp_move));
			simulator2.doMove(&testMove2, (Side) (1 - side));
			if(NaiveHeuristic(simulator2) < currScore)
			{
				currScore = NaiveHeuristic(simulator2);
			}
		}
		
		if(currScore > bestScore)
		{
			bestScore = currScore;
			if(bestMove != nullptr)
			{
				Move * tempMove = bestMove;
				delete tempMove;
			}			
			bestMove = testMove;
		}
		else
		{
			delete testMove;
		}
	}
	
	return bestMove;
}

/*
 * Checks if we can place a token on the "power spots"
 */
Move *Player::checkPowerSpots(vector< tuple<int, int> > move_coords)
// this actually may be slower/worse than the original version
{
    tuple<int, int> powerspots[16];
    powerspots[0] = tuple<int, int> (0, 0);
    powerspots[1] = tuple<int, int> (7, 7);
    powerspots[2] = tuple<int, int> (0, 7);
    powerspots[3] = tuple<int, int> (7, 0);
    powerspots[4] = tuple<int, int> (0, 2);
    powerspots[5] = tuple<int, int> (0, 5);
    powerspots[6] = tuple<int, int> (2, 0);
    powerspots[7] = tuple<int, int> (2, 5);
    powerspots[8] = tuple<int, int> (5, 0);
    powerspots[9] = tuple<int, int> (5, 7);
    powerspots[10] = tuple<int, int> (7, 2);
    powerspots[11] = tuple<int, int> (7, 5);
    powerspots[12] = tuple<int, int> (2, 2);
    powerspots[13] = tuple<int, int> (2, 5);
    powerspots[14] = tuple<int, int> (5, 2);
    powerspots[15] = tuple<int, int> (5, 5);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < (int) move_coords.size(); j++) {
            if (get<0>(move_coords[j]) == get<0>(powerspots[i])
             && get<1>(move_coords[j]) == get<1>(powerspots[i])) {
                return new Move(get<0>(move_coords[j]), get<1>(move_coords[j]));
            }
        }
    }
    return nullptr;
}
/* The following is the older version but may actually be better/faster
Move *Player::checkPowerSpots()
{
    Move* powerspots[] = new Move*[16];
    powerspots[0] = new Move(0, 0);
    powerspots[1] = new Move(7, 7);
    powerspots[2] = new Move(0, 7);
    powerspots[3] = new Move(7, 0);
    powerspots[4] = new Move(0, 2);
    powerspots[5] = new Move(0, 5);
    powerspots[6] = new Move(2, 0);
    powerspots[7] = new Move(2, 5);
    powerspots[8] = new Move(5, 0);
    powerspots[9] = new Move(5, 7);
    powerspots[10] = new Move(7, 2);
    powerspots[11] = new Move(7, 5);
    powerspots[12] = new Move(2, 2);
    powerspots[13] = new Move(2, 5);
    powerspots[14] = new Move(5, 2);
    powerspots[15] = new Move(5, 5);
    for (int i = 0; i < 16; i++) {
        if (game.checkMove(powerspots[i], side)) {
            for (int j = 0; j < 16; j++) {
                if (i != j) {
                    delete powerspots[j];
                }
            }
            return powerspots[i];
        }
    }
    return nullptr;
}
*/

/*
 * Picks the move that maximizes the number of moves next turn
 * May need to account for all possible moves from opponent
 * (maybe this will be implemented in minimax)
 */
 /*
Move *Player::maximizeMoves(Board * simulated_game,
                            vector< tuple<int, int> > move_coords,
                            Side side)
{
    int max_num_moves = 0;
    int num_moves;
    Move * best_move = nullptr;
    for (int i = 0; i < (int) move_coords.size(); i++) {
        Move * move = new Move(get<0>(move_coords[i]), get<1>(move_coords[i]));
        simulated_game->doMove(move, side);
        num_moves = game.numMoves(side);
        if (num_moves > max_num_moves) {
            max_num_moves = num_moves;
            best_move = move;
        }
        simulated_game->undoMove();
        delete move;
    }
    return best_move;
}
*/

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
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
    if(started) {
        assert(game.checkMove(opponentsMove, (Side) (1 - side)));
    }
    else {
        started = true;
    }

    game.doMove(opponentsMove, (Side) (1 - side));
    
	Move * bestMove = Minimax();
	
	game.doMove(bestMove, side);
	return bestMove;

	/*
    Move *move = checkPowerSpots(move_coords);
    if (move != nullptr) {
        game.doMove(move, side);
        return move;
    }
	*/

    // The below block of code does not work
	/*
    Board * simulated_game = game.copy(); // use in maximizeMoves()
    move = maximizeMoves(simulated_game, move_coords, side);
    if (move != nullptr) {
        game.doMove(move, side);
        return move;
    }
    delete move;
    // The above block of code does not work

    for (int i = 0; i < (int) move_coords.size(); i++) {
        Move* move = new Move(get<0>(move_coords[i]), get<1>(move_coords[i]));
        game.doMove(move, side);
        return move;
    }
	*/
    
	/*
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move* move = new Move(i, j);
            if (game.checkMove(move, side)) {
                game.doMove(move, side);
                return move;
            }
            else {
                delete move;
            }
        }
    }
	
    return nullptr;	
	*/
}
