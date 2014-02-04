#include "chess.hpp"
#include <iostream>
#include <cstdio>

using namespace std;

int main ()
{
	Chess ch;
	ch.print();
	ch.move(make_pair(5,1),make_pair(5,3));
	ch.print();
	ch.move(make_pair(4,6),make_pair(4,4));
	ch.print();
	ch.move(make_pair(6,1),make_pair(6,3));
	ch.print();
	ch.move(make_pair(3,7),make_pair(7,3));
	ch.print();
	printf("%d\n", query_state(ch.board));
}
