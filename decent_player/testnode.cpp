#include <iostream>
#include <vector>
#include "node.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	Node * test = new Node();
	int x, y;
	
	while(true)
	{
		cout << '\n' << test->printBoard() << endl;
		cout << '\n' << test->printMoves() << endl;
		cout << "enter move: ";
		cin >> x >> y;
		
		test = test->playMove({x, y});
	}

    return 0;
}