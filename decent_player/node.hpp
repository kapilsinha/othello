#ifndef __NODE_H__
#define __NODE_H__

#include <bitset>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "common.hpp"
using namespace std;

class Node {
	
public:
	bitset<64> black;
	bitset<64> taken;
	Move move;
	Side side;
	bool played; // has this move been played yet?
	bool isEnd; // has the game ended?
	bool searched; // has Search() been called yet?
	Node * parent;
	vector<Node *> children;
	double score;
	int board_weight[8][8] = // gets adjusted as the game progresses
    {
        {4, -3, 2, 2, 2, 2, -3, 4},
        {-3, -4, -1, -1, -1, -1, -4, -3},
        {2, -1, 1, 0, 0, 1, -1, 2},
        {2, -1, 0, 1, 1, 0, -1, 2},
        {2, -1, 0, 1, 1, 0, -1, 2},
        {2, -1, 1, 0, 0, 1, -1, 2},
        {-3, -4, -1, -1, -1, -1, -4, -3},
        {4, -3, 2, 2, 2, 2, -3, 4}
    };

	Node();
	Node(Node * parent, vector<Move> toFlip, Move move, Side side, bool played = false);
	~Node();

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    int count(Side side);
    int countBlack();
    int countWhite();
	
	vector<Node *> Search();
	vector< vector<Move> > SearchLine(vector<Move> MoveList);
	Node * playMove(Move toPlay);
	
	string printBoard();
	string printMoves();
	string printMoveHistory(); // in formatted form

    void adjustBoardWeights();

    int numMoves();
    int getWeightScore();
	
	/* TODO:
	void UpdateScore(double d);
	void CalculateScore(); // implement heuristic function here
	*/
	
};


#endif