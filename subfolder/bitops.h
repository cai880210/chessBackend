# ifndef BITOPS_H
# define BITOPS_H
# include "defs.h"


// Finds the lsb index and turns the "1" into a "0"
inline int poplsb(bitboard &num) {
	unsigned long ret;
	_BitScanForward64(&ret, num);
	num &= num - 1; // num - 1 makes it so that the lsb is for sure 0 now
	return ret;
}

inline int bitscanforward(const bitboard num) {
	if(num == ZERO) return -1;
	unsigned long ret;
	_BitScanForward64(&ret, num);
	return ret;
}

inline int bitscanbackward(const bitboard num) {
	if(num == ZERO) return -1;
	unsigned long ret;
	_BitScanReverse64(&ret, num);
	return 63 - ret;
}


// // This function will take in a number (in our case a chess piece) and will print out the binary representation of it
// void print_bits(bitboard num, const char c = '1') {
// 	// For each position, we will make a bitboard with all zeros and a single one, and then we will bitwise-and the two bitboards.
// 	// If the bitwise-and is 0, then the position where the 1 was must have been 0. 
// 	for(int row = 7; row >= 0; row--) {
// 		for(int col = 0; col <= 7; col++) {
// 			bitboard cur_bitboard = (1ULL << ((row * 8) + col)); // We have to convert to an ULL since "1" by default is an intger with 32 bits
// 			std::cout << ((num & cur_bitboard) ? c : '_') << " ";
// 		}
// 	std::cout << "\n";
// 	}
// 	std::cout << "\n";
// }

// void print_the_board(bitboard bitboards[3][6]) {
// 	for(int row = 7; row >= 0; row--) {
// 		for(int col = 0; col <= 7; col++) {
// 			bitboard cur_bitboard = (1ULL << ((row * 8) + col));
// 			bool empty = true;
// 			for(int side = 0; side <= 1; ++side) {
// 				for(int type = 0; type <= 5; ++type) {
// 					if(bitboards[side][type] & cur_bitboard) {
// 						if(side == 0) {
// 							if(type == 0) std::cout << "P" << " ";
// 							else if(type == 1) std::cout << "C" << " ";
// 							else if(type == 2) std::cout << "H" << " ";
// 							else if(type == 3) std::cout << "B" << " ";
// 							else if(type == 4) std::cout << "Q" << " ";
// 							else if(type == 5) std::cout << "K" << " ";
// 						} else {
// 							if(type == 0) std::cout << "O" << " ";
// 							else if(type == 1) std::cout << "R" << " ";
// 							else if(type == 2) std::cout << "K" << " ";
// 							else if(type == 3) std::cout << "D" << " ";
// 							else if(type == 4) std::cout << "V" << " ";
// 							else if(type == 5) std::cout << "D" << " ";
// 						}
// 						empty = false;
// 						break;
// 					}
// 				}
// 			}
// 			if(empty) std::cout << "_ ";
// 		}
// 		std::cout << std::endl;
// 	}
// 	std::cout << std::endl;
// }

// void print_move(Move move) {
// 	std::vector<std::string> types = {"Pawn", "Castle", "Knight", "Bishop", "Queen", "King", "None"};
// 	int from    = ((move & 0b000000000000111111)); // 63 == 11111 to get first 5 bits
// 	int to      = ((move & 0b000000111111000000) >> 6);
// 	int type    = ((move & 0b000111000000000000) >> 12);
// 	std::cout << "From: " << from % 8 << " " << from / 8 << std::endl;
// 	std::cout << "  To: " << to % 8 << " " << to / 8  << std::endl;
// 	std::cout << "Type: " << types[type] << std::endl;
// 	std::cout << std::endl;
// }


# endif
