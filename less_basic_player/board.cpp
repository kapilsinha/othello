#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/**
 * Adjusts board weights if a corner is taken
 */
void Board::adjustBoardWeights(Side side)
{
    if (move_history.size() == 0) {
        return;
    }
    if (occupied(0, 0) && get(side, 0, 0)) {
        board_weight[0][1] = 2; board_weight[1][0] = 2;
        if (! occupied(0, 1)) { // gaps are bad
            board_weight[0][2] = -3;
        }
        if (! occupied(1, 0)) { // gaps are bad
            board_weight[2][0] = -3;
        }
    }
    if (occupied(7, 0) && get(side, 7, 0)) {
        board_weight[7][1] = 2; board_weight[6][0] = 2;
        if (! occupied(7, 1)) { // gaps are bad
            board_weight[7][2] = -3;
        }
        if (! occupied(6, 0)) { // gaps are bad
            board_weight[5][0] = -3;
        }
    }
    if (occupied(0, 7) && get(side, 0, 7)) {
        board_weight[1][7] = 2; board_weight[0][6] = 2;
        if (! occupied(1, 7)) { // gaps are bad
            board_weight[2][7] = -3;
        }
        if (! occupied(0, 6)) { // gaps are bad
            board_weight[0][5] = -3;
        }
    }
    if (occupied(7, 7) && get(side, 7, 7)) {
        board_weight[7][6] = 2; board_weight[6][7] = 2;
        if (! occupied(7, 6)) { // gaps are bad
            board_weight[7][5] = -3;
        }
        if (! occupied(6, 7)) { // gaps are bad
            board_weight[5][7] = -3;
        }
    }
}

/**
 * Returns sum of that side's weights based on what is occupied
 */
int Board::getWeightScore(Side side)
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
 * Returns number of moves // Added this
 */
int Board::numMoves(Side side) { // change to numGoodMoves based on some heuristic?
    // perhaps not required later due to the added function getMoves()
    int num_moves = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) {
                num_moves++;
            }
        }
    }
    return num_moves;
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Returns list of possible moves in tuples containing the coordinates
 */
vector< tuple<int, int> > Board::getMoves(Side side) {
    vector< tuple<int, int> > moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tuple<int, int> move_coord (i, j);
            Move move(i, j);
            if (checkMove(&move, side)) {
                moves.push_back(move_coord);
            }
        }
    }
    return moves;
}

/*
 * Undos last move (to be used for simulated boards)
 */
/*
void Board::undoMove() {
    SideMove s = move_history.back(); // remove move from list
	move_history.pop_back();
    int x = s.move.getX();
    int y = s.move.getY();
    taken.set(x + 8*y, 0); // set space to not taken
    black.set(x + 8*y, 0); // set space to white
    // the above should not be necessary but it is there as a precaution
}
*/

vector<SideMove> Board::getMoveHistory()
{
    return move_history;
}

/**
 * Converts move_history vector into a string equivalent
 * (to be used in the openings books)
 */
char * Board::encodeMoveHistory(vector<SideMove> moves) // DOESN'T WORK!
{
    char * move_list = new char[moves.size() * 2];
    for (int i = 0; i < (int) moves.size(); i++) {
        if (moves[i].side == BLACK) {
            move_list[i] = 'A' - 1 + moves[i].move.getX();
            move_list[i + 1] = moves[i].move.getY() + 1;
        }
        else { // (moves[i].side == WHITE)
            move_list[i] = 'a' - 1 + moves[i].move.getX();
            move_list[i + 1] = moves[i].move.getY() + 1;
        }
    }
    return move_list;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;
    
    SideMove inputMove = {*m, side};
    move_history.push_back(inputMove); // added this
    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
