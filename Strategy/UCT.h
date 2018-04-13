#ifndef UCT_h
#define UCT_h

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "GameBoard.h"
#include "Timer.hpp"

#define PLAYER_ME 1
#define PLAYER_OPPONENT 2

#define PROFIT_I_WIN -1
#define PROFIT_OPPONENT_WIN 1
#define PROFIT_TIE 0
#define UNTERMINAL_STATE 2

#define COEFFICIENT 0.9

#define eps 1e-8
#define MAX_MEMORY_POOL_SIZE 20000000
#define TIME_LIMIT_MICROSECOND 2.5e5

class Node
{
	static int M, N;
	static int _noX, _noY;
	int _player;
	int _x, _y;
	int visNum;
	double profit;
	Node *father;
	Node **children;
	int expNum;
	int *expNode;
	int _isTerminal;
	friend class UCT;

public:
	Node(int x = -1, int y = -1, int player = PLAYER_OPPONENT, Node *_father = nullptr);
	~Node() {}
	int x() { return _x; }
	int y() { return _y; }
	bool isTerminal();

	void set(int x = -1, int y = -1, int player = PLAYER_OPPONENT, Node *_father = nullptr);

	Node* newNode(int x = -1, int y = -1, int player = PLAYER_OPPONENT, Node *_father = nullptr);

	Node *expand();

	Node *bestChild();

	void backPropagation(double delta);
};


class UCT
{
	Node *root;
	int M, N;
	int _noX, _noY;

	Timer *timer;

	double defaultPolicy(Node *nowNode);
	Node *treePolicy(Node *nowNode);

	static int calculateProfit(const GameBoard &opponentBoard, const GameBoard &myBoard, const int rest, const int player);
	void backPropagation(Node *nowNode, double delta) {
		nowNode->backPropagation(delta);
	}

public:
	UCT(int _M, int _N, int noX, int noY) :M(_M), N(_N), _noX(noX), _noY(noY) {
		timer = new Timer();
	}

	std::pair<int, int> UCTSearch(int const* const* boardStart, const int *topStart);

	~UCT() { delete root; }
};
#endif /* UCT_h */ 
