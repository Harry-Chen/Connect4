#define _AFXDLL
#include <iostream>
#include <afx.h>
#include <cstring>
#include <Mmsystem.h>
#include "Compete.h"
#include "Point.h"
#include "Data.h"
#include "Judge.h"

using namespace std;

#pragma comment(lib, "winmm.lib")

typedef Point* (*GETPOINT)(const int M, const int N, const int* _top, const int* _board, const int lastX, const int lastY, const int noX, const int noY);
typedef void (*CLEARPOINT)(Point* p);

struct Param{
	int M;
	int N;
	int* top;
	int* board;
	int lastX;
	int lastY;
	int noX;
	int noY;
	GETPOINT getPoint;
	Point* p;
	int bugOccurred;
};

void callGetPoint(Param* param){
	try{
		param->p = param->getPoint(param->M, param->N, param->top, param->board, param->lastX, param->lastY, param->noX, param->noY);
	}catch(...){
		param->bugOccurred = 1;
		return;
	}
	param->bugOccurred = 0;
	return;
}

//传入getPointA 和 clearPointA
//returns : 0 - 平局结束 1 - A赢 2 - B赢 3 - A出错 4 - A给出非法落子 5 - B出错 6 - B给出非法落子 7 - A超时 8 - B超时 -1 - 游戏未结束
int AGo(GETPOINT getPoint, CLEARPOINT clearPoint, Data* data){
	int x, y;
	try{
		/*
		Point* p = getPoint(data->M, data->N, data->top, data->boardA, data->lastX, data->lastY, data->noX, data->noY);
		x = p->x;
		y = p->y;
		clearPoint(p);
		*/
		Param param;
		param.M = data->M;	param.N = data->N;	param.top = data->top;	param.board = data->boardA;
		param.lastX = data->lastX;	param.lastY = data->lastY;	param.noX = data->noX;	param.noY = data->noY;
		param.getPoint = getPoint;	param.p = NULL;	param.bugOccurred = 0;
		
		DWORD begin = timeGetTime();//计时开始
		
		DWORD threadID;
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)callGetPoint, &param, 0, &threadID);
		WaitForSingleObject(thread, MAX_TIME);
		
		DWORD end = timeGetTime();//计时结束
		timeA += (end - begin);
		
		if(param.p == NULL){//在规定时间内函数没有返回 或 出错
			if(param.bugOccurred == 0){//仅仅是超时
				TerminateThread(thread, 7);//强制关闭
				CloseHandle(thread);//释放资源
				return 7;
			}
			else{//出bug
				CloseHandle(thread);
				return 3;
			}
		}
		
		x = param.p->x;
		y = param.p->y;
		clearPoint(param.p);
		
		CloseHandle(thread);
	}
	catch(...){
		return 3;
	}
	
	if(!isLegal(x, y, data)){
		return 4;
	}
	data->lastX = x;
	data->lastY = y;
	data->boardA[x * data->N + y] = 2;
	data->boardB[x * data->N + y] = 1;
	data->top[y]--;
	//对不可落子点进行处理
	if(x == data->noX + 1 && y == data->noY){
		data->top[y]--;
	}
	
	if(AWin(x, y, data->M, data->N, data->boardA)){
		return 1;
	}
	if(isTie(data->N, data->top)){
		return 0;
	}
	return -1;
}

//传入getPointB 和 clearPointB
//returns : 0 - 平局结束 1 - A赢 2 - B赢 3 - A出错 4 - A给出非法落子 5 - B出错 6 - B给出非法落子 7 - A超时 8 - B超时 -1 - 游戏未结束
int BGo(GETPOINT getPoint, CLEARPOINT clearPoint, Data* data){
	int x, y;
	try{
		/*
		Point* p = getPoint(data->M, data->N, data->top, data->boardB, data->lastX, data->lastY, data->noX, data->noY);
		x = p->x;
		y = p->y;
		clearPoint(p);
		*/
		Param param;
		param.M = data->M;	param.N = data->N;	param.top = data->top;	param.board = data->boardB;
		param.lastX = data->lastX;	param.lastY = data->lastY;	param.noX = data->noX;	param.noY = data->noY;
		param.getPoint = getPoint;	param.p = NULL;	param.bugOccurred = 0;
		
		DWORD begin = timeGetTime();//计时开始
		
		DWORD threadID;
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)callGetPoint, &param, 0, &threadID);
		WaitForSingleObject(thread, MAX_TIME);
		
		DWORD end = timeGetTime();//计时结束
		timeB += (end - begin);
		
		if(param.p == NULL){//在规定时间内函数没有返回 或 出错
			if(param.bugOccurred == 0){//仅仅是超时
				TerminateThread(thread, 8);//强制关闭
				CloseHandle(thread);//释放资源
				return 8;
			}
			else{//出bug
				CloseHandle(thread);
				return 5;
			}
		}
		
		x = param.p->x;
		y = param.p->y;
		clearPoint(param.p);
		
		CloseHandle(thread);
	}
	catch(...){
		return 5;
	}
	
	if(!isLegal(x, y, data)){
		return 6;
	}
	data->lastX = x;
	data->lastY = y;
	data->boardA[x * data->N + y] = 1;
	data->boardB[x * data->N + y] = 2;
	data->top[y]--;
	//对不可落子点进行处理
	if(x == data->noX + 1 && y == data->noY){
		data->top[y]--;
	}

	if(BWin(x, y, data->M, data->N, data->boardB)){
		return 2;
	}
	if(isTie(data->N, data->top)){
		return 0;
	}
	return -1;
}

void printBoard(Data* data){
	int noPos = data->noX * data->N + data->noY;
	for(int i = 0; i < data->M; i++){
		for(int j = 0; j < data->N; j++){
			int pos = i * data->N + j;
			if(data->boardA[pos] == 0){
				if(pos == noPos){
					cout << "X ";
				}
				else{
					cout << ". ";
				}
			}
			else if(data->boardA[pos] == 2){
				cout << "A ";
			}
			else if(data->boardA[pos] == 1){
				cout << "B ";
			}
		}
		cout << endl;
	}
	return;
}

/*
	input:
		strategyA[] strategyB[] 两个策略文件的文件名
		Afirst: -true : A(前面的文件)先手 -false : B(后面的文件)先手
	reutrns:
		0 - 平局结束 1 - A赢 2 - B赢 3 - A出错 4 - A给出非法落子 5 - B出错 6 - B给出非法落子 7 - A超时 8 - B超时
		-1 - A文件无法载入 -2 - B文件无法载入 -3 - A文件中无法找到需要的接口函数 -4 - B文件中无法找到需要的接口函数
*/
int compete(char strategyA[], char strategyB[], bool Afirst, Data* data){
	CString sA(strategyA);
	CString sB(strategyB);
	HINSTANCE hDLLA;		// Handle to DLL
	GETPOINT getPointA;		// Function pointer
	CLEARPOINT clearPointA;
	HINSTANCE hDLLB;		// Handle to DLL
	GETPOINT getPointB;		// Function pointer
	CLEARPOINT clearPointB;
	hDLLA = LoadLibrary(sA);
	hDLLB = LoadLibrary(sB);
	
	/*
	if(!(hDLLA && hDLLB)){
		cout << "Load DLL file failed" << endl;
		return -1;
	}
	*/
	if(!hDLLA){
		cout << "Load file A failed" << endl;
		return -1;
	}
	if(!hDLLB){
		cout << "Load file B failed" << endl;
		return -2;
	}
	
	getPointA = (GETPOINT)GetProcAddress(hDLLA, "getPoint");
	clearPointA = (CLEARPOINT)GetProcAddress(hDLLA, "clearPoint");
	getPointB = (GETPOINT)GetProcAddress(hDLLB, "getPoint");
	clearPointB = (CLEARPOINT)GetProcAddress(hDLLB, "clearPoint");
	
	/*
	if(getPointA == NULL || getPointB == NULL || clearPointA == NULL || clearPointB == NULL){
		cout << "Can't find entrance of the wanted functions in the DLL file" << endl;
		return -1;
	}
	*/
	if(getPointA == NULL || clearPointA == NULL){
		cout << "Can't find entrance of the wanted functions in the A DLL file" << endl;
		return -3;
	}
	if(getPointB == NULL || clearPointB == NULL){
		cout << "Can't find entrance of the wanted functions in the B DLL file" << endl;
		return -4;
	}
	
	//四个个函数已经拿到手，现在可以开始进行棋盘初始化和进行对抗了
	//Data* data = new Data();
	
	if(Afirst){
		int res = AGo(getPointA, clearPointA, data);
		if(res != -1){
			printBoard(data);
			//delete data;
			return res;
		}
	}
	
	int res = -1;
	bool aGo = false;
	while(true){
		if(aGo){
			res = AGo(getPointA, clearPointA, data);
			aGo = false;
		}
		else{
			res = BGo(getPointB, clearPointB, data);
			aGo = true;
		}
		if(res != -1){
			printBoard(data);
			//delete data;
			return res;
		}
	}
	
	return 0;//程序不应执行到这一步
}