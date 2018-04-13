//
//  UCT.h
//  Strategy
//
//  Created by ”‡–¿Ω° on 16/5/11.
//  Copyright ? 2016ƒÍ Yongfeng Zhang. All rights reserved.
//
#ifndef UCT_h
#define UCT_h

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "chessboard.h"
#include "Timer.hpp"

#define PLAYER 1
#define COMPUTER 2
#define TIME_LIMIT_MICROSECOND 2.5e6
#define PLAYER_WIN_PROFIT -1
#define COMPUTER_WIN_PROFIT 1
#define TIE_PROFIT 0
#define UNTERMINAL_STATE 2
#define COEFFICIENT 0.9
#define eps 1e-8
#define SQRT_MAGIC_F 0x5f3759df
#define maxMemoryPool 20000000

class Node;

class UCT;

class Node
{
    static int M, N;
    static int _noX, _noY;
    int _chessman;
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
    Node(int x = -1, int y = -1, int chessman = COMPUTER, Node *_father = NULL);
    ~Node()
    {
        //printf("Node was freed\n");
    }
    int x(){return _x;}
    int y(){return _y;}
    bool isTerminal();
    
    void set(int x = -1, int y = -1, int chessman = COMPUTER, Node *_father = NULL);
    
    Node* newNode(int x = -1, int y = -1, int chessman = COMPUTER, Node *_father = NULL);
    
    Node *expand();
    
    Node *bestChild();
    
    void backUp(double delta);
};


class UCT
{
    Node *root;
    int M, N;
    int _noX, _noY;
    int startTime;
	Timer *timer;
    
    void randomPlace(chessboard &computerBoard, chessboard &playerBoard, int *top, int &chessman, int &x, int &y, int &rest);
    Node *treePolicy(Node *nowNode);
    
    int calcProfit(chessboard &computerBoard, chessboard &playerBoard, int *top, int &rest, int chessman, int &x, int &y);
    double defaultPolicy(Node *nowNode);
    void backUp(Node *nowNode, double delta)
    {
        nowNode->backUp(delta);
    }
    
public:
    UCT(int _M, int _N, int noX, int noY):M(_M), N(_N), _noX(noX), _noY(noY), startTime(clock())
    {
		timer = new Timer();
    }
    
    Node *UCTSearch(int **boardStart, int *topStart);
    ~UCT() {delete root;}
};
#endif /* UCT_h */
