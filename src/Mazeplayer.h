#pragma once
#ifndef mazeplayer_
#define mazeplayer_

#include "ofMain.h"
#include "GenerateMaze.h"

using namespace std;

struct graphNode {
	int vertex;
	graphNode* link;
}; //struct used to make adj list

struct MazeCell {  
	bool isUpConnected;
	bool isDownConnected;
	bool isLeftConnected;
	bool isRightConnected;
}; //struct used while drawing maze
class player {
	public:
		player();
		~player();

		void setup();
		void update();
		void draw();

		void MovePlayer(int key, vector<MazeCell>& cell); //플레이어의 위치를 이동시킨다
		void GetBoundary(GenerateMaze maze); //미로의 행과 열 크기를 변수에 따로 저장한다

		int PlayerIndex;				//플레이어가 있는 현재 칸의 인덱스를 저장
		int PlayerXPos;					//플레이어 인덱스의 x 좌표
		int PlayerYPos;					//플레이어 인덱스의 y 좌표

		int PlayerXBoundary;			//플레이어가 이동할 수 있는 최대 x좌표
		int PlayerYBoundary;			//플레이어가 이동할 수 있는 최대 y 좌표

		bool PlayerWon = false;			//플레이어가 한 레벨을 이겼는지 저장하는 변수	
		int CurrentLevel = 1;		    //플레이어의 현재 레벨
		int MaxLevel = 5;			    //미로의 크기가 커지는 최대 레벨
		int BestLevel;					//플레이어가 도달한 최고로 높은 레벨
};

#endif // !mazeplayer_