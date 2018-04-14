#ifndef UCT_h
#define UCT_h

#include <utility>

#include "Constants.h"

class GameBoard;
class Timer;
class TreeNode;

class UCT {
	TreeNode *root;

	Timer *timer;

	double defaultPolicy(TreeNode *nowNode);
	TreeNode *treePolicy(TreeNode *nowNode);
	void backPropagation(TreeNode *nowNode, double delta);


	static int calculateProfit(const GameBoard &opponentBoard, const GameBoard &myBoard, const int rest, const int player);

public:
	UCT(int _M, int _N, int _noX, int _noY);

	std::pair<int, int> UCTSearch(int const* const* boardStart, const int *topStart);

	~UCT();
	
	static GameBoard nowOpponentBoard, nowMyBoard;
	static int *nowTop;
	static int M, N, noX, noY;
};
#endif /* UCT_h */ 
