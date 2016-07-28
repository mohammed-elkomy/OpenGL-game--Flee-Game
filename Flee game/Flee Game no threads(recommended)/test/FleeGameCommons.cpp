#include <GL\glew.h>
#include <GL\freeglut.h>
#include "FleeCommons.h"
#include <math.h>

float   rowStep = 5, columnStep = 5, rowStepTolerance, columnStepTolerance, velocity , totalTime;
int delta = 0, gemsCount = 1, ghostCount = 0, TimeRemainingInMSC,  noOfRows, noOfColumns , winnerIndex = 6, alternator = 0, gameSpeedFactor;;
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
		velocity = 1.3 - exp(-8 * (v / totalTime));
		if (velocity > 1) velocity = 1;
	}

	if ((delta += 1) == 360)
		delta = 0;

	alternator= ++alternator % 3 ;
	glutTimerFunc((alternator? 16 : 17), updateRemainingTime, v);
}