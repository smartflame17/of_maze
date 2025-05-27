#pragma once
// Porting week11 exercise C code as a class
//Used vectors instead of malloc arrays
#ifndef GenerateMaze_
#define GenerateMaze_


#include <string>
#include <vector>
#include <time.h>
#include "ofMain.h"

using namespace std;


class GenerateMaze {
	public:
		GenerateMaze();
		~GenerateMaze();


		
		void setup();
		void update();
		void draw();

		//Maze Generation related functions
		void DrawWall(int row);	//벽 한 줄을 생성해 maz 파일에 그리는 함수
		void DrawRow(int row);	//행 한 줄을 생성해 maz 파일에 그리는 함수
		void DrawBottom(int row);	//행 사이를 maz 파일에 그리는 함수
		void InitMaze(int row);	//첫 행과 첫 행과 다음 행 사이를 처리하는 함수
		void CreateLine(int row);	//각 행마다 행과 다음 행의 연결을 처리하는 함수
		void EndMaze(int row);	//마지막 행을 처리하는 함수
		int RandBool();				//랜덤하게true 또는 false를 반환하는 함수
		int RandBoolWithBias();		//true를 더 자주 반환하는 함수
		int RandBoolUnlikely();		//false를 더 자주 반환하는 함수






		//Maze Generation related variables
		int CellIndex;			//현재 칸의 인덱스를 저장하는 변수
		int rowSize;			//미로의 가로 크기
		int colSize;			//미로의 세로 크기
		int* MazeRow;			//미로 한 행을 저장하기 위한 배열 포인터 변수
		int* RowConnect;		//미로의 가로 연결 여부를 저장하기 위한 배열 포인터 변수
		int* ColConnect;		//미로의 세로 연결 여부를 저장하기 위한 배열 포인터 변수
		ofFile mazFile;			//파일 출력을 위한 객체
		int RandBias = 3;		//미로 생성 알고리즘에서 칸 연결 여부의 확률을 조정하는 변수
};
#endif 