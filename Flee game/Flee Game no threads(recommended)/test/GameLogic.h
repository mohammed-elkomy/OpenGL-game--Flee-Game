#pragma once
#include <string>
#include <sstream>
#include "mazeModel.h"

using namespace std;

enum StatusBar {
	stableMode, ghostOn, ghostOff, computerOn, computerOff
};

extern float deltaHeight , cameraHeight , deltaAngleDueTokeyboard , deltaMove   /*initially camera doesn't move*/
, Camx , Camy  /*camera points initially along y-axis*/
, lx , ly  // camera points initially along y-axis
, angle  // angle of rotation for the camera direction
, deltaAngleDueToMouse  // additional angle change when dragging
, currentX , currentY  //Camera position
, TwoBywindowhight 
, lookingUPAngle 
, heightMem;


extern short nearestX, nearestY, nearestXLOG, nearestYLOG, MnearestX, MnearestY, MnearestXLOG, MnearestYLOG;


extern int MCol, MRow, w , h 
, winnerIndex 
// Mouse drag control
, isDragging  // true when dragging
, yDragStart // records the x-coordinate when dragging starts
, statusTimeRemaining ,
previousY ;

extern bool someoneWon, isMoving ;
extern string printOutText;
extern StatusBar statusBar;

void deployTheWorld();
void renderScene(void);
void statusBarAknowledge(int par);
void XY_Monitor(Coord Lcell);
void sustainingViewUpdate(int timeRemainingInThisCycle);
void sustainingGameLogicUpdate(int timeRemainingInThisCycle);
string getWinnerText(int i);
void smoothDecrease(int par);
