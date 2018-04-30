#ifndef TREENODE_H
#define TREENODE_H

#include "Constants.h"

class TreeNode
{
	friend class UCT;

	int _player;
	int _x, _y;
	int visNum;
	double profit;
	TreeNode *father;
	TreeNode **children;
	int expandableNumber;
	int *expandableNodes;
	int _isTerminal;
	static TreeNode **pool;

public:
	TreeNode(int x, int y, int player, TreeNode *_father);
	~TreeNode() {}
	int x() { return _x; }
	int y() { return _y; }
	bool isTerminal();

	void set(int x, int y, int player, TreeNode *_father);

	TreeNode* newNode(int x, int y, int player, TreeNode *_father);

	TreeNode *expand();

	TreeNode *bestChild(bool placeNewChess = true);

	void backPropagation(double delta);

	static size_t usedMemory;

	void freeMemory();
};

#endif