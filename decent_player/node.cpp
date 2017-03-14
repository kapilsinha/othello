#include "node.hpp"

/*
 * Default Node constructor
 */
Node::Node() {
	taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
	
	played = false;
	isEnd = false;
	searched = false;
	move = {8, 8};
	side = WHITE;
	
	parent = nullptr;
}

/*
 * Specialized Node constructor
 */
Node::Node(Node * parent, vector<Move> toFlip, Move move, Side side, bool played) {
	this->black = parent->black;
	this->taken = parent->taken;
	for(Move pair: toFlip)
	{
		black.flip(pair.toInt());
	}
	
	// assume the move is valid
	this->move = move;
	this->side = side;
	this->played = played;
	this->parent = parent;
	
	Move tmp = {-1, -1};
	if(move != tmp)
	{
		taken.set(move.toInt());
		if(side == BLACK) black.set(move.toInt());
	}	
	
	isEnd = false;
	searched = false;
}

/*
 * Destructor
 */
Node::~Node() {
	for(int i = 0; i < children.size(); i++)
	{
		delete children[i];
		children[i] = nullptr;
	}
}

Node * Node::playMove(Move toPlay)
{
	if(!searched) Search();
	
	Move tmp = {-1, -1};
	if(toPlay == tmp)
	{
		children[0]->played = true;
		return children[0];
	}
	
	Node * output = nullptr;
	
	for(int i = 0; i < children.size(); i++)
	{
		if(children[i]->move == toPlay)
		{
			children[i]->played = true;
			output = children[i];
		}
		else
		{
			delete children[i];
			children[i] = nullptr;
		}
	}
	
	return output;
}

/*
 * The magic is all here... 
 */
vector<Node *> Node::Search()
{
	if(searched) return children;
	
	Side other = (Side) (1 - side);
	map< Move, vector<Move> > moves; // the key is the move, the value is the set of coins that get flipped
	
	// search -slope diagonals
	vector<Move> starts = { {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6} };
	vector<Move> ends =   { {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, {7, 6}, {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7} };
	for(int i = 0; i < starts.size(); i++)
	{
		Move pos = starts[i];
		vector<Move> line = {pos};
		
		while(pos != ends[i])
		{
			pos.x += 1;
			pos.y += 1;
			
			line.push_back(pos);
		}
		
		for(vector<Move> result: SearchLine(line))
		{
			Move move = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move].push_back(result[k]);
			}
		}		
	}
	
	// search +slope diagonals
	vector<Move> starts2 = { {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7} };
	vector<Move> ends2 =   { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {7, 6}, {7, 5}, {7, 4}, {7, 3}, {7, 2}, {7, 1} };
	for(int i = 0; i < starts2.size(); i++)
	{
		Move pos = starts2[i];
		vector<Move> line = {pos};
		
		while(pos != ends2[i])
		{
			pos.x += 1;
			pos.y -= 1;
			
			line.push_back(pos);
		}
		
		for(vector<Move> result: SearchLine(line))
		{
			Move move = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move].push_back(result[k]);
			}
		}
	}
	
	// search horizontals
	for(int i = 0; i < 8; i++) // i is row number
	{
		vector<Move> line;
		for(int j = 0; j < 8; j++)
		{
			line.push_back( {j, i} );
		}
		
		for(vector<Move> result: SearchLine(line))
		{
			Move move = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move].push_back(result[k]);
			}
		}
	}
	
	// search verticals
	for(int i = 0; i < 8; i++) // i is column number
	{
		vector<Move> line;
		for(int j = 0; j < 8; j++)
		{
			line.push_back( {i, j} );
		}
		
		for(vector<Move> result: SearchLine(line))
		{
			Move move = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move].push_back(result[k]);
			}
		}
	}
	
	// make babies!
	for (const auto &pair : moves) 
	{
        Node * child = new Node(this, pair.second, pair.first, other);
		children.push_back(child);
    }
	
	// No moves were found; pass unless the game has ended
	if(moves.size() == 0)
	{
		Move tmp = {-1, -1};
		if(move == tmp)
		{
			isEnd = true;
		}
		else
		{
			vector<Move> empty;
			Node * child = new Node(this, empty, {-1, -1}, other);
			children.push_back(child);
		}		
	}
	
	searched = true;
	
	return children;
}

/*
 * Finds moves along a line. Ugly, but efficient.
 */
vector< vector<Move> > Node::SearchLine(vector<Move> MoveList)
{
	vector< vector<Move> > output;
	vector<Move> mightFlip;
	Side other = (Side) (1 - side);
	
	int state = 0;
	for(Move pos: MoveList)
	{
		// 0 = initial state
		// 1 = the previous square was empty, and we are looking for one of theirs
		// 2 = the previous square was ours, and we are looking for one of theirs
		// 3 = the previous square was theirs, and we are looking for one of ours
		// 4 = the previous square was theirs, and we are looking for an empty
		
		// this is effectively a finite automaton, lol
		if(state == 0)
		{
			if(!taken[pos.toInt()])
			{
				mightFlip.push_back(pos);
				state = 1;
			}
			else if(!(black[pos.toInt()] != other))
			{
				state = 2;
			}
			else
			{
				// remain in state 0
			}				
		}
		else if(state == 1)
		{
			if(!taken[pos.toInt()])
			{
				mightFlip[0] = pos;
				// remain in state 1
			}
			else if(!(black[pos.toInt()] != other))
			{
				mightFlip.clear();
				state = 2;
			}
			else
			{
				mightFlip.push_back(pos);
				state = 3;
			}
		}
		else if(state == 2)
		{
			if(!taken[pos.toInt()])
			{
				mightFlip.push_back(pos);
				state = 1;
			}
			else if(!(black[pos.toInt()] != other))
			{
				// remain in state 2
			}
			else
			{
				mightFlip.push_back(pos);
				state = 4;
			}
		}
		else if(state == 3)
		{
			if(!taken[pos.toInt()])
			{
				mightFlip.clear();
				mightFlip.push_back(pos);
				state = 1;
			}
			else if(!(black[pos.toInt()] != other))
			{
				output.push_back(mightFlip);
				
				mightFlip.clear();
				state = 2;
			}
			else
			{
				mightFlip.push_back(pos);
				// remain in state 3
			}
		}
		else if(state == 4)
		{
			if(!taken[pos.toInt()])
			{
				mightFlip.insert(mightFlip.begin(), pos);
				output.push_back(mightFlip);
				
				mightFlip.clear();
				mightFlip.push_back(pos);
				state = 1;
			}
			else if(!(black[pos.toInt()] != other))
			{
				mightFlip.clear();
				state = 2;
			}
			else
			{
				mightFlip.push_back(pos);
				// remain in state 4
			}
		}
	}
	
	return output;
}

string Node::printBoard()
{
	string output = "";
	for(int i = 0; i < 8; i++) // row index
	{
		for(int j = 0; j < 8; j++) // column index
		{
			if(!taken[8*i+j])
			{
				output += '-';
			}
			else if(black[8*i+j])
			{
				output += 'x';
			}
			else
			{
				output += 'o';
			}
		}
		output += '\n';
	}
	
	return output;
}

string Node::printMoves()
{
	if(!searched) Search();
	
	string output = "";
	for(Node * child: children)
	{
		output += std::to_string(child->move.x);
		output += " ";
		output += std::to_string(child->move.y);
		output += "\n";
	}
	
	return output;
}