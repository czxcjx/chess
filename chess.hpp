#ifndef CHESS_H
#define CHESS_H

#include <utility>
#include <string>
#include <cstring>
#include <list>

enum ChessPiece {
	PIECE_EMPTY,
	PIECE_WPAWN,
	PIECE_WROOK,
	PIECE_WKNIGHT,
	PIECE_WBISHOP,
	PIECE_WQUEEN,
	PIECE_WKING,
	PIECE_BPAWN,
	PIECE_BROOK,
	PIECE_BKNIGHT,
	PIECE_BBISHOP,
	PIECE_BQUEEN,
	PIECE_BKING
};

//TODO: Maybe get rid of these?
typedef std::pair<int,int> pii;
typedef std::pair<pii,pii> ppii;



//TODO: implement 3fold rep and 50move draw
struct Board {
	ChessPiece grid[8][8]; //a1 - [0][0], a8 = [0][7]
	int enpassant;
	bool castling[2][3];
	int turn;
};

int side(ChessPiece c);
void shift_piece(Board & board, pii s, pii e, ChessPiece promote = PIECE_EMPTY);
void gen_trymoves(const Board & board, std::list<ppii> & moves);
void cull_illegal_moves(const Board & board, std::list<ppii> & moves);
void gen_moves(const Board & board, std::list<ppii> & legal_moves);
int query_state(const Board & board);

void print_board(const Board & board);

class Chess {
	public:
		Board board;
		Chess();
		~Chess();

		bool move(pii starting_square, pii ending_square,ChessPiece promotion=PIECE_EMPTY);
		bool move(std::string move_string);

		void print();
};

#endif //CHESS_H
