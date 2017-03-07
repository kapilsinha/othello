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

/*
 * Destructor for the player.
 */
Player::~Player() {
	// Nothing here!
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
        // WE NEED TO ADD AN UNDO MOVE METHOD TO BOARD
        // I AM PRETENDING THAT THIS METHOD EXISTS FOR NOW BUT IT DOES NOT!
        // IT WILL REQUIRE US TO ADD A VARIABLE/ARRAY last_move(s) TO BOARD
        // Add this function to doMove() when the above works with undoMove()
        simulated_game->undoMove(move, side);
        delete move;
    }
    return best_move;
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
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
    if(started) {
        assert(game.checkMove(opponentsMove, (Side) (1 - side)));
    }
    else {
        started = true;
    }

    game.doMove(opponentsMove, (Side) (1 - side));
    // Board * simulated_game = game.copy(); // use in maximizeMoves()

    // not-so-intelligent AI: returns the first move found
    vector< tuple<int, int> > move_coords = game.getMoves(side);
    Move *move = checkPowerSpots(move_coords);
    if (move != nullptr) {
        game.doMove(move, side);
        return move;
    }
    delete move;
    for (int i = 0; i < (int) move_coords.size(); i++) {
        Move* move = new Move(get<0>(move_coords[i]), get<1>(move_coords[i]));
        game.doMove(move, side);
        return move;
    }
    
    /**
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
    */
    return nullptr;	
}
