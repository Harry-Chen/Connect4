#pragma once

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
	//static TreeNode **pool;

public:
	TreeNode(int x = -1, int y = -1, int player = PLAYER_OPPONENT, TreeNode *_father = nullptr);
	~TreeNode() {}
	int x() { return _x; }
	int y() { return _y; }
	bool isTerminal();

	void set(int x = -1, int y = -1, int player = PLAYER_OPPONENT, TreeNode *_father = nullptr);

	TreeNode* newNode(int x = -1, int y = -1, int player = PLAYER_OPPONENT, TreeNode *_father = nullptr);

	TreeNode *expand();

	TreeNode *bestChild();

	void backPropagation(double delta);

	static size_t usedMemory;

	void freeMemory();
};

