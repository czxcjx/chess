#include <iostream>
#include <cstdio>
#include "chess.hpp"

using namespace std;

int main () {
	Chess ch;
	for (;;) {
		ch.print();
		char str[10];
		scanf("%s", str);
		int x1,y1,x2,y2;
		x1 = str[0]-'a';
		y1 = str[1]-'1';
		x2 = str[2]-'a';
		y2 = str[3]-'1';
		if (ch.move(make_pair(x1,y1),make_pair(x2,y2))) {
			printf("Successful!\n");
		} else {
			printf("Illegal!\n");
		}
		switch (ch.board.query_state()) {
			case 0:
				printf("Ongoing.\n");
				break;
			case 1:
				printf("Black wins!\n");
				break;
			case 2:
				printf("Drawn!\n");
				break;
			case 3:
				printf("White wins!\n");
				break;
			default:
				printf("Wut.\n");
				break;
		}
	}
	return 0;
}
