//
//  chessboard.h
//  Strategy
//
//  Created by ”‡–¿Ω° on 16/5/11.
//  Copyright ? 2016ƒÍ Yongfeng Zhang. All rights reserved.
//

#ifndef chessboard_h
#define chessboard_h

#include <iostream>
#include "Point.h"
#include "Judge.h"
#include "Strategy.h"
#include <bitset>

class chessboard
{
    static int N, M;
    std::bitset<144> bitCb;
    friend class UCT;
public:
    
    void place(int x, int y)
    {
        bitCb[M * y + (M - 1 - x)] = 1;
    }
    chessboard(){}
    chessboard(int* const* board)
    {
        for (int i = 0; i < N; i++)
            for (int j = M - 1; j >= 0; j--)
            {
                if (board[j][i] != 0)
                    bitCb[(M * i) + (M - 1 - j)] = 1;
            }
    }
    void output()
    {
        printf("=========hehe=========\n");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d ", get(i,j));
            printf("\n");
        }
        printf("=========heheEnd======\n");
    }

    
    bool get(int x, int y)
    {
        return bitCb[(M * y + (M - 1 - x))];
    }
    
    bool neWin(int x, int y)
    {
        
        if ((bitCb & (bitCb >> 1) & (bitCb >> 2) & (bitCb >> 3)).any())
            return 1;
        
        if ((bitCb & (bitCb >> M) & (bitCb >> M * 2) & (bitCb >> M * 3)).any())
            return 1;
        
        if ((bitCb & (bitCb >> (M + 1)) & (bitCb >> (M * 2 + 2)) & (bitCb >> (M * 3 + 3))).any())
            return 1;
        
        if ((bitCb & (bitCb >> (M - 1)) & (bitCb >> (M * 2 - 2)) & (bitCb >> (M * 3 - 3))).any())
            return 1;
        
        return 0;
            
    }
    
    bool win(int x, int y)
    {
        //printf("get(x, y) = %d\n", get(x, y));
        int i, j;
        int count = 0;
        
        for (i = x; i < M; i++)
            if (!get(i , y))
                break;
        count += (i - x);
        if (count >= 4) return 1;
        
        
        
        count = 0;
        for (i = y; i >= 0; i--)
            if (!get(x, i))
                break;
        count += (y - i);
        
        for (i = y; i < N; i++)
            if (!get(x, i))
                break;
        count += (i - y - 1);
        //printf("%d %d, line: %d\n", x, y, count);
        if (count >= 4) return 1;
        
        
        count = 0;
        for (i = x, j = y; i < M && j >= 0; i++, j--)
            if (get(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i >= 0 && j < N; i--, j++)
            if (get(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        count = 0;
        for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
            if (get(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i < M && j < N; i++, j++)
            if (get(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        return 0;
    }
};

/*
class chessboard
{
    int N, M;
    unsigned long long cb[3];
    std::bitset<144> bitCb;
    
public:
    
    void place(int x, int y)
    {
        int tmpy = y % 4;
        cb[y/4] |= 1LL << (M * tmpy + (M - 1 - x));
        bitCb[M * y + (M - 1 - x)] = 1;
    }
    chessboard(int _M, int _N, int* const* board):M(_M), N(_N)
    {
        for (int i = 0; i < _N; i++)
            for (int j = _M - 1; j >= 0; j--)
            {
                if (board[j][i] != 0)
                    bitCb[(_M * i) + (_M - 1 - j)] = 1;
            }
        cb[0] = cb[1] = cb[2] = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = _M - 1; j >= 0; j--)
                if (board[j][i] != 0)
                    cb[0] |= 1LL << (_M * i + (_M - 1 - j));
        }
        
        for (int i = 4; i < 8; i++)
        {
            for (int j = _M - 1; j >= 0; j--)
                if (board[j][i] != 0)
                    cb[1] |= 1LL << (_M * (i - 4) + (_M - 1 - j));
        }
        
        for (int i = 8; i < _N; i++)
        {
            for (int j = _M - 1; j >= 0; j--)
                if (board[j][i] != 0)
                    cb[2] |= 1LL << (_M * (i - 8) + (_M - 1 - j));
        }
        
        printf("=========board=========\n");
        for (int i = 0; i < _M; i++)
        {
            for (int j = 0; j < _N; j++)
                printf("%d ", board[i][j]);
            printf("\n");
        }
        printf("=========boardEnd======\n");
        
        printf("=========hehe=========\n");
        for (int i = 0; i < _M; i++)
        {
            for (int j = 0; j < _N; j++)
                printf("%d ", get(i,j));
            printf("\n");
        }
        printf("=========heheEnd======\n");
 
        
        
    }
    void output()
    {
        printf("=========hehe=========\n");
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%d ", get(i,j));
            printf("\n");
        }
        printf("=========heheEnd======\n");
    }
    bool bitGet(int x, int y)
    {
        return bitCb[(M * y + (M - 1 - x))];
    }
    
    bool get(int x, int y)
    {
        long long tmp = cb[y / 4];
        y = y % 4;
        bool ans = ((1LL << (M * y + (M - 1 - x))) & tmp) != 0;
        return ans;
    }
    
    bool bitWin(int x, int y)
    {
        //printf("get(x, y) = %d\n", get(x, y));
        int i, j;
        int count = 0;
        
        for (i = x; i < M; i++)
            if (!bitGet(i , y))
                break;
        count += (i - x);
        if (count >= 4) return 1;
        
        
        
        count = 0;
        for (i = y; i >= 0; i--)
            if (!bitGet(x, i))
                break;
        count += (y - i);
        
        for (i = y; i < N; i++)
            if (!bitGet(x, i))
                break;
        count += (i - y - 1);
        //printf("%d %d, line: %d\n", x, y, count);
        if (count >= 4) return 1;
        
        
        count = 0;
        for (i = x, j = y; i < M && j >= 0; i++, j--)
            if (bitGet(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i >= 0 && j < N; i--, j++)
            if (bitGet(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        count = 0;
        for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
            if (bitGet(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i < M && j < N; i++, j++)
            if (bitGet(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        return 0;
    }
    
    bool win(int x, int y)
    {
        //printf("get(x, y) = %d\n", get(x, y));
        int i, j;
        int count = 0;
        
        for (i = x; i < M; i++)
            if (!get(i , y))
                break;
        count += (i - x);
        if (count >= 4) return 1;
        

        
        count = 0;
        for (i = y; i >= 0; i--)
            if (!get(x, i))
                break;
        count += (y - i);
        
        for (i = y; i < N; i++)
            if (!get(x, i))
                break;
        count += (i - y - 1);
        //printf("%d %d, line: %d\n", x, y, count);
        if (count >= 4) return 1;
        
        
        count = 0;
        for (i = x, j = y; i < M && j >= 0; i++, j--)
            if (get(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i >= 0 && j < N; i--, j++)
            if (get(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        count = 0;
        for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
            if (get(i, j) == 0)
                break;
        count += (y - j);
        for (i = x, j = y; i < M && j < N; i++, j++)
            if (get(i, j) == 0)
                break;
        count += (j - y - 1);
        if (count >= 4) return 1;
        
        return 0;
    }
};
 */

#endif /* chessboard_h */
