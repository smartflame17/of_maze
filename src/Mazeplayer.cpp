#include "Mazeplayer.h"

player::player() {
	PlayerIndex = 0;		//Player always starts at index 0 (0, 0)
	PlayerXPos = 0;
	PlayerYPos = 0;
	PlayerWon = false;
	BestLevel = 0;			//best level is set every new program run
}

player::~player() {

}

void player::setup() {

}

void player::update() {

}

void player::draw() {
	
}

void player::MovePlayer(int key, vector<MazeCell>& cell) {				
	if (key == OF_KEY_LEFT) {
		if (PlayerXPos - 1 >= 0) {							//outer wall check
			if (cell[PlayerIndex].isLeftConnected)			//cell connection check
				PlayerXPos--;
		}
	}
	if (key == OF_KEY_RIGHT) {
		if (PlayerXPos + 1 < PlayerXBoundary) {
			if (cell[PlayerIndex].isRightConnected)
				PlayerXPos++;
		}
	}
	if (key == OF_KEY_UP) {
		if (PlayerYPos - 1 >= 0) {
			if (cell[PlayerIndex].isUpConnected)
				PlayerYPos--;
		}
	}
	if (key == OF_KEY_DOWN) {
		if (PlayerYPos + 1 < PlayerYBoundary) {
			if (cell[PlayerIndex].isDownConnected)
				PlayerYPos++;
		}
	}
	//change x or y coords according to input
	PlayerIndex = PlayerXPos + PlayerYPos * PlayerXBoundary;
	//update index according to coords
	cout << "index: " << PlayerIndex << ", (" << PlayerXPos << ", " << PlayerYPos <<  ")" << endl;
	//print player position to stdout
}

void player::GetBoundary(GenerateMaze maze) {
	PlayerXBoundary = maze.rowSize;
	PlayerYBoundary = maze.colSize;
}

