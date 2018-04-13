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

//����getPointA �� clearPointA
//returns : 0 - ƽ�ֽ��� 1 - AӮ 2 - BӮ 3 - A���� 4 - A�����Ƿ����� 5 - B���� 6 - B�����Ƿ����� 7 - A��ʱ 8 - B��ʱ -1 - ��Ϸδ����
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
		
		DWORD begin = timeGetTime();//��ʱ��ʼ
		
		DWORD threadID;
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)callGetPoint, &param, 0, &threadID);
		WaitForSingleObject(thread, MAX_TIME);
		
		DWORD end = timeGetTime();//��ʱ����
		timeA += (end - begin);
		
		if(param.p == NULL){//�ڹ涨ʱ���ں���û�з��� �� ����
			if(param.bugOccurred == 0){//�����ǳ�ʱ
				TerminateThread(thread, 7);//ǿ�ƹر�
				CloseHandle(thread);//�ͷ���Դ
				return 7;
			}
			else{//��bug
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
	//�Բ������ӵ���д���
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

//����getPointB �� clearPointB
//returns : 0 - ƽ�ֽ��� 1 - AӮ 2 - BӮ 3 - A���� 4 - A�����Ƿ����� 5 - B���� 6 - B�����Ƿ����� 7 - A��ʱ 8 - B��ʱ -1 - ��Ϸδ����
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
		
		DWORD begin = timeGetTime();//��ʱ��ʼ
		
		DWORD threadID;
		HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)callGetPoint, &param, 0, &threadID);
		WaitForSingleObject(thread, MAX_TIME);
		
		DWORD end = timeGetTime();//��ʱ����
		timeB += (end - begin);
		
		if(param.p == NULL){//�ڹ涨ʱ���ں���û�з��� �� ����
			if(param.bugOccurred == 0){//�����ǳ�ʱ
				TerminateThread(thread, 8);//ǿ�ƹر�
				CloseHandle(thread);//�ͷ���Դ
				return 8;
			}
			else{//��bug
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
	//�Բ������ӵ���д���
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
		strategyA[] strategyB[] ���������ļ����ļ���
		Afirst: -true : A(ǰ����ļ�)���� -false : B(������ļ�)����
	reutrns:
		0 - ƽ�ֽ��� 1 - AӮ 2 - BӮ 3 - A���� 4 - A�����Ƿ����� 5 - B���� 6 - B�����Ƿ����� 7 - A��ʱ 8 - B��ʱ
		-1 - A�ļ��޷����� -2 - B�ļ��޷����� -3 - A�ļ����޷��ҵ���Ҫ�Ľӿں��� -4 - B�ļ����޷��ҵ���Ҫ�Ľӿں���
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
	
	//�ĸ��������Ѿ��õ��֣����ڿ��Կ�ʼ�������̳�ʼ���ͽ��жԿ���
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
	
	return 0;//����Ӧִ�е���һ��
}