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
	Side playerSide; // what side is the player?
	Node * parent;
	vector<Node *> children;
	double score;
	int board_weight[8][8] = // gets adjusted as the game progresses
    {
        {15, -4, 5, 4, 4, 5, -4, 15},
        {-4, -12, -3, -2, -2, -3, -12, -4},
        {5, -3, 1, 0, 0, 1, -3, 5},
        {4, -2, 0, 1, 1, 0, -2, 4},
        {4, -2, 0, 1, 1, 0, -2, 4},
        {5, -3, 1, 0, 0, 1, -3, 5},
        {-4, -12, -3, -2, -2, -3, -12, -4},
        {15, -4, 5, 4, 4, 5, -4, 15}
    };

	Node(Side playerSide = BLACK);
	Node(Node * parent, vector<Move> toFlip, Move move, Side side, bool played = false);
	~Node();

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    int count(Side side);
    int countBlack();
    int countWhite();
	
	vector<Node *> getLeaves();
	vector<Node *> Search();
	vector< vector<Move> > SearchLine(vector<Move> MoveList);
	Node * playMove(Move toPlay);
	
	string printBoard();
	string printMoves();
	string printMoveHistory(); // in formatted form

    int getGameMoveNumber(); // returns game move number
    void adjustBoardWeights();

    int playerNumMoves();
    int opponentNumMoves();
    int getWeightScore(Side side);
    int getFrontierSquaresScore(Side side);
	
	void UpdateScore(double d);
	void CalculateScore(int game_move); // implement heuristic function here
	
};


#endif