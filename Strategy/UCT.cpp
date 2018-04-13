#include "UCT.h"
#include "GameBoard.h"
#include "fmath.hpp"

int usedMemory = 0;
Node **pool = new Node*[MAX_MEMORY_POOL_SIZE]();

GameBoard nowOpponentBoard, nowMyBoard;
int *nowTop = nullptr;

int Node::M = 0;
int Node::N = 0;
int Node::_noX = 0;
int Node::_noY = 0;
int GameBoard::M = 0;
int GameBoard::N = 0;

Node::Node(int x, int y, int player, Node *_father) :_x(x), _y(y), _player(player), visNum(0), profit(0), father(_father), expNum(0), _isTerminal(-1)
{
	children = new Node*[N];
	expNode = new int[N];
	for (int i = 0; i < N; i++)
	{
		if (nowTop[i] != 0)
			expNode[expNum++] = i;
		children[i] = nullptr;
	}
}

bool Node::isTerminal()
{
	if (_isTerminal != -1)
		return _isTerminal;
	if (_x == -1 && _y == -1)
	{
		_isTerminal = 0;
		return _isTerminal;
	}
	//if (_player == PLAYER_ME && opponentBoard.win(_x, _y) != opponentBoard.oldWin(_x, _y))
	//    printf("Wrong newWin!\n");

	if ((_player == PLAYER_ME && nowOpponentBoard.win()) ||
		(_player == PLAYER_OPPONENT && nowMyBoard.win()) ||
		expNum == 0
		) 
	{
		_isTerminal = 1;
		return _isTerminal;
	}
	_isTerminal = 0;
	return _isTerminal;
}

void Node::set(int x, int y, int player, Node *_father) {
	visNum = 0;
	profit = 0;
	expNum = 0;
	_isTerminal = -1;
	father = _father;
	_player = player;
	_x = x;
	_y = y;
	for (int i = 0; i < N; i++) {
		if (nowTop[i] != 0) {
			expNode[expNum++] = i;
		}
		children[i] = nullptr;
	}
}

Node* Node::newNode(int x, int y, int player, Node *_father)
{
	if (pool[usedMemory] == nullptr) {
		pool[usedMemory] = new Node(x, y, player, _father);
		return pool[usedMemory++];
	}
	pool[usedMemory]->set(x, y, player, _father);
	return pool[usedMemory++];
}

Node* Node::expand()
{
	int index = rand() % expNum;
	int newY = expNode[index], newX = --nowTop[newY];

	if (_player == PLAYER_OPPONENT) {
		nowOpponentBoard.place(newX, newY);
	}
	else {
		nowMyBoard.place(newX, newY);
	}

	if (newX - 1 == _noX && newY == _noY) {
		nowTop[newY]--;
	}

	if (_player == PLAYER_OPPONENT) {
		children[newY] = newNode(newX, newY, PLAYER_ME, this);
	}
	else {
		children[newY] = newNode(newX, newY, PLAYER_OPPONENT, this);
	}
	std::swap(expNode[index], expNode[--expNum]);
	return children[newY];
}

Node* Node::bestChild()
{
	Node *bestNode;
	double maxProfit = -RAND_MAX;
	int bestY;
	for (int i = 0; i < N; i++) {
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
	if (_player == PLAYER_OPPONENT) {
		nowOpponentBoard.place(--nowTop[bestY], bestY);
	}
	else {
		nowMyBoard.place(--nowTop[bestY], bestY);
	}

	if (nowTop[bestY] - 1 == _noX && bestY == _noY) {
		nowTop[bestY] --;
	}

	return bestNode;
}

void Node::backPropagation(double delta)
{
	Node *nowNode = this;
	while (nowNode)
	{
		nowNode->visNum++;
		nowNode->profit += delta;
		nowNode = nowNode->father;
	}
}


Node* UCT::treePolicy(Node *nowNode) {
	while (!nowNode->isTerminal()) {
		if (nowNode->expNum > 0) {
			return nowNode->expand();
		}
		else {
			nowNode = nowNode->bestChild();
		}
	}
	return nowNode;
}

int UCT::calculateProfit(const GameBoard &opponentBoard,const GameBoard &myBoard,const int rest,const int player) {
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

double UCT::defaultPolicy(Node *nowNode) {

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

		if (x - 1 == _noX && y == _noY) {
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

std::pair<int, int> UCT::UCTSearch(int const* const* boardStart, const int *topStart)
{
	Node::M = M;
	Node::N = N;
	Node::_noX = _noX;
	Node::_noY = _noY;
	GameBoard::M = M;
	GameBoard::N = N;

	int **myBoard = new int*[M];
	int **opponentBoard = new int*[M];

	for (int i = 0; i < M; i++)
	{
		myBoard[i] = new int[N];
		opponentBoard[i] = new int[N];
		memcpy(myBoard[i], boardStart[i], N * sizeof(int));
		memcpy(opponentBoard[i], boardStart[i], N * sizeof(int));
	}

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++)
		{
			if (myBoard[i][j] == 2) {
				myBoard[i][j] = 0;
			}
			if (opponentBoard[i][j] == 1) {
				opponentBoard[i][j] = 0;
			}
		}
	}

	auto originalMyBoard = GameBoard{ myBoard };
	auto originalOpponentBoard = GameBoard{ opponentBoard };

	for (int i = 0; i < M; i++) {
		delete[] myBoard[i];
		delete[] opponentBoard[i];
	}
	delete[] myBoard;
	delete[] opponentBoard;

	nowTop = new int[N];
	memcpy(nowTop, topStart, N * sizeof(int));


	root = new Node();
	usedMemory = 0;
	int times = 0;

	srand(time(nullptr));

	while (timer->getElapsedMicroseconds() < TIME_LIMIT_MICROSECOND)
	{
		times++;
		nowOpponentBoard = originalOpponentBoard;
		nowMyBoard = originalMyBoard;
		memcpy(nowTop, topStart, N * sizeof(int));
		Node *nowNode = treePolicy(root);
		double delta = defaultPolicy(nowNode);
		backPropagation(nowNode, delta);
	}

	auto best = root->bestChild();
	auto result = std::pair<int, int>{ best->x(), best->y() };
	delete[] nowTop;
	printf("Searched %d times, taking %lf ns\n", times, timer->getElapsedMicroseconds());
	return result;

}