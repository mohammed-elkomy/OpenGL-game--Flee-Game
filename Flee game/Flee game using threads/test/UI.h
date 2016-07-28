#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include "mazeModel.h"
#include "FleeCommons.h"

extern float ghostColors[7][3];
extern bool displayListNotAvailable ;
extern GLuint index;

struct GraphicalCell {
	float	left, right, top, bottom;
	GraphicalCell() {}		//default

	float getCenterX() {
		return (right + left) / 2;
	}
	float getCenterY() {
		return (top + bottom) / 2;
	}
};


void compileObjects();


GraphicalCell getGraphicalRepresentionOfCell(int curROW, int curCOL);
GraphicalCell getGraphicalRepresentionOfCell(Coord coord);
Coord getRowAndColumnOFXY(float pcurrentX, float pcurrentY);
