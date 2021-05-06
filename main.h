#ifndef MAIN_H
#define MAIN_H

# include <vector>
# include <utility>
# include "defs.h"
# include "bitops.h"
# include <string>

enum PieceType {Pawn, Castle, Knight, Bishop, Queen, King, None};
enum Colour {White, Black};
# define All 2

class Board {
	public:

		Board(std::string boardRepresentation);
		
		bitboard bitboards[3][6];
		Colour SideToPlay;
		short lastEnPassant = 0;
		bitboard attackMap[2];
		//std::vector<std::pair<short,short>> pins;

		std::vector<Move> moves;

		std::vector<Move> genMoves();

		Move MakeMove(Move move);
		void UnMakeMove(Move move);

		//short Rpin(int &to, int &from, Colour SideToPlay)
		
		void PawnMoves(std::vector<Move>& moves);
		void BishopMoves(std::vector<Move>& moves);
		void CastleMoves(std::vector<Move>& moves);
		void QueenMoves(std::vector<Move>& moves);
		void KnightMoves(std::vector<Move>& moves);
		void KingMoves(std::vector<Move>& moves);
		Board();
		PieceType get_intersection_type(int from, bool c);
		void remove(int sq, PieceType pt, bool c);
		void add(int sq, PieceType pt, bool c);
		int perft(int depth, unsigned long long move_depth_index);
		void time(int depth);

		int evaluate();
		std::pair<int, Move> miniMax(int depth, bool isMaximizing, int alpha, int beta);

		
};


inline constexpr int get_from(Move m) {return m & 0b0000000000111111;}

inline constexpr int get_to(Move m) {return ((m & 0b0000111111000000) >> 6);}

inline constexpr PieceType get_from_piece_type(Move m) {return static_cast<PieceType>((m & 0b111000000000000) >> 12);}

inline constexpr PieceType get_to_piece_type(Move m) {return static_cast<PieceType>((m & 0b111000000000000000) >> 15);}

inline constexpr short get_en_passant_file(Move m) {return ((m & 0b1111000000000000000000) >> 18);}

inline constexpr bool get_en_passant_capture(Move m) {return (((m & 0b1111000000000000000000) >> 18) == 15);}

std::string getNextMove(std::string boardRepresentation);


#endif
