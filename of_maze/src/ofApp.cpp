/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	//MazeCellSize = 100;					//Set size of maze cell
	maze_draw_flag = 0;					//Set to 1 when adj list is made		
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	//testing
	hPopup = menu->AddPopupMenu(hMenu, "Testmenu");
	menu->AddPopupItem(hPopup, "testpopup", false, false);

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		freeMemory();		//clear previous maze info
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
			dfsdrawsetup();
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co\n");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (maze_draw_flag) {
		for (i = 0; i < HEIGHT*WIDTH; i++) {
			DrawMazeCell(MazeGraph[i]);
		}
	}

	if (isdfs){
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	//week03 hw, do similar for bfs

	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			freeMemory();
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			
			ofBuffer buffer(file);
			for (auto line : buffer.getLines())
				MazeLines.push_back(line);

			int i, j;
			WIDTH = 0;
			HEIGHT = MazeLines.size() / 2;
			for (i = 0; i < MazeLines[0].size(); i++)
				if (MazeLines[0][i] == '-') WIDTH++;
			
			MazeGraph = (graphNode**)malloc(sizeof(graphNode*)*WIDTH * HEIGHT);
			for (i = 0; i < WIDTH * HEIGHT; i++) {
				MazeGraph[i] = (graphNode*)malloc(sizeof(graphNode));
				MazeGraph[i]->vertex = i;
				MazeGraph[i]->link = nullptr;
			}

			//part where we read line by line of maz file
			for (j = 0; j < HEIGHT - 1; j++) {
				for (i = 0; i < WIDTH - 1; i++) {
					if (MazeLines[2 * j + 1][2 * i + 2] == ' ') {	//if maze is horizontally connected
						InsertToAdjList(i + j * WIDTH, i + j * WIDTH + 1);
					}
					if (MazeLines[2 * j + 2][2 * i + 1] == ' ') {	//if maze is vertically connected
						InsertToAdjList(i + j * WIDTH, i + (j+1) * WIDTH);
					}
				}
				//exception handling for last cell in row (only have to check for vertical connection)
				if (MazeLines[2 * j + 2][2 * i + 1] == ' ') {	
					InsertToAdjList(i + j * WIDTH, i + (j + 1) * WIDTH);
				}
			}
			//exception handling for last row (only have to check for horizontal connection)
			for (i = 0; i < WIDTH - 1; i++) {
				if (MazeLines[2 * (HEIGHT - 1) + 1][2 * i + 2] == ' ') {
					InsertToAdjList(i + (HEIGHT-1) * WIDTH, i + (HEIGHT - 1) * WIDTH + 1);
				}
			}
			MazeCellSize = (float)(300 / WIDTH);
			maze_draw_flag = 1;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::InsertToAdjList(int vertex1, int vertex2) {
	graphNode* tmp;
	graphNode* cur;

	cur = MazeGraph[vertex1];
	while (cur->link)
		cur = cur->link;								//cur now points to last added element on list node
	tmp = (graphNode*)malloc(sizeof(graphNode));
	tmp->vertex = vertex2;
	tmp->link = nullptr;
	cur->link = tmp;									// edge vertex1 -> vertex2 added

	cur = MazeGraph[vertex2];
	while (cur->link)
		cur = cur->link;
	tmp = (graphNode*)malloc(sizeof(graphNode));
	tmp->vertex = vertex1;
	tmp->link = nullptr;
	cur->link = tmp;									// edge vertex2 -> vertex1 added
}

void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	//also initialize some variables
	int i;
	graphNode* tmp;
	graphNode* cur;

	for (i = 0; i < WIDTH * HEIGHT; i++) {
		cur = MazeGraph[i];
		tmp = cur->link;
		free(cur);
		while (tmp) {
			cur = tmp;
			tmp = cur->link;
			free(cur);
		}
	}
	free(MazeGraph);
	vector<string>().swap(MazeLines);
	free(isVisited);
	vector<vector<int>>().swap(drawDFS);
}

void ofApp::DrawMazeCell(graphNode* vertex) {
	MazeCell temp;
	temp.isUpConnected = false;
	temp.isDownConnected = false;
	temp.isLeftConnected = false;
	temp.isRightConnected = false;
	graphNode* edge = vertex->link;
	int CurrentVertex = vertex->vertex;
	maze_col = CurrentVertex / WIDTH;				//get index of current maze cell (for easier drawing)
	maze_row = CurrentVertex % WIDTH;				//maze_row = x coord, maze_col = y coord

	while (edge != nullptr) {						//find all connected elements
		if (edge->vertex == CurrentVertex + 1)		//connected to left cell
			temp.isRightConnected = true;
		if (edge->vertex == CurrentVertex - 1)		//connected to right cell
			temp.isLeftConnected = true;
		if (edge->vertex == CurrentVertex + WIDTH)	//connected to bottom cell
			temp.isDownConnected = true;
		if (edge->vertex == CurrentVertex - WIDTH)  //connected to top cell
			temp.isUpConnected = true;
		edge = edge->link;
	}
	ofSetLineWidth(10);
	ofSetColor(0);
	if (!temp.isUpConnected)
		ofDrawLine(20 + maze_row * MazeCellSize, 20 + maze_col * MazeCellSize, 20 + (maze_row+1) * MazeCellSize, 20 + maze_col * MazeCellSize);
	if (!temp.isLeftConnected)
		ofDrawLine(20 + maze_row * MazeCellSize, 20 + maze_col * MazeCellSize, 20 + maze_row * MazeCellSize, 20 + (maze_col+1) * MazeCellSize);
	if (!temp.isRightConnected)
		ofDrawLine(20 + (maze_row+1) * MazeCellSize, 20 + maze_col * MazeCellSize, 20 + (maze_row+1) * MazeCellSize, 20 + (maze_col+1) * MazeCellSize);
	if (!temp.isDownConnected)
		ofDrawLine(20 + maze_row * MazeCellSize, 20 + (maze_col+1) * MazeCellSize, 20 + (maze_row+1) * MazeCellSize, 20 + (maze_col+1) * MazeCellSize);
}

bool ofApp::DFS()//DFS탐색을 하는 함수 
{
	int i;
	int target = HEIGHT*WIDTH - 1;
	int visit_flag = 0;							//if we have visited at least one adj node, this is set to 1
	graphNode* temp;

	isVisited = (bool*)malloc(sizeof(bool)*HEIGHT*WIDTH); //allocated memory for size of vertices		
	for (i = 0; i < HEIGHT*WIDTH; i++)
		isVisited[i] = false;

	while (!DFSvertices.empty())
		DFSvertices.pop();						//empty stack for new search
	DFSvertices.push(0);
	isVisited[0] = true;						//search starts at index 0
	while (!DFSvertices.empty()) {
		if (DFSvertices.top() == target) {
			isdfs = true;
			return true;
		}
		temp = MazeGraph[DFSvertices.top()]->link;
		while (temp != nullptr) {				//search through all adjacent, unvisited nodes
			if (!isVisited[temp->vertex]) {
				isVisited[temp->vertex] = true;
				DFSvertices.push(temp->vertex);
				visit_flag = 1;
				break;
			}
			temp = temp->link;
		}
		if (!visit_flag) {						//flag being 0 means no more connected nodes to search through
			DFSvertices.pop();					//NEEDS FIXING -> pop must be done later (2 is inside stack) (maze0.maz)
		}
		else {
			visit_flag = 0;						//reset flag for next iteration
		}
	}
	isdfs = false;
	return false;
}

void ofApp::dfsdrawsetup() {
	int row_prev, col_prev;
	int row_next, col_next;
	int index;
	
	index = DFSvertices.top();
	row_prev = index % WIDTH;
	col_prev = index / WIDTH;
	DFSvertices.pop();
	while (!DFSvertices.empty()) {
		vector<int> tempv;

		index = DFSvertices.top();
		row_next = index % WIDTH;
		col_next = index / WIDTH;
		tempv.push_back(20 + (row_prev + 0.5) * MazeCellSize);		//push x coord of node1
		tempv.push_back(20 + (col_prev + 0.5) * MazeCellSize);		//push y coord of node1
		tempv.push_back(20 + (row_next + 0.5) * MazeCellSize);		//push x coord of node2
		tempv.push_back(20 + (col_next + 0.5) * MazeCellSize);		//push y coord of node2
		drawDFS.push_back(tempv);
		row_prev = row_next;
		col_prev = col_next;
		DFSvertices.pop();
	}
}

void ofApp::dfsdraw(){
	for (int i = 0; i < drawDFS.size(); i++)
		ofDrawLine(drawDFS[i][0], drawDFS[i][1], drawDFS[i][2], drawDFS[i][3]);
}

