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

	Node();
	Node(Node * parent, vector<Move> toFlip, Move move, Side side, bool played = false);
	~Node();
	
	vector<Node *> Search();
	vector< vector<Move> > SearchLine(vector<Move> MoveList);
	Node * playMove(Move toPlay);
	
	string printBoard();
	string printMoves();
	
	/* TODO:
	void UpdateScore(double d);
	void CalculateScore(); // implement heuristic function here
	*/
	
};


#endif