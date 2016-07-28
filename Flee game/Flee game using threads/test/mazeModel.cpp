#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stack>
#include "mazeModel.h"
#include "FleeCommons.h"
#include <thread>         // std::thread


// general variables
std::stack< Coord > pathStack;
Cell **grid;
GridRelations *gridRelations;
Coord *GemsLocations, *GhostInitialLocations, StartCell;
Ghost *Ghosts;


Coord randomCell(int noOfRows, int noOfColumns) {
	Coord coord;
	coord.row = (int)(((float)rand() / (RAND_MAX)) * noOfRows);
	coord.column = (int)(((float)rand() / (RAND_MAX)) * noOfColumns);
	return coord;

}

bool isIsolated(Cell* current) { //checking neighbours
	return !(current->hasLeft || current->hasLower || current->hasRight || current->hasUpper);
}


void updateRelations(int curCOL, int curROW, int dirc) {
	if (dirc == LEFT)
		gridRelations->horizontal[curROW][curCOL - 1] = true;

	else if (dirc == DOWN)
		gridRelations->vertical[curCOL][curROW] = true;

	else if (dirc == RIGHT)
		gridRelations->horizontal[curROW][curCOL] = true;

	else if (dirc == UP)
		gridRelations->vertical[curCOL][curROW - 1] = true;
}


void traverse(Cell* current) {

	int curROW = current->coord.row, rowIndex = curROW, curCOL = current->coord.column, columnIndex = curCOL;

	pathStack.push(current->coord);

	if (current->isNotRightmost(noOfColumns))
		grid[rowIndex][columnIndex + 1].hasLeft = false;
	if (current->isNotDownmost(noOfRows))
		grid[rowIndex + 1][columnIndex].hasUpper = false;
	if (current->isNotUpmost())
		grid[rowIndex - 1][columnIndex].hasLower = false;
	if (current->isNotLeftmost())
		grid[rowIndex][columnIndex - 1].hasRight = false;


	while (isIsolated(current))
	{
		if (pathStack.size() == 0)  return;

		rowIndex = pathStack.top().row;
		columnIndex = pathStack.top().column;

		curROW = rowIndex;
		curCOL = columnIndex;

		current = &grid[rowIndex][columnIndex];
		pathStack.pop();
	}

	int side;
	while (1) {
		side = (int)(((float)rand() / (RAND_MAX)) * 4);
		if (side == 0)
			if (current->hasLeft)  columnIndex--;		else continue;
		else if (side == 1)
			if (current->hasLower) rowIndex++;			else continue;
		else if (side == 2)
			if (current->hasRight) columnIndex++;		else continue;
		else if (side == 3)
			if (current->hasUpper)  rowIndex--;	    	else continue;

		break;
	}

	updateRelations(curCOL, curROW, side); //make a way
	traverse(&grid[rowIndex][columnIndex]);
}


void initializeGame(int pnoOfRows, int pnoOfColumns) {
	noOfRows = pnoOfRows;
	noOfColumns = pnoOfColumns;

	gemsCount = (noOfRows*noOfColumns)/50 ;

	if (gemsCount == 0)gemsCount = 1;

	rowStepTolerance = rowStep * .06;
	columnStepTolerance = columnStep *.06;

	gridRelations = new GridRelations(noOfRows, noOfColumns);

	grid = new Cell*[noOfRows];//n for row **  initialize cell array of arrays

	for (int i = 0; i < noOfRows; i++) {
		grid[i] = new Cell[noOfColumns]; //m for column ** initialize cell array
		for (int j = 0; j < noOfColumns; j++)
		{
			grid[i][j].coord.row = i;
			grid[i][j].coord.column = j;
		} 
	}

	for (int j = 0; j < noOfColumns; j++)
	{
		grid[0][j].hasUpper = false;	grid[noOfRows - 1][j].hasLower = false;
	}

	for (int i = 0; i < noOfRows; i++)
	{
		grid[i][0].hasLeft = false;     grid[i][noOfColumns - 1].hasRight = false;
	}

	traverse(&grid[(int)(((float)rand() / (RAND_MAX)) * noOfRows)][(int)(((float)rand() / (RAND_MAX)) * noOfColumns)]);
	
	int counter = 0;

	GemsLocations = new Coord[gemsCount]();
	GhostInitialLocations = new Coord[ghostCount]();
	Ghosts= new Ghost[ghostCount]();

		while (1){
			counter = 0;
			StartCell = randomCell(noOfRows, noOfColumns);
			
			
			for (int i = 0; i < ghostCount && counter < noOfRows*noOfColumns ; i++) {
			
				GhostInitialLocations[i] = randomCell(noOfRows, noOfColumns);

				if ((abs(GhostInitialLocations[i].row - StartCell.row) < 5 && abs(GhostInitialLocations[i].column - StartCell.column) < 5))
				{
					i--; counter++;  continue;
				}
			
				for (int j = 0; j < i; j++) //check duplicates
					if ( GhostInitialLocations[j].row == GhostInitialLocations[i].row && GhostInitialLocations[j].column == GhostInitialLocations[i].column)
					{
						  i--; counter++;  break;
					}

				
			}


			if (counter > noOfRows*noOfColumns)
				continue;
			

			counter = 0;

			for (int i = 0; i < gemsCount && counter < noOfRows*noOfColumns; i++) {
				
				GemsLocations[i] = randomCell(noOfRows, noOfColumns);

				if (abs(GemsLocations[i].row - StartCell.row) < 5 && abs(GemsLocations[i].column - StartCell.column) < 5)
				{
					i--; counter++; continue;
				}

				for (int j = 0; j < i;j++) //check duplicates
					if (abs(GemsLocations[i].row - GemsLocations[j].row) < 5 && abs(GemsLocations[i].column - GemsLocations[j].column) < 5)
					{
						  i--; counter++;  break;
					}

			}

			if (counter < noOfRows*noOfColumns) {
				break;
			}
		}


		for (int i = 0; i < ghostCount; i++)
			Ghosts[i].currentCell = GhostInitialLocations[i];



		totalTime = noOfRows*noOfColumns * 35;

		

		glutTimerFunc(16, updateRemainingTime, totalTime);// blocking
}