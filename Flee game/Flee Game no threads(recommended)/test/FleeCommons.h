#pragma once
extern bool ghostModeOn, pcModeOn, GAMEOVER;
extern int TimeRemainingInMSC, ghostCount, gemsCount, delta, noOfRows, noOfColumns,winnerIndex, gameSpeedFactor;
extern float columnStep, rowStep, rowStepTolerance, columnStepTolerance, velocity, totalTime;



struct GhostControlParameters {
	float comlumFactor;
	float rowFator;
	float angelFactor;

	void setParameters() {
		comlumFactor = columnStep / 100.0 / gameSpeedFactor;
		rowFator = rowStep / 100.0 / gameSpeedFactor;
		angelFactor = 25.0f * gameSpeedFactor;
	}
};

struct PlayerControlParameters {
	float	deltaMove;
	float	deltaAngleDueTokeyboard;
	float	deltaHeight;
	float	lookingUPAngle;

	void setGameControl() {

		deltaMove = 1.0f / gameSpeedFactor;
		deltaAngleDueTokeyboard = .095f / gameSpeedFactor;
		lookingUPAngle = .05f / gameSpeedFactor;
		deltaHeight = .08f / gameSpeedFactor;

	}

};

extern GhostControlParameters ghostControlParameters;
extern PlayerControlParameters playerControlParameters;


void updateRemainingTime(int v);