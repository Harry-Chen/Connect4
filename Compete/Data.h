#ifndef DATA_H_
#define DATA_H_

#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;

class Data{
public:
	const static int minSize = 9;
	const static int maxSize = 13;
	
	int M;
	int N;
	int* top;
	int* boardA;	//0 - ��λ�� 1 - ���û����� 2 - �е��Ե���
	int* boardB;
	
	int noX;
	int noY;
	
	int lastX;
	int lastY;
	
	Data(){
		srand((int)time(0));
		
		M = (rand() % (maxSize - minSize)) + minSize;
		N = (rand() % (maxSize - minSize)) + minSize;
		//M = 10;
		//N = 11;

		cout << "M = " << M << ", N = " << N << endl;
		
		top = new int[N];
		for(int i = 0; i < N; i++){
			top[i] = M;
		}
		boardA = new int[M * N];
		boardB = new int[M * N];
		for(int i = 0; i < M * N; i++){
			boardA[i] = 0;
			boardB[i] = 0;
		}
		
		//��������������ӵ�
		srand((int)time(0));
		noX = rand() % M;
		noY = rand() % N;
		//noX = 0;
		//noY = 0;


		//���������top���е���
		if(noX == M - 1){
			top[noY] = M - 1;
		}
		
		lastX = -1;
		lastY = -1;
	}
	
	void reset(){
		for(int i = 0; i < N; i++){
			top[i] = M;
		}
		if(noX == M - 1){
			top[noY] = M - 1;
		}
		for(int i = 0; i < M * N; i++){
			boardA[i] = 0;
			boardB[i] = 0;
		}
		lastX = -1;
		lastY = -1;
	}
	
	~Data(){
		delete[] top;
		delete[] boardA;
		delete[] boardB;
	}
};

#endif
