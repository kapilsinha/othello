#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"

#include <tuple>
#include <vector>
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    // vector< tuple<Move *, Side> > move_list;
    // The above causes an error if the corresponding statement in the cpp
    // file is not commented out. After the error occurs, comment out both
    // this and the line in the cpp file

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    int numMoves(Side side);
    bool checkMove(Move *m, Side side);
    vector< tuple<int, int> > getMoves(Side side);
    void undoMove(Move *m, Side side); // NOT IMPLEMENTED
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
};

#endif
