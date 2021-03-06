#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FALSE 0
#define TRUE 1

int CellIndex;                                              //Index for cell element, increases if disconnected cell appears
int* MazeRow;                                               //Pointer for malloc array of one row (size : row)
int* RowConnect;                                            //Pointer for malloc array of connection in a row (size : row-1)
int* ColConnect;                                            //Pointer for malloc array of connection with bottom (size : row)
void DrawWall(int row, FILE** fp);                          //Draws horizontal wall of maze(called at start and end)
void DrawRow(int row, FILE** fp);                           //Draws horizontal wall of each line
void DrawBottom(int row, FILE** fp);                        //Draws vertical walls after DrawRow
void InitMaze(int row);                                     //Deals with first row of maze
void CreateLine(int row);                                   //Creates arrays for each line
void EndMaze(int row);                                      //Deals with last row of maze
int RandBool();
int main()
{
	FILE* fp;
	int row, col, i;

	fp = fopen("maze.maz", "w");
	printf("Input row size of maze : ");
	scanf("%d", &row);
	printf("Input column size of maze : ");
	scanf("%d", &col);

	MazeRow = (int*)malloc(sizeof(int)*row);
	RowConnect = (int*)malloc(sizeof(int)*(row - 1));
	ColConnect = (int*)malloc(sizeof(int)*row);

	srand(time(NULL));
	////////////////////////////////////////////////////////// Actual creation & drawing of maze part
	DrawWall(row, &fp);
	InitMaze(row);
	DrawRow(row, &fp);
	DrawBottom(row, &fp);
	for (i = 0; i < col - 2; i++) {
		CreateLine(row);
		DrawRow(row, &fp);
		DrawBottom(row, &fp);
	}
	EndMaze(row);
	DrawRow(row, &fp);
	DrawWall(row, &fp);
	//////////////////////////////////////////////////////////
	fclose(fp);
	free(MazeRow);
	free(RowConnect);
	free(ColConnect);
}
void DrawWall(int row, FILE** fp) {
	for (int i = 0; i < row; i++)
		fprintf(*fp, "+-");
	fprintf(*fp, "+\n");
}

void DrawRow(int row, FILE** fp) {
	fprintf(*fp, "|");
	for (int i = 0; i < row - 1; i++) {
		//fprintf(*fp, "%d", MazeRow[i]);               //debugging
		fprintf(*fp, " ");                              //Print one cell of maze
		if (RowConnect[i])
			fprintf(*fp, " ");                          //If cell is connected
		else fprintf(*fp, "|");
	}
	//fprintf(*fp, "%d|\n", MazeRow[row - 1]);            //debugging
	fprintf(*fp, " |\n");
}

void DrawBottom(int row, FILE** fp) {
	for (int i = 0; i < row; i++) {
		fprintf(*fp, "+");
		if (ColConnect[i])
			fprintf(*fp, " ");              //If col is connected, bottom cell has same index
		else {
			fprintf(*fp, "-");
			MazeRow[i] = -1;                //Otherwise initialized to -1 (addressed in CreateLine)
		}
	}
	fprintf(*fp, "+\n");
}

void InitMaze(int row) {
	int i, bottom_check_flag = 0;                   //bottom_check_flag makes sure at least 1 bottom hole exists for each index
	int currentIndex;                               //tracks current index of cell for bottom hole check

	CellIndex = 0;
	for (i = 0; i < row; i++) {
		MazeRow[i] = CellIndex;
		CellIndex++;
	}
	for (i = 1; i < row; i++) {
		if (RandBool()) {                            //Decide to merge 2 cells
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
			else if (RandBool()) {
				ColConnect[i] = TRUE;
				bottom_check_flag = 1;
			}
		}
		else if (RandBool()) {                           //already has hole, then randomly decide
			ColConnect[i] = TRUE;
			bottom_check_flag = 1;
		}
	}
}

void CreateLine(int row) {
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
			else if (RandBool()) {
				ColConnect[i] = TRUE;
				bottom_check_flag = 1;
			}
		}
		else if (RandBool()) {                           //already has hole, then randomly decide
			ColConnect[i] = TRUE;
			bottom_check_flag = 1;
		}
	}
}

void EndMaze(int row) {
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

int RandBool() {
	return rand() % 2;
}