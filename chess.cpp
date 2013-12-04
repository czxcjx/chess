#include "chess.hpp"

#include <cstring>
#include <cstdio>

int side(ChessPiece c);
char print_piece(ChessPiece p);

Chess::Chess() {
	board.grid[0][0] = board.grid[7][0] = PIECE_WROOK;
	board.grid[1][0] = board.grid[6][0] = PIECE_WKNIGHT;
	board.grid[2][0] = board.grid[5][0] = PIECE_WBISHOP;
	board.grid[3][0] = PIECE_WQUEEN;
	board.grid[4][0] = PIECE_WKING;
	board.grid[0][7] = board.grid[7][7] = PIECE_BROOK;
	board.grid[1][7] = board.grid[6][7] = PIECE_BKNIGHT;
	board.grid[2][7] = board.grid[5][7] = PIECE_BBISHOP;
	board.grid[3][7] = PIECE_BQUEEN;
	board.grid[4][7] = PIECE_BKING;
	for (int i = 0; i < 8; i++)
	{
		board.grid[i][1] = PIECE_WPAWN;
		board.grid[i][6] = PIECE_BPAWN;
		for (int j = 2; j <= 5; j++) board.grid[i][j] = PIECE_EMPTY;
	}
	board.turn = 0;
	for (int i = 0; i < 3; i++) board.castling[0][i] = board.castling[1][i] = true;
	board.enpassant = -1;
}
Chess::~Chess() {
}

int side(ChessPiece c) {
	if (c==PIECE_EMPTY) return 2;
	else if (c>=PIECE_WPAWN && c<=PIECE_WKING) return 0;
	return 1;
}
char print_piece(ChessPiece p) {
	switch (p) {
		case PIECE_EMPTY:
			return '.';
		case PIECE_WPAWN:
			return 'p';
		case PIECE_WROOK:
			return 'r';
		case PIECE_WKNIGHT:
			return 'n';
		case PIECE_WBISHOP:
			return 'b';
		case PIECE_WQUEEN:
			return 'q';
		case PIECE_WKING:
			return 'k';
		case PIECE_BPAWN:
			return 'P';
		case PIECE_BROOK:
			return 'R';
		case PIECE_BKNIGHT:
			return 'N';
		case PIECE_BBISHOP:
			return 'B';
		case PIECE_BQUEEN:
			return 'Q';
		case PIECE_BKING:
			return 'K';
	}
	return '.';
}

void Board::print() const {
	for (int y = 7; y >= 0; y--) {
		for (int x = 0; x < 8; x++) {
			printf("%c", print_piece(grid[x][y]));
		}
		printf("\n");
	}
	return;
}

void Chess::print() const {
	board.print();
	return;
}


bool Board::is_attacked(pii sq) const {
	std::list<ppii> moves;
	gen_trymoves(moves);
	for (std::list<ppii>::iterator it = moves.begin(); it != moves.end(); it++) {
		if (it->second == sq) return true;
	}
	return false;
}

void Board::gen_moves(std::list<ppii> & legal_moves) const {
	gen_trymoves(legal_moves);
	cull_illegal_moves(legal_moves);
	return;
}

int Board::query_state() const {
	//0 - ongoing, 1 - black wins, 2 - drawn, 3 - white wins
	std::list<ppii> moves;
	gen_moves(moves);
	for (std::list<ppii>::iterator it = moves.begin(); it != moves.end(); it++) {
	}
	printf("\n");
	if (!moves.empty()) return 0;
	int kx=-1,ky=-1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; (j<8)&&(kx==-1); j++) {
			if ((grid[i][j]==PIECE_WKING&&(turn%2==0)) ||
					(grid[i][j]==PIECE_BKING&&(turn%2==1)) ) {
						kx = i;
						ky = j;
						break;
			}
		}
	}
	Board nb;
	memcpy(&nb,this,sizeof(nb));
	nb.turn++;
	if (nb.is_attacked(std::make_pair(kx,ky))) {
		if (turn%2==0) return 1;
		else return 3;
	} else {
		return 2;
	}
	return 0;
}

//Assumes correct input
void Board::shift_piece(pii s, pii e, ChessPiece promote) {
	int x1=s.first,y1=s.second,x2=e.first,y2=e.second;

	ChessPiece piece = grid[x1][y1];
	//Fixing the castling
	if (piece==PIECE_WKING) castling[0][0] = false;
	else if (piece==PIECE_BKING) castling[1][0] = false;
	else if (piece==PIECE_WROOK) {
		if (x1==0&&y1==0) castling[0][1] = false;
		else if (x1==7&&y1==0) castling[0][2] = false;
	} else if (piece==PIECE_BROOK) {
		if (x1==0&&y1==7) castling[1][1] = false;
		else if (x1=7&&y1==7) castling[1][2] = false;
	}
	//Setting enpassant file
	if (piece==PIECE_WPAWN&&x1==x2&&y2==3&&y1==1) {
		enpassant = x1;
	} else if (piece==PIECE_BPAWN&&x1==x2&&y1==6&&y2==4) {
		enpassant = x1;
	}
	//Doing an enpassant capture
	if (piece==PIECE_WPAWN&&x1!=x2&&grid[x2][y2]==PIECE_EMPTY) {
		grid[x2][y2-1] = PIECE_EMPTY;
	} else if (piece==PIECE_BPAWN&&x1!=x2&&grid[x2][y2]==PIECE_EMPTY) {
		grid[x2][y2+1] = PIECE_EMPTY;
	}
	//Doing the actual move
	grid[x2][y2] = grid[x1][y1];
	grid[x1][y1] = PIECE_EMPTY;
	//Doing a castling move
	if (piece==PIECE_WKING||piece==PIECE_BKING) {
		if (x2==x1+2) {
			grid[5][y1] = grid[7][y1];
			grid[7][y1] = PIECE_EMPTY;
		} else if (x2==x1-2) {
			grid[3][y1] = grid[0][y1];
			grid[0][y1] = PIECE_EMPTY;
		}
	}
	//Doing a pawn promotion
	if (piece==PIECE_WPAWN&&y2==7) {
		grid[x2][y2] = promote;
	} else if (piece==PIECE_BPAWN&&y2==0) {
		grid[x2][y2] = promote;
	}

	turn++;
	return;
}

void Board::gen_trymoves(std::list<ppii> & moves) const {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (grid[x][y]==PIECE_EMPTY||side(grid[x][y])!=(turn%2)) continue;
			ChessPiece piece = grid[x][y];
			pii cur = std::make_pair(x,y);
			if (piece==PIECE_WPAWN||piece==PIECE_BPAWN) {
				int sign = (piece==PIECE_WPAWN)?1:-1;
				if (y+sign<0||y+sign>=8) continue;
				if (grid[x][y+sign]==PIECE_EMPTY) {
					//Normal pawn move
					moves.push_back(std::make_pair(cur,std::make_pair(x,y+sign)));
					if ((y==1&&sign==1)||(y==6&&sign==-1)) {
						if (grid[x][y+2*sign]==PIECE_EMPTY) {
							//Double pawn move
							moves.push_back(std::make_pair(cur,std::make_pair(x,y+2*sign)));
						}
					}
				}
				//Pawn captures
				if (x+1<8&&side(grid[x+1][y+sign])!=side(piece)) {
					if (side(grid[x+1][y+sign])==1-side(piece)) {
						moves.push_back(std::make_pair(cur,std::make_pair(x+1,y+sign)));
					} else if (enpassant==x+1) { //Enpassant
						if (y==4&&sign==1) {
							moves.push_back(std::make_pair(cur,std::make_pair(x+1,y+1)));
						} else if (y==3&&sign==-1) {
							moves.push_back(std::make_pair(cur,std::make_pair(x+1,y-1)));
						}
					}
				}
				if (x-1>=0&&side(grid[x-1][y+sign])!=side(piece)) {
					if (side(grid[x-1][y+sign])==1-side(piece)) {
						moves.push_back(std::make_pair(cur,std::make_pair(x-1,y+sign)));
					} else if (enpassant==x-1) { //Enpassant
						if (y==4&&sign==1) {
							moves.push_back(std::make_pair(cur,std::make_pair(x-1,y+1)));
						} else if (y==3&&sign==-1) {
							moves.push_back(std::make_pair(cur,std::make_pair(x-1,y-1)));
						}
					}
				}
			} else if (piece==PIECE_WKNIGHT||piece==PIECE_BKNIGHT) {
			//Knight moves
				int delta[8][2] = {{1,2},{2,1},{-1,2},{2,-1},{1,-2},{-2,1},{-1,-2},{-2,-1}};
				for (int i = 0; i < 8; i++) {
					int nx = x+delta[i][0],ny = y+delta[i][1];
					if (nx<0||nx>=8||ny<0||ny>=8) continue;
					if (side(grid[nx][ny])==side(piece)) continue;
					moves.push_back(std::make_pair(cur,std::make_pair(nx,ny)));
				}
			} else if (piece==PIECE_WKING||piece==PIECE_BKING) {
			//King moves
				int delta[8][2] = {{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};
				for (int i = 0; i < 8; i++) {
					int nx = x+delta[i][0],ny = y+delta[i][1];
					if (nx<0||nx>=8||ny<0||ny>=8) continue;
					if (side(grid[nx][ny])==side(piece)) continue;
					moves.push_back(std::make_pair(cur,std::make_pair(nx,ny)));
				}
				//castling
				if (castling[side(piece)][0]) {
					//0-0
					if (grid[x+2][y]==PIECE_EMPTY &&
							grid[x+1][y]==PIECE_EMPTY &&
							castling[side(piece)][1]
					) {
						moves.push_back(std::make_pair(cur,std::make_pair(x+2,y)));
					}
					//0-0-0
					if (grid[x-2][y]==PIECE_EMPTY &&
							grid[x-1][y]==PIECE_EMPTY &&
							grid[x-3][y]==PIECE_EMPTY &&
							castling[side(piece)][2]
					) {
						moves.push_back(std::make_pair(cur,std::make_pair(x-2,y)));
					}
				}
			}
			//TODO: Hell lot of copypaste code. Probably should fix it later
			if (piece==PIECE_WROOK||piece==PIECE_BROOK||piece==PIECE_WQUEEN||piece==PIECE_BQUEEN) {
			//Horizontal moves
				int delta[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
				for (int i = 0; i < 4; i++) {
					bool capture = false;
					for (int j = 1;;j++) {
						int nx = x+j*delta[i][0],ny = y+j*delta[i][1];
						if (nx<0||nx>=8||ny<0||ny>=8) break;
						if (side(grid[nx][ny])==side(piece)) break;
						if (side(grid[nx][ny])==1-side(piece)) capture = true;
						moves.push_back(std::make_pair(cur,std::make_pair(nx,ny)));
						if (capture) break;
					}
				}
			}
			if (piece==PIECE_WBISHOP||piece==PIECE_BBISHOP||piece==PIECE_WQUEEN||piece==PIECE_BQUEEN) {
			//Vertical moves
				int delta[4][2] = {{-1,-1},{1,-1},{1,1},{-1,1}};
				for (int i = 0; i < 4; i++) {
					bool capture = false;
					for (int j = 1;;j++) {
						int nx = x+j*delta[i][0],ny = y+j*delta[i][1];
						if (nx<0||nx>=8||ny<0||ny>=8) break;
						if (side(grid[nx][ny])==side(piece)) break;
						if (side(grid[nx][ny])==1-side(piece)) capture = true;
						moves.push_back(std::make_pair(cur,std::make_pair(nx,ny)));
						if (capture) break;
					}
				}
			}
		}
	}
	return;
}
//TODO: Cull illegal moves for castling as well
void Board::cull_illegal_moves(std::list<ppii> & moves) const {
	bool erased = false;
	for (std::list<ppii>::iterator it = moves.begin(); it != moves.end();) {
		Board nb;
		erased = false;
		memcpy(&nb, this, sizeof(nb));
		//Special case for castling
		ChessPiece p = grid[it->first.first][it->first.second];
		if (p==PIECE_WKING||p==PIECE_BKING) {
			int x1,y1,x2,y2;
			x1 = it->first.first;
			y1 = it->first.second;
			x2 = it->second.first;
			y2 = it->second.second;
			if (x2==x1+2||x2==x1-2) {
				nb.turn++;
				if (x2==x1+2) {
					if (nb.is_attacked(std::make_pair(x1,y1)) ||
							nb.is_attacked(std::make_pair(x1+1,y1)) ||
							nb.is_attacked(std::make_pair(x1+2,y1))) {
						it = moves.erase(it);
						continue;
					}
				} else if (x2==x1-2) {
					if (nb.is_attacked(std::make_pair(x1,y1)) ||
							nb.is_attacked(std::make_pair(x1-1,y1)) ||
							nb.is_attacked(std::make_pair(x1-2,y1))) {
						it = moves.erase(it);
						continue;
					}
				}
			}
		}

		nb.shift_piece(it->first,it->second);
		int kx=-1,ky=-1;
		for (int i = 0; (i < 8)&&(kx==-1); i++) {
			for (int j = 0; j < 8; j++) {
				if ((turn%2==0&&nb.grid[i][j]==PIECE_WKING)||(turn%2==1&&nb.grid[i][j]==PIECE_BKING)) {
					kx = i;
					ky = j;
					break;
				}
			}
		}
		if (kx==-1) return;
		pii kp = std::make_pair(kx,ky);
		if (nb.is_attacked(kp)) {
			//printf("Culled move %d,%d -> %d,%d\n", it->first.first, it->first.second, it->second.first, it->second.second);
			it = moves.erase(it);
			erased = true;
		}
		if (!erased) it++;
	}
	return;
}

bool Board::move(pii starting_square,pii ending_square, ChessPiece promotion) {
	std::list<ppii> moves;
	gen_moves(moves);
	ppii p = std::make_pair(starting_square,ending_square);
	for (std::list<ppii>::iterator it = moves.begin(); it != moves.end(); it++) {
		if (*it==p) {
			shift_piece(p.first,p.second,promotion);
			return true;
		}
	}
	return false;
}
bool Chess::move(pii starting_square, pii ending_square, ChessPiece promotion) {
	return board.move(starting_square, ending_square, promotion);
}

//TODO: Write this
bool Chess::move(std::string move_string) {
	return false;
}
