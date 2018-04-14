#include <ctime>

#include "UCT.h"
#include "TreeNode.h"
#include "Timer.hpp"
#include "GameBoard.hpp"
#include "Strategy.h"

GameBoard UCT::nowOpponentBoard, UCT::nowMyBoard;
int *UCT::nowTop = nullptr;
int UCT::M = 0, UCT::N = 0, UCT::noX = 0, UCT::noY = 0;

UCT::~UCT() {
	//root->freeMemory();
	delete root;
	delete timer;
}

void UCT::backPropagation(TreeNode *nowNode, double delta) {
	nowNode->backPropagation(delta);
}

TreeNode* UCT::treePolicy(TreeNode *node) {
	while (!node->isTerminal()) {
		if (node->expandableNumber > 0) {
			return node->expand();
		}
		else {
			node = node->bestChild();
		}
	}
	return node;
}

int UCT::calculateProfit(const GameBoard &opponentBoard, const GameBoard &myBoard, const int rest, const int player) {
	if (player == PLAYER_ME && opponentBoard.win()) {
		return PROFIT_OPPONENT_WIN;
	}
	if (player == PLAYER_OPPONENT && myBoard.win()) {
		return PROFIT_I_WIN;
	}
	if (rest == 0) {
		return PROFIT_TIE;
	}
	return UNTERMINAL_STATE;
}

double UCT::defaultPolicy(TreeNode *nowNode) {

	int x = nowNode->_x, y = nowNode->_y, player = nowNode->_player;

	int rest = 0;
	for (int i = 0; i < N; i++)
		rest += nowTop[i];

	auto profit = calculateProfit(nowOpponentBoard, nowMyBoard, rest, player);

	while (profit == UNTERMINAL_STATE)
	{
		y = rand() % N;
		while (nowTop[y] == 0) {
			y = rand() % N;
		}
		x = --nowTop[y];
		if (player == PLAYER_OPPONENT) {
			nowOpponentBoard.place(x, y);
		}
		else {
			nowMyBoard.place(x, y);
		}
		rest--;

		if (x - 1 == noX && y == noY) {
			nowTop[y]--;
			rest--;
		}

		if (player == PLAYER_ME) {
			player = PLAYER_OPPONENT;
		}
		else {
			player = PLAYER_ME;
		}

		profit = calculateProfit(nowOpponentBoard, nowMyBoard, rest, player);

	}
	return (double)profit;
}

UCT::UCT(int _M, int _N, int _noX, int _noY) {
	timer = new Timer();
	M = _M;
	N = _N;
	noX = _noX;
	noY = _noY;
}

std::pair<int, int> UCT::UCTSearch(int const* const* boardStart, const int *topStart)
{
	int **myBoard = new int*[M];
	int **opponentBoard = new int*[M];

	for (int i = 0; i < M; i++) {
		myBoard[i] = new int[N];
		opponentBoard[i] = new int[N];
		memcpy(myBoard[i], boardStart[i], N * sizeof(int));
		memcpy(opponentBoard[i], boardStart[i], N * sizeof(int));
	}

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (myBoard[i][j] == 2) {
				myBoard[i][j] = 0;
			}
			if (opponentBoard[i][j] == 1) {
				opponentBoard[i][j] = 0;
			}
		}
	}

	auto originalMyBoard = GameBoard{ M, N, myBoard };
	auto originalOpponentBoard = GameBoard{ M, N, opponentBoard };

	nowTop = new int[N];
	memcpy(nowTop, topStart, N * sizeof(int));

	root = new TreeNode(-1, -1, PLAYER_OPPONENT, nullptr);
	TreeNode::usedMemory = 0;
	int times = 0;

	srand(time(nullptr));

	while (timer->getElapsedMicroseconds() < TIME_LIMIT_MICROSECOND) {
		times++;
		nowOpponentBoard = originalOpponentBoard;
		nowMyBoard = originalMyBoard;
		memcpy(nowTop, topStart, N * sizeof(int));
		auto nowNode = treePolicy(root);
		auto delta = defaultPolicy(nowNode);
		backPropagation(nowNode, delta);
	}

	auto best = root->bestChild(false);
	auto result = std::pair<int, int>{ best->x(), best->y() };

	clearArray(M, N, myBoard);
	myBoard = nullptr;
	clearArray(M, N, opponentBoard);
	opponentBoard = nullptr;
	delete[] nowTop;
	nowTop = nullptr;

	printf("Searched %d times, taking %lf ns\n", times, timer->getElapsedMicroseconds());
	return result;


}