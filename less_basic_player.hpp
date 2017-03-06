#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <assert.h>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {
	
private:
	Board game;
	Side side;
	bool started;

public:
    Player(Side side);
    ~Player();

    // Move *checkPowerSpots();
    Move *Player::checkPowerSpots(vector< tuple<int, int> > move_coords);
    Move *Player::maximizeMoves(Board * simulated_game, vector< tuple<int, int> > move_coords);
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
