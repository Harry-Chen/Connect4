#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <cstdio>

#include "BitSet.hpp"


class GameBoard
{
	BitSet::bits board = { 0, 0, 0 };
	int M, N;

public:

	GameBoard(int _M = 0, int _N = 0) : M(_M), N(_N) {}

	GameBoard(int M, int N, int * const * source) :GameBoard(M, N) {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				if (source[i][j] != 0) {
					BitSet::set(board, i + 16 * j);
				}
			}
		}
	}

	void output() {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				printf("%d ", get(i, j));
			}
			putc('\n', stdout);
		}
	}

	inline void place(int x, int y) {
		BitSet::set(board, x + 16 * y);
	}

	inline bool get(int x, int y) {
		return BitSet::test(board, x + 16 * y);
	}

	inline bool win() const {
		//auto vert = board & board >> 1 & board >> 2 & board >> 3;
		//auto hori = board & board >> 16 & board >> 32 & board >> 48;
		//auto bslash = board & board >> 15 & board >> 30 & board >> 45;
		//auto slash = board & board >> 17 & board >> 34 & board >> 51;
		//return hori | vert | bslash | slash;
		auto vert = BitSet:: and (board, BitSet:: and (BitSet::rightShift(board, 1), BitSet:: and (BitSet::rightShift(board, 2), BitSet::rightShift(board, 3))));
		auto hori = BitSet:: and (board, BitSet:: and (BitSet::rightShift(board, 16), BitSet:: and (BitSet::rightShift(board, 32), BitSet::rightShift(board, 48))));
		auto bslash = BitSet:: and (board, BitSet:: and (BitSet::rightShift(board, 15), BitSet:: and (BitSet::rightShift(board, 30), BitSet::rightShift(board, 45))));
		auto slash = BitSet:: and (board, BitSet:: and (BitSet::rightShift(board, 17), BitSet:: and (BitSet::rightShift(board, 34), BitSet::rightShift(board, 51))));
		auto final = BitSet:: or (hori, BitSet:: or (vert, BitSet:: or (bslash, slash)));
		return BitSet::notZero(final);
	}

};


#endif /* GAMEBOARD_HPP */
