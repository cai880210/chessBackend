# include "main.h"
# include "magicmoves.h"
# include <time.h>
# include <utility>
# include <string>




using namespace std;

Board::Board(): bitboards{}, SideToPlay{White} {
	initmagicmoves();
	bitboards[White][King] = 0b0000000000000000000000000000000000000000000000000000000000010000;
	bitboards[White][Queen] = 0b0000000000000000000000000000000000000000000000000000000000001000;
	bitboards[White][Castle] = 0b0000000000000000000000000000000000000000000000000000000010000001;
	bitboards[White][Knight] = 0b0000000000000000000000000000000000000000000000000000000001000010;
	bitboards[White][Bishop] = 0b0000000000000000000000000000000000000000000000000000000000100100;
	bitboards[White][Pawn] = 0b0000000000000000000000000000000000000000000000001111111100000000;
	bitboards[Black][King] = 0b0001000000000000000000000000000000000000000000000000000000000000;
	bitboards[Black][Queen] = 0b0000100000000000000000000000000000000000000000000000000000000000;
	bitboards[Black][Castle] = 0b1000000100000000000000000000000000000000000000000000000000000000;
	bitboards[Black][Knight] = 0b0100001000000000000000000000000000000000000000000000000000000000;
	bitboards[Black][Bishop] = 0b0010010000000000000000000000000000000000000000000000000000000000;
	bitboards[Black][Pawn] = 0b000000011111111000000000000000000000000000000000000000000000000;
	
	bitboards[All][White] = bitboards[White][King] | bitboards[White][Queen] | bitboards[White][Castle] | bitboards[White][Knight] | bitboards[White][Bishop] | bitboards[White][Pawn];
	bitboards[All][Black] = bitboards[Black][King] | bitboards[Black][Queen] | bitboards[Black][Castle] | bitboards[Black][Knight] | bitboards[Black][Bishop] | bitboards[Black][Pawn];
	bitboards[All][All] = bitboards[All][White] | bitboards[All][Black];
}

Board::Board(std::string boardRepresentation): bitboards{}, SideToPlay{Black} {
	initmagicmoves();
	bitboards[White][King] = ZERO;
	bitboards[White][Queen] = ZERO;
	bitboards[White][Castle] = ZERO;
	bitboards[White][Knight] = ZERO;
	bitboards[White][Bishop] = ZERO;
	bitboards[White][Pawn] = ZERO;
	bitboards[Black][King] = ZERO;
	bitboards[Black][Queen] = ZERO;
	bitboards[Black][Castle] = ZERO;
	bitboards[Black][Knight] = ZERO;
	bitboards[Black][Bishop] = ZERO;
	bitboards[Black][Pawn] = ZERO;

	for(int i = 0; i < 64; ++i) {
		// White Pawns
		if(boardRepresentation[i] == 'P') bitboards[White][Pawn] |= (1ULL << i);
		// White Knight
		if(boardRepresentation[i] == 'H') bitboards[White][Knight] |= (1ULL << i);
		// White Bishop
		if(boardRepresentation[i] == 'B') bitboards[White][Bishop] |= (1ULL << i);
		// White Rook
		if(boardRepresentation[i] == 'R') bitboards[White][Castle] |= (1ULL << i);
		// White Queen
		if(boardRepresentation[i] == 'Q') bitboards[White][Queen] |= (1ULL << i);
		// White King
		if(boardRepresentation[i] == 'K') bitboards[White][King] |= (1ULL << i);
		// Black Pawns
		if(boardRepresentation[i] == 'O') bitboards[Black][Pawn] |= (1ULL << i);
		// Black Knight
		if(boardRepresentation[i] == 'L') bitboards[Black][Knight] |= (1ULL << i);
		// Black Bishop
		if(boardRepresentation[i] == 'D') bitboards[Black][Bishop] |= (1ULL << i);
		// Black Rook
		if(boardRepresentation[i] == 'C') bitboards[Black][Castle] |= (1ULL << i);
		// Black Queen
		if(boardRepresentation[i] == 'W') bitboards[Black][Queen] |= (1ULL << i);
		// Black King
		if(boardRepresentation[i] == 'I') bitboards[Black][King] |= (1ULL << i);
	}

	bitboards[All][White] = bitboards[White][King] | bitboards[White][Queen] | bitboards[White][Castle] | bitboards[White][Knight] | bitboards[White][Bishop] | bitboards[White][Pawn];
	bitboards[All][Black] = bitboards[Black][King] | bitboards[Black][Queen] | bitboards[Black][Castle] | bitboards[Black][Knight] | bitboards[Black][Bishop] | bitboards[Black][Pawn];
	bitboards[All][All] = bitboards[All][White] | bitboards[All][Black];
}

PieceType Board::get_intersection_type(int from, bool c) {
	bitboard mask = (1ULL << from);
	for(int i = 0; i < 6; ++i) {
		if(bitboards[c][i] & mask) return static_cast<PieceType>(i);
	}
	return None; // If none match, potentially this is reached if the move was not a capture
}

void Board::KnightMoves(vector<Move>& moves) {
	bitboard horse_board = bitboards[SideToPlay][Knight];
	while(horse_board) {
		int from = poplsb(horse_board);
		bitboard pos_moves = KNIGHT_LT[from];
		attackMap[SideToPlay] |= pos_moves;
		while(pos_moves) {
			int to = poplsb(pos_moves);
			if(!(bitboards[All][SideToPlay] & (1Ull << to))) {
				if(bitboards[All][!SideToPlay] & (1Ull << to)) { // then this is a capture
					moves.push_back(from | to << 6 | Knight << 12 | get_intersection_type(to, !SideToPlay) << 15);
				} else {
					moves.push_back(from | to << 6 | Knight << 12 | None << 15);
				}
			}
		}
	}
}

void Board::PawnMoves(vector<Move>& moves) {
	bitboard pawn_board = bitboards[SideToPlay][Pawn];
	while(pawn_board) {
		int from = poplsb(pawn_board);
		bitboard pos_pushes = ZERO;
		bitboard pos_double_pushes = ZERO;
		bitboard pos_captures = ZERO;
		if(SideToPlay == White) {
			if(from < 56) {
				pos_pushes ^= (1ULL << (8 + from));
				if(from % 8 != 0) pos_captures |= (1ULL << (7 + from));
				if(from % 8 != 7) pos_captures |= (1ULL << (9 + from));
			}
			if(from < 16 && !(bitboards[All][All] & ((1Ull << (from + 8)) | (1Ull << (from + 16))))) pos_double_pushes ^= (1ULL << (16 + from)); // en passant
		} else {
			if(from > 7) {
				pos_pushes ^= (1ULL << (from - 8));
				if(from % 8 != 0) pos_captures |= (1ULL << (from - 9));
				if(from % 8 != 7) pos_captures |= (1ULL << (from - 7));
			}
			if(from > 47 && !(bitboards[All][All] & ((1Ull << (from - 8)) | (1Ull << (from - 16))))) pos_double_pushes ^= (1ULL << (from - 16)); // en passant
		}
		while(pos_pushes) {
			int to = poplsb(pos_pushes);
			if(!(bitboards[All][All] & (1Ull << to))) {
				moves.push_back(from | to << 6 | Pawn << 12 | None << 15);
			}
		}
		attackMap[SideToPlay] |= pos_captures;
		while(pos_captures) {
			int to = poplsb(pos_captures);
			if(bitboards[All][!SideToPlay] & (1Ull << to)) {
				moves.push_back(from | to << 6 | Pawn << 12 | get_intersection_type(to, !SideToPlay) << 15);
			} else if ((lastEnPassant != 0) && ((SideToPlay == White && (to % 8) == lastEnPassant - 1 && (to / 8) == 5) ||
			 (SideToPlay == Black && (to % 8) == lastEnPassant - 1 && (to / 8) == 2))) {
				moves.push_back(from | to << 6 | Pawn << 12 | get_intersection_type(to, !SideToPlay) << 15 | 15 << 18);
			 }
		}
		while(pos_double_pushes) {
			int to = poplsb(pos_double_pushes);
			moves.push_back(from | to << 6 | Pawn << 12 | get_intersection_type(to, !SideToPlay) << 15 | ((to % 8) + 1) << 18);
		}
	}
}

void Board::BishopMoves(vector<Move>& moves) {
	bitboard bishop_board = bitboards[SideToPlay][Bishop];
	while(bishop_board) {
		int from = poplsb(bishop_board);
		bitboard pos_moves = Bmagic(from, bitboards[All][All]);
		attackMap[SideToPlay] |= pos_moves;
		while(pos_moves) {
			int to = poplsb(pos_moves);
			if(!(bitboards[All][SideToPlay] & (1Ull << to))) {
				if(bitboards[All][!SideToPlay] & (1Ull << to)) { // then this is a capture
					moves.push_back(from | to << 6 | Bishop << 12 | get_intersection_type(to, !SideToPlay) << 15);
				} else {
					moves.push_back(from | to << 6 | Bishop << 12 | None << 15);
				}
			}
		}
	}
}

void Board::QueenMoves(vector<Move>& moves) {
	bitboard queen_board = bitboards[SideToPlay][Queen];
	while(queen_board) {
		int from = poplsb(queen_board);
		bitboard pos_moves = Bmagic(from, bitboards[All][All]);
		pos_moves |= Rmagic(from, bitboards[All][All]);
		attackMap[SideToPlay] |= pos_moves;
		while(pos_moves) {
			int to = poplsb(pos_moves);
			if(!(bitboards[All][SideToPlay] & (1Ull << to))) {
				if(bitboards[All][!SideToPlay] & (1Ull << to)) { // then this is a capture
					moves.push_back(from | to << 6 | Queen << 12 | get_intersection_type(to, !SideToPlay) << 15);
				} else {
					moves.push_back(from | to << 6 | Queen << 12 | None << 15);
				}
			}
		}
	}
}

void Board::CastleMoves(vector<Move>& moves) {
	bitboard castle_board = bitboards[SideToPlay][Castle];
	while(castle_board) {
		int from = poplsb(castle_board);
		bitboard pos_moves = Rmagic(from, bitboards[All][All]);
		attackMap[SideToPlay] |= pos_moves;
		while(pos_moves) {
			int to = poplsb(pos_moves);
			if(!(bitboards[All][SideToPlay] & (1Ull << to))) {
				if(bitboards[All][!SideToPlay] & (1Ull << to)) { // then this is a capture
					moves.push_back(from | to << 6 | Castle << 12 | get_intersection_type(to, !SideToPlay) << 15);
				} else {
					moves.push_back(from | to << 6 | Castle << 12 | None << 15);
				}
			}
		}
	}
}

void Board::KingMoves(vector<Move>& moves) {
	bitboard king_board = bitboards[SideToPlay][King];
	while(king_board) {
		int from = poplsb(king_board);
		bitboard pos_moves = KING_LT[from];
		attackMap[SideToPlay] |= pos_moves;
		while(pos_moves) {
			int to = poplsb(pos_moves);
			if(!(bitboards[All][SideToPlay] & (1Ull << to))) {
				if(bitboards[All][!SideToPlay] & (1Ull << to)) { // then this is a capture
					moves.push_back(from | to << 6 | King << 12 | get_intersection_type(to, !SideToPlay) << 15);
				} else {
					moves.push_back(from | to << 6 | King << 12 | None << 15);
				}
			}
		}
	}
}

void Board::remove(int sq, PieceType pt, bool c) {
	if(pt == None) return;
	bitboard mask = 1ULL << sq;
	bitboards[c][pt] ^= mask;
	bitboards[All][c] ^= mask;
	bitboards[All][All] ^= mask;	
}



void Board::add(int sq, PieceType pt, bool c) {
	if(pt == None) return;
	bitboard mask = 1ULL << sq;
	bitboards[c][pt] ^= mask;
	bitboards[All][c] ^= mask;
	bitboards[All][All] ^= mask;	
}



Move Board::MakeMove(Move move) {
	int from = get_from(move);
	int to = get_to(move);
	PieceType pt_from = get_from_piece_type(move);
	PieceType pt_to = get_to_piece_type(move);
	lastEnPassant = get_en_passant_file(move);
	if(get_en_passant_capture(move)) { // we captured a piece that en passanted
		if(SideToPlay == White) remove((to - 8), Pawn, Black);
		if(SideToPlay == Black) remove((to + 8), Pawn, White);
	}
	remove(from, pt_from, SideToPlay);
	remove(to, pt_to, !SideToPlay);
	add(to, pt_from, SideToPlay);
	SideToPlay = static_cast<Colour>(!SideToPlay);
	return move;
}

void Board::UnMakeMove(Move move) {
	int from = get_from(move);
	int to = get_to(move);
	PieceType pt_from = get_from_piece_type(move);
	PieceType pt_to = get_to_piece_type(move);
	lastEnPassant = get_en_passant_file(move);
	SideToPlay = static_cast<Colour>(!SideToPlay);
	remove(to, pt_from, SideToPlay);
	add(to, pt_to, !SideToPlay);
	add(from, pt_from, SideToPlay);
	if(get_en_passant_capture(move)) {
		if(SideToPlay == White) add((to - 8), Pawn, Black);
		if(SideToPlay == Black) add((to + 8), Pawn, White);
	}
}

vector<Move> Board::genMoves() {
	// attackMap[SideToPlay] = ZERO;
	vector<Move> newMoves;
	PawnMoves(newMoves);
	KnightMoves(newMoves);
	KingMoves(newMoves);
	QueenMoves(newMoves);
	CastleMoves(newMoves);
	BishopMoves(newMoves);
	// attackMap[SideToPlay] ^= (bitboards[All][SideToPlay] & attackMap[SideToPlay]);
	return newMoves;
}

int Board::evaluate() {
	int score = 0;
	
	// Calculate White score"
	bitboard king_board = bitboards[White][King];
	while(king_board) {
		score += 20000;
		poplsb(king_board);
	}
	bitboard bishop_board = bitboards[White][Bishop];
	while(bishop_board) {
		score += 300 + bishopLookup[poplsb(bishop_board)];
	}
	bitboard knight_board = bitboards[White][Knight];
	while(knight_board) {
		score += 300 + knightLookup[poplsb(knight_board)];
	}
	bitboard pawn_board = bitboards[White][Pawn];
	while(pawn_board) {
		score += 100 + pawnLookup[poplsb(pawn_board)];
	}
	bitboard queen_board = bitboards[White][Queen];
	while(queen_board) {
		score += 900;
		poplsb(queen_board);
	}
	bitboard rook_board = bitboards[White][Castle];
	while(rook_board) {
		score += 500;
		poplsb(rook_board);
	}

	// Calculate Black score
	bitboard king_board2 = bitboards[Black][King];
	while(king_board2) {
		score -= 20000;
		poplsb(king_board2);
	}
	bitboard bishop_board2 = bitboards[Black][Bishop];
	while(bishop_board2) {
		score -= 300 + bishopLookup[(63 - poplsb(bishop_board2)) % 64];
	}
	bitboard knight_board2 = bitboards[Black][Knight];
	while(knight_board2) {
		score -= 300 + knightLookup[(63 - poplsb(knight_board2)) % 64];
	}
	bitboard pawn_board2 = bitboards[Black][Pawn];
	while(pawn_board2) {
		score -= 100 + pawnLookup[(63 - poplsb(pawn_board2)) % 64];
	}
	bitboard queen_board2 = bitboards[Black][Queen];
	while(queen_board2) {
		score -= 900;
		poplsb(queen_board2);
	}
	bitboard rook_board2 = bitboards[Black][Castle];
	while(rook_board2) {
		score -= 500;
		poplsb(rook_board2);
	}

	return score;
}


std::pair<int, Move> Board::miniMax(int depth, bool isMaximizing, int alpha, int beta) {
	if(depth == 0) return std::make_pair(evaluate(), 0);
	else if(isMaximizing) {
		int bestScore = INT_MIN;
		Move bestMove = 0;
		for(Move move : genMoves()) {
			MakeMove(move);
			int val = miniMax(depth-1, !isMaximizing, alpha, beta).first;
			if(val > bestScore) {
				bestScore = val;
				bestMove = move;
			}
			UnMakeMove(move);
			alpha = std::max(alpha, val);
			if(alpha >= beta) break;
		}
		return make_pair(bestScore, bestMove);
	} else {
		int bestScore = INT_MAX;
		Move bestMove = 0;
		for(Move move : genMoves()) {
			MakeMove(move);
			std::pair<int, Move> pair = miniMax(depth-1, !isMaximizing, alpha, beta);
			int val = pair.first;
			int response = pair.second;
			if(val < bestScore) {
				if(depth == 4) {
					std::cout << "best move before had score: " << bestScore << "now have : " << val << std::endl;
					std::cout << "new move" << std::endl;
					std::vector<std::string> types = {"Pawn", "Castle", "Knight", "Bishop", "Queen", "King", "None"};
					int from    = ((move & 0b000000000000111111)); // 63 == 11111 to get first 5 bits
					int to      = ((move & 0b000000111111000000) >> 6);
					std::cout << "From: " << from % 8 << " " << from / 8 << std::endl;
					std::cout << "  To: " << to % 8 << " " << to / 8  << std::endl;
					std::cout << std::endl;
					std::cout << "best move in response:" << std::endl;
					from    = ((response & 0b000000000000111111)); // 63 == 11111 to get first 5 bits
					to      = ((response & 0b000000111111000000) >> 6);
					std::cout << "From: " << from % 8 << " " << from / 8 << std::endl;
					std::cout << "  To: " << to % 8 << " " << to / 8  << std::endl;
				}
				bestScore = val;
				bestMove = move;
			}
			UnMakeMove(move);
			beta = std::min(beta, val);
			if(beta <= alpha) break;
	}
	return make_pair(bestScore, bestMove);
	}
}

// int Board::perft(int depth, unsigned long long move_depth_index) {
// 	//std::vector<Move> temp = moves;
// 	if(depth == 0) {
// 		// genMoves();
// 		// moves = temp;
// 		// if(bitboards[!SideToPlay][King] & attackMap[SideToPlay]) {
// 		// 	//print_board(bitboards);
// 		// 	return 0;
// 		// }
// 		return 1;
// 	} else {
// 		long long int count = 0;
// 		genMoves();
// 		// if(bitboards[!SideToPlay][King] & attackMap[SideToPlay]) {
// 		// 	moves = temp;
// 		// 	//print_board(bitboards);
// 		// 	return 0;
// 		// }
// 		unsigned long long int total_size = moves.size();
// 		for(unsigned long long i = move_depth_index; i < total_size; ++i) {
// 			Move temp_move = moves.back();
// 			bitboard temp_attack_map = attackMap[SideToPlay];
// 			// if(get_from_piece_type(temp_move) == Queen) {
// 			// 	print_board(bitboards);
// 			// 	print_bits(attackMap[SideToPlay]);
// 			// 	std::cout << "\n\n\n\n\n\n\n\n\n\n" << std::endl;
// 			// }
// 			MakeMove();
// 			count += perft(depth-1, moves.size());
// 			UnMakeMove(temp_move);
// 			attackMap[SideToPlay] = temp_attack_map;
// 		}
// 		return count;
// 	}
// }

// void Board::time(int depth = 6) {
// 	clock_t start, end;
// 	for(int i = 1; i <= depth; ++i) {
// 		start = clock();
// 		long long num = perft(i, 0);
// 		end = clock();
// 		std::cout << num << " " << i << " ";
// 		double total_time = ((double) (end - start) / CLOCKS_PER_SEC);
// 		std::cout << "total time: " << total_time;
// 		std::cout << " | time per move: " << ((double) total_time / num) << std::endl;
// 	}
// }

std::string getNextMove(std::string boardRepresentation) {
	Board b(boardRepresentation);
	std::cout << "Current board score: " << std::endl;
	std::cout << b.evaluate() << std::endl;


	std::cout << "Current Board before making new move:"  << std::endl;
	for(int row = 7; row >= 0; row--) {
		for(int col = 0; col <= 7; col++) {
			bitboard cur_bitboard = (1ULL << ((row * 8) + col));
			bool empty = true;
			for(int side = 0; side <= 1; ++side) {
				for(int type = 0; type <= 5; ++type) {
					if(b.bitboards[side][type] & cur_bitboard) {
						if(side == 0) {
							if(type == 0) std::cout << "P" << " ";
							else if(type == 1) std::cout << "C" << " ";
							else if(type == 2) std::cout << "H" << " ";
							else if(type == 3) std::cout << "B" << " ";
							else if(type == 4) std::cout << "Q" << " ";
							else if(type == 5) std::cout << "K" << " ";
						} else {
							if(type == 0) std::cout << "O" << " ";
							else if(type == 1) std::cout << "R" << " ";
							else if(type == 2) std::cout << "L" << " ";
							else if(type == 3) std::cout << "D" << " ";
							else if(type == 4) std::cout << "W" << " ";
							else if(type == 5) std::cout << "I" << " ";
						}
						empty = false;
						break;
					}
				}
			}
			if(empty) std::cout << "_ ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::pair<int, Move> pair = b.miniMax(5, b.SideToPlay == White, INT_MIN, INT_MAX);
	std::cout << "score of best move " << pair.first << std::endl;
	Move move = pair.second;

	std::vector<std::string> types = {"Pawn", "Castle", "Knight", "Bishop", "Queen", "King", "None"};
	int from    = ((move & 0b000000000000111111));
	int to      = ((move & 0b000000111111000000) >> 6);
	int type    = ((move & 0b000111000000000000) >> 12);
	std::cout << from << " " << to << " " << types[type] << std::endl;

	b.MakeMove(move);

	std::cout << "board after making move" << std::endl;

	for(int row = 7; row >= 0; row--) {
		for(int col = 0; col <= 7; col++) {
			bitboard cur_bitboard = (1ULL << ((row * 8) + col));
			bool empty = true;
			for(int side = 0; side <= 1; ++side) {
				for(int type = 0; type <= 5; ++type) {
					if(b.bitboards[side][type] & cur_bitboard) {
						if(side == 0) {
							if(type == 0) std::cout << "P" << " ";
							else if(type == 1) std::cout << "C" << " ";
							else if(type == 2) std::cout << "H" << " ";
							else if(type == 3) std::cout << "B" << " ";
							else if(type == 4) std::cout << "Q" << " ";
							else if(type == 5) std::cout << "K" << " ";
						} else {
							if(type == 0) std::cout << "O" << " ";
							else if(type == 1) std::cout << "R" << " ";
							else if(type == 2) std::cout << "L" << " ";
							else if(type == 3) std::cout << "D" << " ";
							else if(type == 4) std::cout << "W" << " ";
							else if(type == 5) std::cout << "I" << " ";
						}
						empty = false;
						break;
					}
				}
			}
			if(empty) std::cout << "_ ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	return to_string(from / 8) + " " + to_string(from % 8) + " " + to_string(to / 8) + " " + to_string(to % 8) + " " + types[type];
}

int main () {
	Board b;
	for(int i = 0; i < 20; ++i) {
		// print_board(b.bitboards);
		std::pair<int, Move> pair = b.miniMax(4, b.SideToPlay == White, INT_MIN, INT_MAX);
		std::cout << "Score: " << pair.first << std::endl;
		// print_move(pair.second);
		b.MakeMove(pair.second);
	}
	return 0;
}



