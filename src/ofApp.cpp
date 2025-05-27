#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("The Maze Game"); // Set the app name on the title bar
	MaxFrameRate = 30;
	ofSetFrameRate(MaxFrameRate);
	myFont.loadFont("verdana.ttf", 24, true, true);
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;						//don't draw dfs
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	//Initialize timer related stuff
	TimerTime = 20;				//level 1 gets 20 seconds of time
	TimerOffset = 0;
	TimerWidth = 400;
	TimerHeight = 50;
	TimerBody.set(ofGetWidth() - TimerWidth * 1.2, TimerHeight * 0.2, TimerWidth, TimerHeight);	//timer position

	//Initialize start button & quit button related stuff
	start_timer_flag = 0;		//not in-game yet
	gameover_flag = 0;			
	mainMenu_draw_flag = 1;		//draw main menu
	ofBackground(255);

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
	// View popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show Answer", false, false); // Checked
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "How To Play", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

}

void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// Window menu
	//
	if (title == "Show Answer") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen && start_timer_flag && !isdfs)			//added !isdfs to avoid same calculation again
		{
			DFS();
			dfsdrawsetup();
			bShowInfo = bChecked;
		}
		else
			cout << "you must start game first" << endl;

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "How To Play") {
		ofSystemAlertDialog("Use Arrow Keys to move player.\nYou must reach the bottom right corner\nbefore time runs out!\n");
	}

} // end appMenuFunction

//--------------------------------------------------------------
void ofApp::update(){
	if (start_timer_flag) {
		TimerOffset += TimerWidth / (MaxFrameRate * TimerTime);
		if (TimerWidth - TimerOffset <= 0) {						//game over (time left reached 0)
			//timer gets resetted on mousePressed gameover_flag part
			start_timer_flag = 0;									//end timer
			gameover_flag = 1;
			if (myPlayer.CurrentLevel > myPlayer.BestLevel) {
				myPlayer.BestLevel = myPlayer.CurrentLevel - 1;		//Best level is current level - 1 since player failed
			}
			TimerTime = 30;											//By default (level 1) timer is set to 30s
			DFS();
			dfsdrawsetup();											//show answer
		}
		else if (myPlayer.PlayerIndex == HEIGHT*WIDTH-1) {			//game won (player reached end)
			TimerOffset = 0;
			start_timer_flag = 0;
			myPlayer.PlayerWon = true;								//for drawing mid-game menu
			myPlayer.PlayerIndex = 0;
			myPlayer.PlayerXPos = 0;
			myPlayer.PlayerYPos = 0;								//reset player position for next maze
		}
		TimerBody.set(ofGetWidth() - TimerWidth * 1.2, TimerHeight * 0.2, TimerWidth - TimerOffset, TimerHeight);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	int i;
	char str[256];
	//while timer is active (in-game)
	//pressed answer button, get answer with dfs
	
	if (start_timer_flag || gameover_flag) {	//gameover_flag is added as OR statement so that players can still see the maze after gameover
		//draws timer
		ofFill();
		ofSetColor(ofColor::green);
		if (TimerWidth - TimerOffset <= TimerWidth * 0.66 && TimerWidth - TimerOffset > TimerWidth*0.33)
			ofSetColor(ofColor::yellow);
		else if (TimerWidth - TimerOffset <= TimerWidth * 0.33)
			ofSetColor(ofColor::red);
		ofDrawRectangle(TimerBody);							//Timer changes color over time

		ofSetColor(ofColor::black);
		sprintf(str, "Level %d", myPlayer.CurrentLevel);
		myFont.drawString(str, ofGetWidth() - 300, 300);			//print current level player is in
		sprintf(str, "Best Level : %d", myPlayer.BestLevel);
		myFont.drawString(str, ofGetWidth() - 300, 400);			//print best level player has achieved

		if (isdfs){											//if showing answer
			ofSetColor(200);
			ofSetLineWidth(5);
			dfsdraw();
		}

		if (maze_draw_flag) {
			//draws maze
			for (i = 0; i < HEIGHT*WIDTH; i++) {
				DrawMazeCell(MazeGraph[i]);
			}
			//draws player
			ofSetColor(ofColor::red);
			ofDrawCircle(MazeStartXPos+(float)(myPlayer.PlayerXPos+0.5)*MazeCellSize, MazeStartYPos+ (float)(myPlayer.PlayerYPos + 0.5)*MazeCellSize, 5);
		}

		if (gameover_flag) {								//draws home button for game over situation
			ofSetColor(ofColor::black);
			ofSetLineWidth(5);
			ofNoFill();
			HomeButtonBody.set(ofGetWidth() - 300, ofGetHeight() - 120, 200, 100);
			ofDrawRectangle(HomeButtonBody);
			myFont.drawString("Home", ofGetWidth() - 250, ofGetHeight() - 60);
			myFont.drawString("Game Over", ofGetWidth() - 300, 60);
		}
	}
	
	//draw main menu (start & quit button)
	if (mainMenu_draw_flag) {
		ofSetColor(ofColor::black);
		ofSetLineWidth(5);
		ofNoFill();
		StartButtonBody.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 50, 200, 100);
		QuitButtonBody.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 60, 200, 100);
		ofDrawRectangle(StartButtonBody);
		myFont.drawString("Start", ofGetWidth() / 2 - 40, ofGetHeight() / 2 + 10);
		ofDrawRectangle(QuitButtonBody);
		myFont.drawString("Quit", ofGetWidth() / 2 - 40, ofGetHeight() / 2 + 120);
		myFont.drawString("The Maze Game", ofGetWidth() / 2 - 120, 200);
		myFont.drawString("Final Project\nBy Jason Kim", ofGetWidth() / 2 - 100, 250);
	}
	
	//draw mid-game menu if player passed one level
	if (myPlayer.PlayerWon) {

		ofSetColor(ofColor::black);
		ofSetLineWidth(5);
		ofNoFill();
		NextButtonBody.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 50, 200, 100);
		HomeButtonBody.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 60, 200, 100);
		ofDrawRectangle(NextButtonBody);
		myFont.drawString("Next", ofGetWidth() / 2 - 40, ofGetHeight() / 2 + 10);
		ofDrawRectangle(HomeButtonBody);
		myFont.drawString("Home", ofGetWidth() / 2 - 50, ofGetHeight() / 2 + 120);
		sprintf(str, "Your current level : %d", myPlayer.CurrentLevel);
		myFont.drawString(str, ofGetWidth() / 2 - 150, 200);				//print current level in mid-game menu
		sprintf(str, "Your highest level : %d", myPlayer.BestLevel);
		myFont.drawString(str, ofGetWidth() / 2 - 150, 250);
	}

	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(ofColor::black);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}
	//ofSetColor(ofColor::black);
	//ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 10, 15);
}

void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
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
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen

void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
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
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (start_timer_flag) {
		if (key == OF_KEY_LEFT) {
			myPlayer.MovePlayer(OF_KEY_LEFT, MazeCells);
		}
		if (key == OF_KEY_RIGHT) {
			myPlayer.MovePlayer(OF_KEY_RIGHT, MazeCells);
		}
		if (key == OF_KEY_UP) {
			myPlayer.MovePlayer(OF_KEY_UP, MazeCells);
		}
		if (key == OF_KEY_DOWN) {
			myPlayer.MovePlayer(OF_KEY_DOWN, MazeCells);
		}
	}	//Move player while timer is active
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			//freeMemory();
			//ofExit();
		}
	}
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mainMenu_draw_flag) {
		StartButtonClicked = StartButtonBody.inside(x, y);
		if (StartButtonClicked) {
			start_timer_flag = 1;
			mainMenu_draw_flag = 0;
			myMaze.rowSize = 16;
			myMaze.colSize = 16;							//level 1 maze is 16 x 16
			myMaze.setup();									//create maze
			myPlayer.GetBoundary(myMaze);					//set player boundary
			AutoReadFile();									//reads created maze
		}
		QuitButtonClicked = QuitButtonBody.inside(x, y);
		if (QuitButtonClicked) {
			ofExit();
		}
			
	}
	if (myPlayer.PlayerWon) {										//mid-game menu (player won previous level)
		NextButtonClicked = NextButtonBody.inside(x, y);
		if (NextButtonClicked) {									//Next button clicked
			start_timer_flag = 1;
			myPlayer.PlayerWon = false;
			if (myPlayer.CurrentLevel < myPlayer.MaxLevel) {		//level is smaller than MaxLevel
				myMaze.rowSize += 2;
				myMaze.colSize += 2;								//increase next maze size by 2
			}
			//maze does not get any bigger than MaxLevel
			myPlayer.CurrentLevel++;								//increase level
			if (myPlayer.CurrentLevel > myPlayer.MaxLevel) {
				TimerTime = 30 + myPlayer.MaxLevel - myPlayer.CurrentLevel;	//past max level, time gets shorter
				myMaze.RandBias = 4;	//RandBias is increased, meaning the maze has more horizontal halls, making it harder
			}														
			else TimerTime = 30 + myPlayer.CurrentLevel;			//additional time for next level
			freeMemory();											//clear previous maze
			myMaze.setup();											//create next maze
			myPlayer.GetBoundary(myMaze);							//set player boundary
			AutoReadFile();											//reads created maze
		}
		HomeButtonClicked = HomeButtonBody.inside(x, y);
		if (HomeButtonClicked) {									//reset game elements and return to main menu
			freeMemory();
			TimerOffset = 0;
			start_timer_flag = 0;
			mainMenu_draw_flag = 1;
			myPlayer.PlayerWon = false;
			myPlayer.PlayerIndex = 0;
			myPlayer.PlayerXPos = 0;
			myPlayer.PlayerYPos = 0;
			if (myPlayer.CurrentLevel > myPlayer.BestLevel) {
				myPlayer.BestLevel = myPlayer.CurrentLevel;	//save highest level on BestLevel
			}
			myPlayer.CurrentLevel = 1;
			TimerTime = 30;
		}
	}
	if (gameover_flag) {									//player lost and only has option to return to main menu
		HomeButtonClicked = HomeButtonBody.inside(x, y);
		if (HomeButtonClicked) {							//reset game elements and return to main menu
			freeMemory();									//if called by gameover_flag, free memory
			gameover_flag = 0;
			TimerOffset = 0;								//Reset timer here (so that timer doesn't show up after gameover)												
			start_timer_flag = 0;
			mainMenu_draw_flag = 1;							//back to main menu
			myPlayer.PlayerWon = false;
			myPlayer.PlayerIndex = 0;
			myPlayer.PlayerXPos = 0;
			myPlayer.PlayerYPos = 0;						//reset player position
			if (myPlayer.CurrentLevel > myPlayer.BestLevel) {
				myPlayer.BestLevel = myPlayer.CurrentLevel;	//save highest level on BestLevel
			}
			myPlayer.CurrentLevel = 1;						//reset player level for new game
			TimerTime = 30;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

bool ofApp::AutoReadFile() {
	ofFile file;
	file.open(ofToDataPath("maze.maz"), ofFile::ReadOnly, false);
	ofBuffer buffer(file);
	isOpen = 1;
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
				InsertToAdjList(i + j * WIDTH, i + (j + 1) * WIDTH);
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
			InsertToAdjList(i + (HEIGHT - 1) * WIDTH, i + (HEIGHT - 1) * WIDTH + 1);
		}
	}
	MazeCellSize = (float)(600 / WIDTH);
	maze_draw_flag = 1;

	return true;
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
	vector<MazeCell>().swap(MazeCells);					// clear maze information
	vector<vector<int>>().swap(drawDFS);				// clear dfs information
	isdfs = 0;
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
		ofDrawLine(MazeStartXPos + maze_row * MazeCellSize, MazeStartYPos + maze_col * MazeCellSize, MazeStartXPos + (maze_row + 1) * MazeCellSize, MazeStartYPos + maze_col * MazeCellSize);
	if (!temp.isLeftConnected)
		ofDrawLine(MazeStartXPos + maze_row * MazeCellSize, MazeStartYPos + maze_col * MazeCellSize, MazeStartXPos + maze_row * MazeCellSize, MazeStartYPos + (maze_col + 1) * MazeCellSize);
	if (!temp.isRightConnected)
		ofDrawLine(MazeStartXPos + (maze_row + 1) * MazeCellSize, MazeStartYPos + maze_col * MazeCellSize, MazeStartXPos + (maze_row + 1) * MazeCellSize, MazeStartYPos + (maze_col + 1) * MazeCellSize);
	if (!temp.isDownConnected)
		ofDrawLine(MazeStartXPos + maze_row * MazeCellSize, MazeStartYPos + (maze_col + 1) * MazeCellSize, MazeStartXPos + (maze_row + 1) * MazeCellSize, MazeStartYPos + (maze_col + 1) * MazeCellSize);
	//draw walls according to each side's connection status
	if (CurrentVertex == HEIGHT * WIDTH - 1) {
		ofSetColor(ofColor::lightGreen);
		ofDrawRectangle(MazeStartXPos + (maze_row + 0.1) * MazeCellSize, MazeStartYPos + (maze_col + 0.1) * MazeCellSize, MazeCellSize * 0.8, MazeCellSize * 0.8);
	}
	//color last cell (destination) to green
	MazeCells.push_back(temp);
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	int i;
	int target = HEIGHT * WIDTH - 1;
	int visit_flag = 0;							//if we have visited at least one adj node, this is set to 1
	graphNode* temp;

	//isVisited = (bool*)malloc(sizeof(bool)*HEIGHT*WIDTH); 	
	isVisited = new bool[HEIGHT*WIDTH];			//allocated memory for size of vertices
	for (i = 0; i < HEIGHT*WIDTH; i++)
		isVisited[i] = false;

	while (!DFSvertices.empty())
		DFSvertices.pop();						//empty stack for new search
	DFSvertices.push(0);
	isVisited[0] = true;						//search starts at index 0
	while (!DFSvertices.empty()) {
		if (DFSvertices.top() == target) {		//target found, path to target inside stack
			isdfs = true;
			delete[] isVisited;
			return true;
		}
		temp = MazeGraph[DFSvertices.top()]->link;
		while (temp != nullptr) {				//search through all adjacent, unvisited nodes
			if (!isVisited[temp->vertex]) {
				isVisited[temp->vertex] = true; //update isVisited
				DFSvertices.push(temp->vertex); //push to stack
				visit_flag = 1;					//flag set to not pop this node yet
				break;							//unvisited adj node found, so break loop
			}
			temp = temp->link;
		}
		if (!visit_flag) {						//flag being 0 means no more connected nodes to search through
			DFSvertices.pop();					
		}
		else {
			visit_flag = 0;						//reset flag for next iteration
		}
	}
	isdfs = false;						//just in case something goes wrong
	return false;
}
void ofApp::dfsdraw()
{
	for (int i = 0; i < drawDFS.size(); i++)
		ofDrawLine(drawDFS[i][0], drawDFS[i][1], drawDFS[i][2], drawDFS[i][3]); //drawDFS contains (x1, y1, x2, y2)
}

void ofApp::dfsdrawsetup() {
	int row_prev, col_prev;
	int row_next, col_next;
	int index;

	index = DFSvertices.top();
	row_prev = index % WIDTH;
	col_prev = index / WIDTH;
	DFSvertices.pop();					//pop from stack to read next vertex info
	while (!DFSvertices.empty()) {
		vector<int> tempv;

		index = DFSvertices.top();
		row_next = index % WIDTH;
		col_next = index / WIDTH;
		tempv.push_back(MazeStartXPos + (row_prev + 0.5) * MazeCellSize);		//push x coord of node1
		tempv.push_back(MazeStartYPos + (col_prev + 0.5) * MazeCellSize);		//push y coord of node1
		tempv.push_back(MazeStartXPos + (row_next + 0.5) * MazeCellSize);		//push x coord of node2
		tempv.push_back(MazeStartYPos + (col_next + 0.5) * MazeCellSize);		//push y coord of node2
		drawDFS.push_back(tempv);
		row_prev = row_next;
		col_prev = col_next;
		DFSvertices.pop();
	}
}