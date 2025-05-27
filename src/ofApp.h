#pragma once
#ifndef ofApp_
#define ofApp_

#include "ofMain.h"
#include "GenerateMaze.h"
#include "Mazeplayer.h"
#include "ofxWinMenu.h"


class ofApp : public ofBaseApp{

	public:
		void setup();	//update와 draw 함수가 시행되기 전 필요한 준비를 하는 함수
		void update();	//초당 프레임 횟수만큼 반복하며 현재 상태를 업데이트하는 함수
		void draw();	//초당 프레임 횟수만큼 반복하며 화면에 그리는 함수

		void keyPressed(int key);	//키보드의 특정 키를 눌렀을 때 실행되는 함수
		void keyReleased(int key);	//키보드의 특정 키를 땠을 때 실행되는 함수
		void mouseMoved(int x, int y );		//미사용
		void mouseDragged(int x, int y, int button);	//미사용
		void mousePressed(int x, int y, int button);	//마우스를 클릭할 때 실행되는 함수
		void mouseReleased(int x, int y, int button);	//미사용
		void mouseEntered(int x, int y);	//미사용
		void mouseExited(int x, int y);		//미사용
		void windowResized(int w, int h);	//미사용
		void dragEvent(ofDragInfo dragInfo);//미사용
		void gotMessage(ofMessage msg);		//미사용
		void appMenuFunction(string title, bool bChecked); // 창 메뉴에서의 선택을 관리하는 함수
		bool AutoReadFile();	//GenerateMaze 클래스에서 생성된 미로 파일을 읽는 함수
		void freeMemory();		//미로 및 DFS 수행에서 할당한 메모리를 해제하는 함수
		bool DFS();				//올바른 경로를 찾기 위해 DFS를 수행하는 함수
		void dfsdraw();			//DFS 탐색을 통해 찾은 경로를 그리는 함수
		void InsertToAdjList(int vertex1, int vertex2);	//미로를 표현하기 위해 인접 리스트에 노드를 추가하는 함수
		void DrawMazeCell(graphNode* vertex);	//인접 리스트로 표현된 미로를 화면에 그릴 때 호출되는 함수
		void dfsdrawsetup();	//DFS 탐색을 통해 스택에 저장된 경로를 벡터에 옮기는 함수
		// Example functions
		void doFullScreen(bool bFull);	//전체화면으로 전환하는 함수
		void doTopmost(bool bTop);	//창 화면으로 전환 시 창을 제일 앞으로 보내는 함수

		//Menu related stuff
		ofTrueTypeFont myFont;				//프로그램에서 사용할 폰트
		int mainMenu_draw_flag;				//메인 메뉴 활성화를 위한 flag
		int gameover_flag;					//게임 오버 여부를 알기 위한 flag
		//Quit button (main menu)
		ofRectangle QuitButtonBody;			//Quit 버튼의 좌표를 저장하는 변수
		bool QuitButtonClicked = false;		//Quit 버튼 눌림 여부를 저장
		//Start button (main menu)
		ofRectangle StartButtonBody;		//Start 버튼의 좌표를 저장하는 변수
		bool StartButtonClicked = false;	//Start 버튼 눌림 여부를 저장
		int start_timer_flag;				//타이머 시작 여부를 저장, 1이면 게임 실행 중
		//Next button (mid-game menu)
		ofRectangle NextButtonBody;			//Next 버튼 좌표를 저장하는 변수
		bool NextButtonClicked = false;		//Next 버튼 눌림 여부 저장
		//Home button (mid-game menu)
		ofRectangle HomeButtonBody;			//Home 버튼 좌표를 저장하는 변수
		bool HomeButtonClicked = false;		//Home 버튼 눌림 여부 저장

		//Timer related stuff
		int MaxFrameRate;			//최대 초당 프레임 속도
		float TimerTime;			//타이머의 측정 시간 저장
		float TimerOffset;			//타이머가 줄어든 정도
		ofRectangle TimerBody;		//타이머를 표현하는 직사각형 좌표를 저장하는 변수
		float TimerWidth;			//타이머의 가로 길이 저장
		float TimerHeight;			//타이머의 세로 길이 저장

		//Maze Generation related stuff
		GenerateMaze myMaze;				//미로 생성 객체

		//Maze related stuff
		int HEIGHT;							//미로의 높이
		int WIDTH;							//미로의 너비
		int maze_col;						//미로칸의 열의 인덱스를 가리킨다.
		int maze_row;						//미로칸의 행의 인덱스를 가리킨다.
		int isOpen;							//파일이 열렸는지를 판단하는 변수
		//Maze related variables
		vector<string> MazeLines;			//파일의 내용을 한 줄 한 줄씩 저장
		graphNode** MazeGraph;				//미로를 인접 리스트로 표현할 때 사용할 포인터
		vector<MazeCell> MazeCells;			//미로를 그리기 위해 방 하나의 정보를 저장
		float MazeCellSize;					//미로칸 1개의 크기, AutoReadFile()에서 값이 정해짐
		int MazeStartXPos = 50;				//미로의 왼쪽 위 x좌표
		int MazeStartYPos = 100;			//미로의 왼쪽 위 y 좌표
		int maze_draw_flag;					//미로를 그릴 지 여부를 판단하는 flag
		//Maze path search with DFS
		bool* isVisited;					//DFS 수행 시 노드 방문 여부를 저장하는 배열 포인터
		stack<int> DFSvertices;				//Iterative DFS 수행 시 사용할 스택
		vector<vector<int>> drawDFS;		//DFS로 얻은 경로의 정보를 저장
		//Window menu related stuff
		ofxWinMenu * menu;					//창 메뉴 객체 포인터
		float windowWidth, windowHeight;	//창의 크기를 저장
		HWND hWnd;							//애플리케이션의 창
		HWND hWndForeground;				// 현재 foreground 창
		// Example menu variables
		bool bShowInfo;						//정보 표시 여부를 저장
		bool bFullscreen;					//전체 화면 여부를 저장
		bool bTopmost;						//창이 제일 위에 있는지 여부를 저장
		bool isdfs;							//DFS가 완료되었는지 여부를 저장
		//player related stuff
		player myPlayer;					//플레이어 객체
};

#endif // !ofApp_