#include "node.hpp"

/*
 * Default Node constructor
 */
Node::Node(Side playerSide) {
	taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
	
	played = true;
	isEnd = false;
	searched = false;
	move = {8, 8};
	score =  0.0;
	side = WHITE;
	this->playerSide = playerSide;
	
	parent = nullptr;
}

/*
 * Specialized Node constructor
 */
Node::Node(Node * parent, vector<Move> toFlip, Move move, Side side, bool played) {
	this->black = parent->black;
	this->taken = parent->taken;
	this->playerSide = parent->playerSide;
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
	
	CalculateScore(getGameMoveNumber());
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

bool Node::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Node::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

/*
 * Current count of given side's stones.
 */
int Node::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Node::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Node::countWhite() {
    return taken.count() - black.count();
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
 * This implements minimax (works recursively).
 */
void Node::UpdateScore(double d)
{
	score = d;
	
	if(!parent->played)
	{
		if(side == playerSide && d > parent->score)
		{
			parent->UpdateScore(d);
		}
		else if(side != playerSide && d < parent->score)
		{
			parent->UpdateScore(d);
		}
	}
}

/*
 * This is the heuristic function.
 */
void Node::CalculateScore(int game_move)
{
	int whiteCount = taken.count() - black.count();
	int blackCount = black.count();
	int count_difference;
	double heuristic;
	// Note: the following weights for the heuristics are arbitrary. They will be adjusted later
	// Also maybe we should add a method that calculates move number (so that the heuristic can be adjusted at the end)
	if(playerSide == BLACK)
	{
		count_difference = blackCount - whiteCount;
	}
	else
	{
		count_difference = whiteCount - blackCount;
	}
	if (game_move < 52) {
		heuristic = 1 * (getWeightScore(playerSide) - getWeightScore((Side) (1 - playerSide)))
	              + 1 * (getFrontierSquaresScore(playerSide) - getFrontierSquaresScore((Side) (1 - playerSide)))
			      + 0.25 * (playerNumMoves() - opponentNumMoves());
	}
	if (game_move >= 50) { // 10 total moves left - 5 each
		heuristic = count_difference; //+ 0.5 * (playerNumMoves() - opponentNumMoves());
	}
	UpdateScore(heuristic);
}

vector<Node *> Node::getLeaves()
{
	vector<Node *> output;
	
	if(!searched)
	{
		output.push_back(this);
	}
	else
	{
		for(Node * child: children)
		{
			for(Node * leaf: child->getLeaves())
			{
				output.push_back(leaf);
			}
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
			Move move2 = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move2].push_back(result[k]);
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
			Move move2 = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move2].push_back(result[k]);
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
			Move move2 = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move2].push_back(result[k]);
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
			Move move2 = result[0];
		
			for(int k = 1; k < result.size(); k++)
			{
				moves[move2].push_back(result[k]);
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

/**
 * Returns number of moves for player
 */
int Node::playerNumMoves() { // change to numGoodMoves based on some heuristic? Probably not
    // ghetto num_moves (looks at number of moves you could have taken for this step, but since
    // we are searching a tree, the cumulative total will be accounted for so this should be ok)
    return parent->children.size();
}

/**
 * Returns number of moves for opponent
 */
int Node::opponentNumMoves() { // change to numGoodMoves based on some heuristic? Probably not
	/**
	if (parent->parent == nullptr) {
		return 0;
	}
	return parent->parent->children.size(); // interestingly, this gave an invalid move
	*/
	return children.size();
}

/**
 * Recurses through the parent nodes and returns a string containing the move history
 * formatted as a string like F5b6G4d3....
 */
string Node::printMoveHistory()
{
	string move_history = "";
	Node * current_node = this;
	while (current_node->parent != nullptr) {
		move_history = std::to_string(current_node->parent->move.y + 1) + move_history;
		if (current_node->parent->side == BLACK) {
			move_history = (char) ('A' + current_node->parent->move.x) + move_history;
		}
		else {
			move_history = (char) ('a' + current_node->parent->move.x) + move_history;
		}
		current_node = current_node->parent;
	}
	return move_history;
}

/**
 * Gets game move number
 */
int Node::getGameMoveNumber()
{
	int game_move_number = 1;
	Node * current_node = this;
	while (current_node->parent != nullptr) {
		game_move_number++;
		current_node = current_node->parent;
	}
}

/**
 * Adjusts board weights if a corner is taken - based on Board class
 */
void Node::adjustBoardWeights()
{
    if (printMoveHistory().length() == 0) {
        return;
    }
    if (occupied(0, 0) && get(side, 0, 0)) {
        board_weight[0][1] = 2; board_weight[1][0] = 2;
        if (! occupied(0, 1)) { // gaps are bad
            board_weight[0][2] = -5;
        }
        if (! occupied(1, 0)) { // gaps are bad
            board_weight[2][0] = -5;
        }
    }
    if (occupied(7, 0) && get(side, 7, 0)) {
        board_weight[7][1] = 2; board_weight[6][0] = 2;
        if (! occupied(7, 1)) { // gaps are bad
            board_weight[7][2] = -5;
        }
        if (! occupied(6, 0)) { // gaps are bad
            board_weight[5][0] = -5;
        }
    }
    if (occupied(0, 7) && get(side, 0, 7)) {
        board_weight[1][7] = 2; board_weight[0][6] = 2;
        if (! occupied(1, 7)) { // gaps are bad
            board_weight[2][7] = -5;
        }
        if (! occupied(0, 6)) { // gaps are bad
            board_weight[0][5] = -5;
        }
    }
    if (occupied(7, 7) && get(side, 7, 7)) {
        board_weight[7][6] = 2; board_weight[6][7] = 2;
        if (! occupied(7, 6)) { // gaps are bad
            board_weight[7][5] = -5;
        }
        if (! occupied(6, 7)) { // gaps are bad
            board_weight[5][7] = -5;
        }
    }
}

/**
 * Returns sum of that side's weights based on what is occupied
 */
int Node::getWeightScore(Side side)
{
    int weight_total = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (occupied(i, j) && get(side, i, j)) {
                weight_total += board_weight[i][j];
            }
        }
    }
    return weight_total;
}

/**
 * Returns the number of frontier squares (number of unoccupied squares next to the side's coins)
 */
int Node::getFrontierSquaresScore(Side side)
{
	int frontier_score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (! occupied(i, j) && i > 0 && i < 7 && j > 0 && j < 7) { // inner square
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i + 1, j + 1) && get(side, i + 1, j + 1) ||
            	  occupied(i + 1, j - 1) && get(side, i + 1, j - 1) ||
            	  occupied(i - 1, j + 1) && get(side, i - 1, j + 1) ||
            	  occupied(i - 1, j - 1) && get(side, i - 1, j - 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 0 && j > 0 && j < 7) { // left column without corners
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i + 1, j + 1) && get(side, i + 1, j + 1) ||
            	  occupied(i + 1, j - 1) && get(side, i + 1, j - 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 7 && j > 0 && j < 7) { // right column without corners
            	if (occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i - 1, j + 1) && get(side, i - 1, j + 1) ||
            	  occupied(i - 1, j - 1) && get(side, i - 1, j - 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && j == 0 && i > 0 && i < 7) { // top row without corners
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i + 1, j + 1) && get(side, i + 1, j + 1) ||
            	  occupied(i - 1, j + 1) && get(side, i - 1, j + 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && j == 7 && i > 0 && i < 7) { // bottom row without corners
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i + 1, j - 1) && get(side, i + 1, j - 1) ||
            	  occupied(i - 1, j - 1) && get(side, i - 1, j - 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 0 && j == 0) { // top left corner
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i + 1, j + 1) && get(side, i + 1, j + 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 7 && j == 0) { // top right corner
            	if (occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j + 1) && get(side, i, j + 1) ||
            	  occupied(i - 1, j + 1) && get(side, i - 1, j + 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 0 && j == 7) { // bottom left corner
            	if (occupied(i + 1, j) && get(side, i + 1, j) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i + 1, j - 1) && get(side, i + 1, j - 1)) {
            		frontier_score++;
            	}
            }
            else if (! occupied(i, j) && i == 7 && j == 7) { // bottom right corner
            	if (occupied(i - 1, j) && get(side, i - 1, j) ||
            	  occupied(i, j - 1) && get(side, i, j - 1) ||
            	  occupied(i - 1, j - 1) && get(side, i - 1, j - 1)) {
            		frontier_score++;
            	}
            }
        }
    }
    return frontier_score;
}