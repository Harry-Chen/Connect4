#include "TreeNode.h"
#include "GameBoard.hpp"
#include "UCT.h"

#include "fmath.hpp"

TreeNode** TreeNode::pool = new TreeNode*[MAX_MEMORY_POOL_SIZE]();
size_t TreeNode::usedMemory = 0;

TreeNode::TreeNode(int x, int y, int player, TreeNode *_father) {
	children = new TreeNode*[UCT::N];
	expandableNodes = new int[UCT::N];
	set(x, y, player, _father);
}

void TreeNode::set(int x, int y, int player, TreeNode *_father) {
	visNum = 0;
	profit = 0;
	expandableNumber = 0;
	_isTerminal = -1;
	father = _father;
	_player = player;
	_x = x;
	_y = y;
	for (int i = 0; i < UCT::N; i++) {
		if (UCT::nowTop[i] != 0) {
			expandableNodes[expandableNumber++] = i;
		}
		children[i] = nullptr;
	}
}

bool TreeNode::isTerminal()
{
	if (_isTerminal != -1) {
		return _isTerminal;
	}

	if (_x == -1 && _y == -1) {
		_isTerminal = 0;
		return _isTerminal;
	}

	if ((_player == PLAYER_ME && UCT::nowOpponentBoard.win()) ||
		(_player == PLAYER_OPPONENT && UCT::nowMyBoard.win()) ||
		expandableNumber == 0
		) {
		_isTerminal = 1;
		return _isTerminal;
	}
	_isTerminal = 0;
	return _isTerminal;
}



TreeNode* TreeNode::newNode(int x, int y, int player, TreeNode *_father) {
	if (usedMemory >= MAX_MEMORY_POOL_SIZE) {
		return new TreeNode(x, y, player, _father);
	}
	else if (pool[usedMemory] == nullptr) {
		pool[usedMemory] = new TreeNode(x, y, player, _father);
		return pool[usedMemory++];
	}
	pool[usedMemory]->set(x, y, player, _father);
	return pool[usedMemory++];
}

TreeNode* TreeNode::expand() {
	int index = rand() % expandableNumber;
	int newY = expandableNodes[index], newX = --UCT::nowTop[newY];

	if (_player == PLAYER_OPPONENT) {
		UCT::nowOpponentBoard.place(newX, newY);
	}
	else {
		UCT::nowMyBoard.place(newX, newY);
	}

	if (newX - 1 == UCT::noX && newY == UCT::noY) {
		UCT::nowTop[newY]--;
	}

	if (_player == PLAYER_OPPONENT) {
		children[newY] = newNode(newX, newY, PLAYER_ME, this);
	}
	else {
		children[newY] = newNode(newX, newY, PLAYER_OPPONENT, this);
	}
	std::swap(expandableNodes[index], expandableNodes[--expandableNumber]);
	return children[newY];
}

TreeNode* TreeNode::bestChild(bool placeNewChess)
{
	TreeNode *bestNode;
	double maxProfit = -RAND_MAX;
	int bestY;
	for (int i = 0; i < UCT::N; i++) {
		if (children[i] == nullptr) {
			continue;
		}
		double realProfit = children[i]->profit * (_player == PLAYER_OPPONENT ? 1 : -1);
		int ChildVisNum = children[i]->visNum;

		double tmp = realProfit / ChildVisNum + sqrt(2 * fmath::log(visNum) / (double)ChildVisNum) * COEFFICIENT;
		if (tmp > maxProfit - eps || (fabs(tmp - maxProfit) < eps && rand() % 2 == 0)) {
			maxProfit = tmp;
			bestNode = children[i];
			bestY = i;
		}
	}
	if (placeNewChess) {
		if (_player == PLAYER_OPPONENT) {
			UCT::nowOpponentBoard.place(--UCT::nowTop[bestY], bestY);
		}
		else {
			UCT::nowMyBoard.place(--UCT::nowTop[bestY], bestY);
		}

		if (UCT::nowTop[bestY] - 1 == UCT::noX && bestY == UCT::noY) {
			UCT::nowTop[bestY] --;
		}
	}

	return bestNode;
}

void TreeNode::backPropagation(double delta)
{
	TreeNode *nowNode = this;
	while (nowNode)
	{
		nowNode->visNum++;
		nowNode->profit += delta;
		nowNode = nowNode->father;
	}
}

void TreeNode::freeMemory() {
	delete[] expandableNodes;
	for (int i = 0; i < UCT::N; ++i) {
		if (children[i] != nullptr) {
			children[i]->freeMemory();
			delete children[i];
			children[i] = nullptr;
		}
	}
	delete[] children;
}
