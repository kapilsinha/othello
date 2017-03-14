#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include <string>

#include "common.hpp"
#include "node.hpp"
using namespace std;

class Player {
	
private:
	Node * root;
	Node * current;
	Side side;

public:
    Player(Side side);
    ~Player();

	Move *openingBook(Move *opponentsMove);
    Move *doMove(Move *opponentsMove, int msLeft);
	void timedSearch(int ms);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif