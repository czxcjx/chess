all:
	g++ chess.cpp main.cpp -o chess
	./chess
test:
	g++ test.cpp chess.cpp -o test
	./test
