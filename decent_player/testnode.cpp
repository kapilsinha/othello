#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "node.hpp"
#include "player.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	Node * test = new Node();
	int x, y;
	
	srand(time(NULL));
	
	cout << rand() % 10 << endl;
	
	/*
	while(true)
	{
		cout << '\n' << test->printBoard() << endl;
		cout << '\n' << test->printMoves() << endl;
		cout << "enter move: ";
		cin >> x >> y;
		
		test = test->playMove({x, y});
	}
	*/

    return 0;
}