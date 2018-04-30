#include <ctime>

#include "UCT.h"
#include "TreeNode.h"
#include "Timer.hpp"
#include "GameBoard.hpp"
#include "Strategy.h"

GameBoard UCT::nowOpponentBoard, UCT::nowMyBoard;
int *UCT::nowTop = nullptr;
int UCT::M = 0, UCT::N = 0, UCT::noX = 0, UCT::noY = 0;

const Point NO_MOVE = { -1, -1 };
const UCT::MoveResult UCT::DOOM_TO_LOSE = { UCT::OptimalMoveResult::DOOM_TO_LOSE, NO_MOVE };
const UCT::MoveResult UCT::NOT_FOUND = { UCT::OptimalMoveResult::NOT_FOUND, NO_MOVE };

UCT::~UCT() {
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

const UCT::MoveResult UCT::generateResult(int X, int Y) {
	if (X == -2) throw - 1;
	return { UCT::OptimalMoveResult::MOVE_DONE,{ X , Y } };
}

const UCT::MoveResult UCT::getOptimalMove(GameBoard &thatBoard, int opponentLastX, int opponentLastY) {

	{
		// vertical 
		auto verticalDown = 0;
		for (verticalDown = opponentLastX; verticalDown < M; verticalDown++) {
			if (!thatBoard.get(verticalDown, opponentLastY)) {
				break;
			}
		}
		if (opponentLastX - verticalDown == 3 && nowTop[opponentLastY] == opponentLastX) {
			return generateResult(opponentLastX - 1, opponentLastY);
		}
	}

	{
		// horizontal
		auto horizontalLeft = 0, horizontalRight = 0;
		for (horizontalLeft = opponentLastY; horizontalLeft >= 0; horizontalLeft--) {
			if (!thatBoard.get(opponentLastX, horizontalLeft)) {
				break;
			}
		}
		for (horizontalRight = opponentLastY; horizontalRight < N; horizontalRight++) {
			if (!thatBoard.get(opponentLastX, horizontalRight)) {
				break;
			}
		}
		if (horizontalRight - horizontalLeft + 1 == 5) {
			auto rightEmpty = (horizontalRight < N) && nowTop[horizontalRight] == opponentLastX + 1;
			auto leftEmpty = (horizontalLeft >= 0) && nowTop[horizontalLeft] == opponentLastX + 1;
			if (leftEmpty && rightEmpty) {
				return DOOM_TO_LOSE;
			}
			else if (leftEmpty) {
				return generateResult(opponentLastX, horizontalLeft);
			}
			else if (rightEmpty) {
				return generateResult(opponentLastX, horizontalRight);
			}
		}
	}

	{
		// slash
		auto slashLeftOffset = 0, slashRightOffset = 0;
		for (; opponentLastY - slashLeftOffset >= 0 && opponentLastX + slashLeftOffset < M; slashLeftOffset++) {
			if (!thatBoard.get(opponentLastX + slashLeftOffset, opponentLastY - slashLeftOffset)) {
				break;
			}
		}
		for (; opponentLastY + slashRightOffset < N && opponentLastX - slashRightOffset >= 0; slashRightOffset++) {
			if (!thatBoard.get(opponentLastX - slashRightOffset, opponentLastY + slashRightOffset)) {
				break;
			}
		}
		if (slashLeftOffset + slashRightOffset + 1 == 5) {
			auto rightEmpty = (opponentLastX - slashRightOffset >= 0) && (opponentLastY + slashRightOffset < N) && nowTop[opponentLastY + slashRightOffset] == opponentLastX - slashRightOffset + 1;
			auto leftEmpty = (opponentLastX + slashLeftOffset < M) && (opponentLastY - slashLeftOffset >= 0) && nowTop[opponentLastY - slashLeftOffset] == opponentLastX + slashLeftOffset + 1;
			if (leftEmpty && rightEmpty) {
				return DOOM_TO_LOSE;
			}
			else if (leftEmpty) {
				return generateResult(opponentLastX + slashLeftOffset, opponentLastY - slashLeftOffset);
			}
			else if (rightEmpty) {
				return generateResult(opponentLastX - slashRightOffset, opponentLastY + slashRightOffset);
			}
		}
	}

	{
		// bslash
		auto slashLeftOffset = 0, slashRightOffset = 0;
		for (; opponentLastY - slashLeftOffset >= 0 && opponentLastX - slashLeftOffset >= 0; slashLeftOffset++) {
			if (!thatBoard.get(opponentLastX - slashLeftOffset, opponentLastY - slashLeftOffset)) {
				break;
			}
		}
		for (; opponentLastY + slashRightOffset < N && opponentLastX + slashRightOffset < M; slashRightOffset++) {
			if (!thatBoard.get(opponentLastX + slashRightOffset, opponentLastY + slashRightOffset)) {
				break;
			}
		}
		if (slashLeftOffset + slashRightOffset + 1 == 5) {
			auto rightEmpty = (opponentLastX + slashRightOffset < M) && (opponentLastY + slashRightOffset < N) && nowTop[opponentLastY + slashRightOffset] == opponentLastX + slashRightOffset +1 ;
			auto leftEmpty = (opponentLastX - slashLeftOffset >= 0) && (opponentLastY - slashLeftOffset >= 0) && nowTop[opponentLastY - slashLeftOffset] == opponentLastX - slashLeftOffset + 1;
			if (leftEmpty && rightEmpty) {
				return DOOM_TO_LOSE;
			}
			else if (leftEmpty) {
				return generateResult(opponentLastX - slashLeftOffset, opponentLastY - slashLeftOffset);
			}
			else if (rightEmpty) {
				return generateResult(opponentLastX + slashRightOffset, opponentLastY + slashRightOffset);
			}
		}
	}

	return NOT_FOUND;
}

double UCT::defaultPolicy(TreeNode *nowNode) {

	int x = nowNode->_x, y = nowNode->_y, player = nowNode->_player;

	int rest = 0;
	for (int i = 0; i < N; i++)
		rest += nowTop[i];

	auto profit = calculateProfit(nowOpponentBoard, nowMyBoard, rest, player);

	while (profit == UNTERMINAL_STATE) {
		if (player == PLAYER_OPPONENT) {
			auto result = getOptimalMove(nowMyBoard, x, y);
			switch (result.first) {
			case UCT::OptimalMoveResult::DOOM_TO_LOSE:
				return PROFIT_I_WIN;
			case UCT::OptimalMoveResult::MOVE_DONE:
				x = result.second.x;
				y = result.second.y;
				nowOpponentBoard.place(x, y);
				--nowTop[y];
				break;
			case UCT::OptimalMoveResult::NOT_FOUND:
				y = rand() % N;
				while (nowTop[y] == 0) {
					y = rand() % N;
				}
				x = --nowTop[y];
				nowOpponentBoard.place(x, y);
				break;
			}
		}
		else {
			auto result = getOptimalMove(nowOpponentBoard, x, y);
			switch (result.first) {
			case UCT::OptimalMoveResult::DOOM_TO_LOSE:
				return PROFIT_OPPONENT_WIN;
			case UCT::OptimalMoveResult::MOVE_DONE:
				x = result.second.x;
				y = result.second.y;
				nowMyBoard.place(x, y);
				--nowTop[y];
				break;
			case UCT::OptimalMoveResult::NOT_FOUND:
				y = rand() % N;
				while (nowTop[y] == 0) {
					y = rand() % N;
				}
				x = --nowTop[y];
				nowMyBoard.place(x, y);
				break;
			}
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