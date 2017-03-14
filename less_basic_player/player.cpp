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
	if(testingMinimax) // why? Maybe change to this basic heuristic in the last few moves?
	{
		return simulator.count(side) - simulator.count((Side) (1 - side));
	}
	else
	{
		return 3 * simulator.getMoves(side).size() - 3 * simulator.getMoves((Side) (1 - side)).size()
               + 3 * simulator.getWeightScore(side) - 3 * simulator.getWeightScore((Side) (1 - side))
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
    // cout << game.encodeMoveHistory(game.getMoveHistory()) << endl; // doesn't work for some reason
    
    // WE HAVE TO CHECK IF THERE IS A KILL MOVE (TAKE ALL OF THE OPPONENT'S PIECES)
    // ALSO WE HAVE TO MAKE SURE THAT WE CANNOT GET KILLED OFF

	Move * bestMove = Minimax();
	
	game.doMove(bestMove, side);
	return bestMove;

}
