#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"

#include <vector>
#include <tuple>
using namespace std;

class Board {

private:
    bitset<64> black; // 1 means black, 0 means white
    bitset<64> taken; // 1 means taken, 0 means not taken

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    vector<SideMove> move_list; // SideMove struct defined in common.hpp

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    int numMoves(Side side);
    bool checkMove(Move *m, Side side);
    vector< tuple<int, int> > getMoves(Side side);
    void undoMove();
    void doMove(Move *m, Side side);
    int count(Side side);
    int countPowerSpots(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
};

#endif
