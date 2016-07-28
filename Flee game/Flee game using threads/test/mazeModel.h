#pragma once
#include "FleeCommons.h"
#include <stack>
#include <list>

#define LEFT 0
#define DOWN 1
#define RIGHT 2
#define UP 3
#define NO_DIRECTION -1

struct Coord {
	int row, column;
};


struct GridRelations {
	bool **horizontal;
	bool **vertical;

	GridRelations(int noOfRows, int noOfColumns) {
		horizontal = new bool*[noOfRows];
		for (int i = 0; i < noOfRows; ++i)
			horizontal[i] = new bool[noOfColumns - 1](); //parenthesis for default value

		vertical = new bool*[noOfColumns];
		for (int i = 0; i < noOfColumns; ++i)
			vertical[i] = new bool[noOfRows - 1](); //parenthesis for default value
	}
	~GridRelations() { delete horizontal;  delete vertical; }
};


struct Cell {
	Coord coord;

	bool hasLeft , hasRight , hasUpper , hasLower ;
	//isVisited = false; this is redundant as a cell to be visited ,neighbours are null
	Cell(int Prow, int Pcolumn) {
		Cell();
		coord.row = Prow;	coord.column = Pcolumn;
	}
	Cell() {
		 hasLeft = true, hasRight = true, hasUpper = true, hasLower = true;
	}

	bool isNotRightmost(int noOfColumns) {
		return coord.column + 1 != noOfColumns;
	}

	bool isNotDownmost(int noOfRows) {
		return  coord.row + 1 != noOfRows;
	}

	bool isNotUpmost() {
		return coord.row - 1 != -1;
	}

	bool isNotLeftmost() {
		return coord.column - 1 != -1;
	}
};




struct Ghost {
	float currentX;
	float currentY;
	float   angle;
	float angleStep;
	short Direction;
	Coord currentCell;
	int strictAngle;

	Ghost() {
		angle=0;
		angleStep=0;
		Direction=NO_DIRECTION;
		strictAngle=0;
	}

	void updateAngleStep(int desiredAngle) {
		int delta = desiredAngle - strictAngle;

		if (abs(delta)  > 180 )  delta = -delta % 180 ;

		angleStep = delta / ghostControlParameters.angelFactor;
		strictAngle = desiredAngle;
	}
};

// general variables
extern  std::stack< Coord > pathStack;
extern  Cell **grid;
extern  GridRelations *gridRelations;
extern Coord StartCell;
extern Coord *GemsLocations;
extern Coord *GhostInitialLocations;
extern Ghost *Ghosts;


void initializeGame(int noOfRows, int noOfColumns);

