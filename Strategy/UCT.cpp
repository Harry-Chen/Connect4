//
//  UCT.cpp
//  Strategy
//
//  Created by ”‡–¿Ω° on 16/5/18.
//  Copyright ? 2016ƒÍ Yongfeng Zhang. All rights reserved.
//
#include "UCT.h"
#include "chessboard.h"
#include "fmath.hpp"

int usedMemory = 0;
Node **pool = new Node*[maxMemoryPool];

chessboard nowComputerBoard, nowPlayerBoard;
int *nowTop;

int Node::M = 0;
int Node::N = 0;
int Node::_noX = 0;
int Node::_noY = 0;
int chessboard::M = 0;
int chessboard::N = 0;

Node::Node(int x, int y, int chessman, Node *_father):_x(x), _y(y), _chessman(chessman), visNum(0), profit(0), father(_father), expNum(0), _isTerminal(-1)
{
    children = new Node*[N];
    expNode = new int[N];
    for (int i = 0; i < N; i++)
    {
        //if (top[i] != 0)
        if (nowTop[i] != 0)
            expNode[expNum++] = i;
        children[i] = NULL;
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
    //if (_chessman == PLAYER && computerBoard.win(_x, _y) != computerBoard.oldWin(_x, _y))
    //    printf("Wrong newWin!\n");
    
    if ( (_chessman == PLAYER && nowComputerBoard.win(_x, _y)) ||
        (_chessman == COMPUTER && nowPlayerBoard.win(_x,_y)) ||
        expNum == 0
        )
    {
        _isTerminal = 1;
        return _isTerminal;
    }
    _isTerminal = 0;
    return _isTerminal;
}

void Node::set(int x, int y, int chessman, Node *_father)
{
    visNum = profit = expNum = 0;
    _isTerminal = -1;
    father = _father;
    _chessman = chessman;
    _x = x;
    _y = y;
    for (int i = 0; i < N; i++)
    {
        if (nowTop[i] != 0)
            expNode[expNum++] = i;
        children[i] = NULL;
    }
}

Node* Node::newNode(int x, int y, int chessman, Node *_father)
{
    if(pool[usedMemory] == NULL)
    {
        pool[usedMemory] = new Node(x, y, chessman, _father);
        return pool[usedMemory++];
    }
    //printf("reuse memory %d\n", usedMemory);
    pool[usedMemory]->set(x, y, chessman, _father);
    return pool[usedMemory++];
}

Node* Node::expand()
{
    int index = rand() % expNum;
    int newY = expNode[index], newX = -- nowTop[newY];
    if (_chessman == COMPUTER)
    {
        nowComputerBoard.place(newX, newY);
    }
    else
    {
        nowPlayerBoard.place(newX, newY);
    }
    if (newX - 1 == _noX && newY == _noY)
        nowTop[newY] --;
    if (_chessman == COMPUTER)
        children[newY] = newNode(newX, newY, PLAYER, this);
    else
        children[newY] = newNode(newX, newY, COMPUTER, this);
    std::swap(expNode[index], expNode[--expNum]);
    return children[newY];
}

Node* Node::bestChild()
{
    Node *bestNode;
    double maxProfit = -RAND_MAX;
    int bestY;
    for (int i = 0; i < N; i++)
    {
        if (children[i] == NULL)
            continue;
        double realProfit = children[i]->profit * (_chessman == COMPUTER ? 1 : -1);
        int ChildVisNum = children[i]->visNum;
        double tmp = realProfit / ChildVisNum + sqrt(2 * log((double)visNum) / (double)ChildVisNum) * COEFFICIENT;
        if (tmp > maxProfit - eps || (fabs(tmp - maxProfit) < eps && rand() % 2 == 0))
        {
            maxProfit = tmp;
            bestNode = children[i];
            bestY = i;
        }
    }
    if (_chessman == COMPUTER)
        nowComputerBoard.place(--nowTop[bestY], bestY);
    else
        nowPlayerBoard.place(--nowTop[bestY], bestY);
    if (nowTop[bestY] - 1 == _noX && bestY == _noY)
        nowTop[bestY] --;
    
    return bestNode;
}

void Node::backUp(double delta)
{
    Node *nowNode = this;
    while (nowNode)
    {
        nowNode->visNum ++;
        nowNode->profit += delta;
        nowNode = nowNode->father;
    }
}


void UCT::randomPlace(chessboard &computerBoard, chessboard &playerBoard, int *top, int &chessman, int &x, int &y, int &rest)
{
    y = rand() % N;
    while(top[y] == 0)
        y = rand() % N;
    x = --top[y];
    if (chessman == COMPUTER)
        computerBoard.place(x, y);
    else
        playerBoard.place(x, y);
    rest --;
    if (x - 1 == _noX && y == _noY)
    {
        top[y] --;
        rest --;
    }
}

Node* UCT::treePolicy(Node *nowNode)
{
    while(!nowNode->isTerminal())
    {
        if (nowNode->expNum > 0)
        {
            return nowNode->expand();
        }
        else
            nowNode = nowNode->bestChild();
    }
    return nowNode;
}

int UCT::calcProfit(chessboard &computerBoard, chessboard &playerBoard, int *top, int &rest, int chessman, int &x, int &y)
{
    if (chessman == PLAYER && computerBoard.win(x, y))
    {
        //computerBoard.output();
        return COMPUTER_WIN_PROFIT;
    }
    if (chessman == COMPUTER && playerBoard.win(x, y))
    {
        //playerBoard.output();
        return PLAYER_WIN_PROFIT;
    }
    if (rest == 0)
        return TIE_PROFIT;
    return UNTERMINAL_STATE;
}

double UCT::defaultPolicy(Node *nowNode)
{
    //chessboard computerBoard = nowNode->computerBoard, playerBoard = nowNode->playerBoard;
    //chessboard computerBoard = nowComputerBoard, playerBoard = nowPlayerBoard;
    
    int x = nowNode->_x, y = nowNode->_y, chessman = nowNode->_chessman;
    
    int rest = 0;
    for (int i = 0; i < N; i++)
        rest += nowTop[i];
    
    int profit;
    if (chessman == PLAYER && nowComputerBoard.win(x, y))
        profit = COMPUTER_WIN_PROFIT;
    else if (chessman == COMPUTER && nowPlayerBoard.win(x, y))
        profit = PLAYER_WIN_PROFIT;
    else if (rest == 0)
        profit = TIE_PROFIT;
    else
        profit = UNTERMINAL_STATE;
    
    while (profit == UNTERMINAL_STATE)
    {
        //randomPlace(nowComputerBoard, nowPlayerBoard, nowTop, chessman, x, y, rest);
        y = rand() % N;
        while(nowTop[y] == 0)
            y = rand() % N;
        x = --nowTop[y];
        if (chessman == COMPUTER)
            nowComputerBoard.place(x, y);
        else
            nowPlayerBoard.place(x, y);
        rest --;
        if (x - 1 == _noX && y == _noY)
        {
            nowTop[y] --;
            rest --;
        }
        if (chessman == PLAYER)
            chessman = COMPUTER;
        else
            chessman = PLAYER;
        
        if (chessman == PLAYER && nowComputerBoard.win(x, y))
            profit = COMPUTER_WIN_PROFIT;
        else if (chessman == COMPUTER && nowPlayerBoard.win(x, y))
            profit = PLAYER_WIN_PROFIT;
        else if (rest == 0)
            profit = TIE_PROFIT;
    
        //profit = calcProfit(nowComputerBoard, nowPlayerBoard, nowTop, rest, chessman, x, y);
        
        /*
         if (profit == PLAYER_WIN_PROFIT)
         playerBoard.output();
         if (profit == COMPUTER_WIN_PROFIT)
         computerBoard.output();
         if (profit == TIE_PROFIT)
         {
         computerBoard.output();
         playerBoard.output();
         }
         */
    }
    return (double)profit;
}

Node* UCT::UCTSearch(int **boardStart, int *topStart)
{
    Node::M = M;
    Node::N = N;
    Node::_noX = _noX;
    Node::_noY = _noY;
    chessboard::M = M;
    chessboard::N = N;
    
    chessboard startComputerBoard, startPlayerBoard;
    int **myBoard = new int*[M];
    for (int i = 0; i < M; i++)
    {
        myBoard[i] = new int[N];
        memcpy(myBoard[i], boardStart[i], N * sizeof(int));
    }
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            if (myBoard[i][j] == 2)
                myBoard[i][j] = 0;
        }
    chessboard playerBoard(myBoard);
    startPlayerBoard = playerBoard;
    
    for (int i = 0; i < M; i++)
        memcpy(myBoard[i], boardStart[i], N * sizeof(int));
    
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            if (myBoard[i][j] == 1)
                myBoard[i][j] = 0;
        }
    chessboard computerBoard(myBoard);
    startComputerBoard = computerBoard;
    
    for (int i = 0; i < M; i++)
        delete[] myBoard[i];
    delete[] myBoard;
    
    nowTop = new int[N];
    memcpy(nowTop, topStart, N * sizeof(int));

    
    root = new Node();
    usedMemory = 0;
    int times = 0;

    srand(time(NULL));
    
    while(timer->getElapsedMicroseconds() < TIME_LIMIT_MICROSECOND)
    {
        times++;
        nowComputerBoard = startComputerBoard;
        nowPlayerBoard = startPlayerBoard;
        memcpy(nowTop, topStart, N * sizeof(int));
        Node *nowNode = treePolicy(root);
        double delta = defaultPolicy(nowNode);
        backUp(nowNode, delta);
    }
    printf("%d\n", times);
    return root->bestChild();
    
}