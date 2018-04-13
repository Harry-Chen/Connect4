#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <bitset>

#include "Point.h"
#include "Judge.h"
#include "Strategy.h"
#include "BitSet.h"

class GameBoard
{
	static int N, M;
	BitSet board{ 0, 0, 0 };
	friend class UCT;

public:

	void place(int x, int y)
	{
		board.set(x + 16 * y);
	}
	GameBoard() {}
	GameBoard(int* const* source)
	{
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
			{
				if (source[i][j] != 0) {
					board.set(i + 16 * j);
				}
			}
	}
	void output()
	{
		printf("=========hehe=========\n");
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++)
				printf("%d ", get(i, j));
			printf("\n");
		}
		printf("=========heheEnd======\n");
	}


	inline bool get(int x, int y)
	{
		return board[x + 16 * y];
	}

	bool win() const
	{
		auto vert = board & board >> 1 & board >> 2 & board >> 3;
		auto hori = board & board >> 16 & board >> 32 & board >> 48;
		auto bslash = board & board >> 15 & board >> 30 & board >> 45;
		auto slash = board & board >> 17 & board >> 34 & board >> 51;
		return hori | vert | bslash | slash;
	}

};


#endif /* CHESSBOARD_H */
