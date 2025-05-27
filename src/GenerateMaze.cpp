#include "GenerateMaze.h"

GenerateMaze::GenerateMaze() {

}

GenerateMaze::~GenerateMaze() {
	
}

void GenerateMaze::setup() {
	mazFile.open("maze.maz", ofFile::WriteOnly, false);
	mazFile.create();
	srand(time(NULL));												//Set new seed for random generation
	MazeRow = (int*)malloc(sizeof(int)*rowSize);
	RowConnect = (int*)malloc(sizeof(int)*(rowSize - 1));
	ColConnect = (int*)malloc(sizeof(int)*rowSize);					//Allocate memory for maze generation

	DrawWall(rowSize);
	InitMaze(rowSize);
	DrawRow(rowSize);
	DrawBottom(rowSize);											//Create and draw first row of maze
	for (int i = 0; i < colSize - 2; i++) {
		CreateLine(rowSize);
		DrawRow(rowSize);
		DrawBottom(rowSize);
	}																//Create and draw maze until last row
	EndMaze(rowSize);
	DrawRow(rowSize);
	DrawWall(rowSize);												//Create and draw last row of maze
	
	mazFile.close();
	free(MazeRow);
	free(RowConnect);
	free(ColConnect);												//free allocated memory
}

void GenerateMaze::update() {

}

void GenerateMaze::draw() {

}

void GenerateMaze::DrawWall(int row) {
	for (int i = 0; i < row; i++) {
		mazFile << "+-";
	}
	mazFile << "+\n";
}

void GenerateMaze::DrawRow(int row) {
	mazFile << "|";
	for (int i = 0; i < row - 1; i++) {
		mazFile << " ";
		if (RowConnect[i])
			mazFile << " ";
		else mazFile << "|";
	}
	mazFile << " |\n";
}

void GenerateMaze::DrawBottom(int row) {
	for (int i = 0; i < row; i++) {
		mazFile << "+";
		if (ColConnect[i])
			mazFile << " ";              //If col is connected, bottom cell has same index
		else {
			mazFile << "-";
			MazeRow[i] = -1;             //Otherwise initialized to -1 (addressed in CreateLine)
		}
	}
	mazFile << "+\n";
}

void GenerateMaze::InitMaze(int row) {
	int i; 
	int bottom_check_flag = 0;
	int currentIndex;

	CellIndex = 0;
	for (i = 0; i < row; i++) {
		MazeRow[i] = CellIndex;
		CellIndex++;
	}
	for (i = 1; i < row; i++) {
		if (RandBool()) {							  //Decide to merge 2 cells
			RowConnect[i - 1] = TRUE;                 //TRUE means no walls between cells
			MazeRow[i] = MazeRow[i - 1];
		}
		else RowConnect[i - 1] = FALSE;
	}
	currentIndex = MazeRow[0];
	for (i = 0; i < row; i++) {
		ColConnect[i] = FALSE;                          //Default is FALSE
		if (MazeRow[i] != currentIndex) {                //If new indexed cell comes, reset flag and currentIndex
			currentIndex = MazeRow[i];
			bottom_check_flag = 0;
		}
		if (!bottom_check_flag) {                        //no holes yet
			if (i == row - 1)                           //if last cell still has no hole, make one
				ColConnect[i] = TRUE;
			else if (MazeRow[i + 1] != currentIndex) {     //look ahead into next cell, if last cell with index
				ColConnect[i] = TRUE;
			}
			else if (RandBoolWithBias()) {				//More likely to be connected to create vertical corridors
				ColConnect[i] = TRUE;
				bottom_check_flag = 1;
			}
		}
		else if (RandBoolWithBias()) {                   //already has hole, then randomly decide
			ColConnect[i] = TRUE;
			bottom_check_flag = 1;
		}
	}
}

void GenerateMaze::CreateLine(int row) {
	int i, j, bottom_check_flag = 0;
	int currentIndex;

	for (i = 0; i < row; i++) {                  //For unconnected new cells, assign new index
		if (MazeRow[i] == -1) {
			MazeRow[i] = CellIndex;
			CellIndex++;
		}
	}
	for (i = 0; i < row - 1; i++) {
		RowConnect[i] = FALSE;
		if (MazeRow[i] != MazeRow[i + 1]) {       //if different index, randomly connect
			if (RandBool()) {
				int tmp = MazeRow[i + 1];
				MazeRow[i + 1] = MazeRow[i];      //Randomly merge two cells
				for (j = 0; j < row; j++) {	      //and update same indexed cells
					if (MazeRow[j] == tmp)
						MazeRow[j] = MazeRow[i];
				}
				RowConnect[i] = TRUE;
			}
		}
	}
	/////
	currentIndex = MazeRow[0];
	for (i = 0; i < row; i++) {
		ColConnect[i] = FALSE;                          //Default is FALSE
		if (MazeRow[i] != currentIndex) {                //If new indexed cell comes, reset flag and currentIndex
			currentIndex = MazeRow[i];
			bottom_check_flag = 0;
		}
		if (!bottom_check_flag) {                        //no holes yet
			if (i == row - 1)                           //if last cell still has no hole, make one
				ColConnect[i] = TRUE;
			else if (MazeRow[i + 1] != currentIndex) {     //look ahead into next cell, if last cell with index
				ColConnect[i] = TRUE;
			}
			else if (RandBoolWithBias()) {				//More likely to be connected to create vertical corridors
				ColConnect[i] = TRUE;
				bottom_check_flag = 1;
			}
		}
		else if (RandBoolWithBias()) {                  //already has hole, then randomly decide
			ColConnect[i] = TRUE;
			bottom_check_flag = 1;
		}
	}
}

void GenerateMaze::EndMaze(int row) {
	int i, j;

	for (i = 0; i < row; i++) {                  //For unconnected new cells, assign new index
		if (MazeRow[i] == -1) {
			MazeRow[i] = CellIndex;
			CellIndex++;
		}
	}
	for (i = 0; i < row - 1; i++) {
		if (MazeRow[i] == MazeRow[i + 1]) {        //if two cells have same index
			RowConnect[i] = FALSE;
		}
		else {
			RowConnect[i] = TRUE;
			int tmp = MazeRow[i + 1];
			MazeRow[i + 1] = MazeRow[i];           //if two different cells merge, update index
			for (j = 0; j < row; j++) {
				if (MazeRow[j] == tmp)
					MazeRow[j] = MazeRow[i];
			}
		}
	}
}

int GenerateMaze::RandBool() {
	return rand() % 2;
}

int GenerateMaze::RandBoolWithBias() {
	if (rand() % RandBias) return TRUE;				//Higher RandBias means more likely to return TRUE
	return FALSE;
}

int GenerateMaze::RandBoolUnlikely(){
	if (rand() % RandBias) return FALSE;			//Higher RandBias means more likely to return FALSE
	return TRUE;
}