#include <GL\glew.h>
#include <GL\freeglut.h>
#include "FleeCommons.h"
#include <math.h>

float   rowStep = 5, columnStep = 5, rowStepTolerance, columnStepTolerance, velocity = 1, totalTime, stepFactor, delta = 0 ;
int  gemsCount = 1, ghostCount = 0, TimeRemainingInMSC,  noOfRows, noOfColumns , winnerIndex = 6, alternator = 0, gameSpeedFactor ;
bool ghostModeOn = false, pcModeOn = false, GAMEOVER = false;

GhostControlParameters ghostControlParameters;
PlayerControlParameters playerControlParameters;

void updateRemainingTime(int v) {
	TimeRemainingInMSC = v;

	if (!v || GAMEOVER)
		GAMEOVER = true;
	else
	{
		v = (!ghostModeOn) ? --v : v;
		
	}

	alternator= ++alternator % 3 ;

	glutTimerFunc((alternator? 16 : 17), updateRemainingTime, v);
}


