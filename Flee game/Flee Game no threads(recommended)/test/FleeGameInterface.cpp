// include files
#include <GL\glew.h>
#include <GL\freeglut.h>

#include "GameLogic.h"
#include "UI.h"
#include "texturingUtil.h"

#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <windows.h>

using namespace std;


// escape key (for exit)
#define ESC 27
//extended ascii characters 
#define G_BETA 225
#define D_PHI 237
#define U_GAMA 218
#define C_other 196


void syntheticLeftClick()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}


//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int pw, int ph)
{
	w = pw;
	h = ph;
	TwoBywindowhight = 2.0f/h;
	initTexturing();
	float ratio = ((float)w) / ((float)h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(70, ratio, .1, ghostModeOn||pcModeOn ? 200 : 15); // perspective transformation
	//glOrtho(-5,5,-5,5,.1,100);
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	syntheticLeftClick();

}



//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{

	if (key == ESC || key == 'q' || key == 'Q') exit(0);
	if (!GAMEOVER || pcModeOn)
	{

		if ((key == 'u' || key == 'U' || key == U_GAMA) && (ghostModeOn || pcModeOn))
			deltaHeight = playerControlParameters.deltaHeight;
		else if ((key == 'd' || key == 'D' || key == D_PHI) && (ghostModeOn || pcModeOn))
			deltaHeight = -playerControlParameters.deltaHeight;
		else if ((key == 'g' || key == 'G' || key == G_BETA) && !GAMEOVER)
		{
			ghostModeOn = !ghostModeOn;
			pcModeOn = false;

			if (ghostModeOn)
			{
				statusBar = ghostOn;
				statusTimeRemaining = 2000;
			}
			else
			{
				statusBar = ghostOff;
				statusTimeRemaining = 2000;

				heightMem=cameraHeight;
				glutTimerFunc(1, smoothDecrease, 200);
			}
			
			

		}

		else if ((key == 'c' || key == 'C' || key == C_other) && !GAMEOVER) {
			pcModeOn = !pcModeOn;
			ghostModeOn = false;

			if (pcModeOn)
			{
				statusBar = computerOn;
				statusTimeRemaining = 2000;
			}
			else
			{
				statusBar = computerOff;
				statusTimeRemaining = 2000;
				

				heightMem = cameraHeight;
				glutTimerFunc(1, smoothDecrease, 200);
			}
		}

		float ratio = ((float)w) / ((float)h); // window aspect ratio
		glMatrixMode(GL_PROJECTION); // projection matrix is active
		glLoadIdentity(); // reset the projection
		gluPerspective(70, ratio, .1, ghostModeOn||pcModeOn ? 200 : 15); // perspective transformation
		glMatrixMode(GL_MODELVIEW); // return to modelview mode
		glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	}
}


void processNormalKeysUp(unsigned char key, int xx, int yy)
{
	if (!GAMEOVER || pcModeOn)
	{
		if ((key == 'u' || key == 'U' || key == U_GAMA)  &&  (ghostModeOn || pcModeOn))
			deltaHeight = 0;
		else if ((key == 'd' || key == 'D' || key == D_PHI) && (ghostModeOn || pcModeOn))
			deltaHeight = 0;
	}
}

void pressSpecialKey(int key, int xx, int yy)
{	
	if (!GAMEOVER || pcModeOn)
	{
		switch (key) {
		case GLUT_KEY_UP: deltaMove = playerControlParameters.deltaMove;   break;
		case GLUT_KEY_DOWN: deltaMove = -playerControlParameters.deltaMove;    break;
		case GLUT_KEY_RIGHT:   deltaAngleDueTokeyboard = -playerControlParameters.deltaAngleDueTokeyboard;   break;
		case GLUT_KEY_LEFT:   deltaAngleDueTokeyboard = playerControlParameters.deltaAngleDueTokeyboard;  break;
		}

	}
}

void releaseSpecialKey(int key, int currentX, int currentY)
{
	if (!GAMEOVER || pcModeOn)
	{
		switch (key) {
		case GLUT_KEY_UP: deltaMove = 0.0;  break;
		case GLUT_KEY_DOWN: deltaMove = 0.0;  break;
		case GLUT_KEY_RIGHT:   deltaAngleDueTokeyboard = 0;  break;
		case GLUT_KEY_LEFT:   deltaAngleDueTokeyboard = 0;  break;
		}
	}

}

//----------------------------------------------------------------------
// Process mouse drag events
// 
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
void mouseMove(int currentX, int currentY)
{
	if (isDragging) { // only when dragging
					  // update the change in angle

		if (currentY > previousY)
			lookingUPAngle += playerControlParameters.lookingUPAngle;
		else 
			lookingUPAngle -= playerControlParameters.lookingUPAngle;
		previousY = currentY;
		
     }

}

void mouseButton(int button, int state, int currentX, int currentY)
{
	if (!GAMEOVER || pcModeOn)
	{
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) { // left mouse button pressed
				isDragging = 1; // start dragging
				yDragStart = currentY; // save x where button first pressed
			}
			else { /* (state = GLUT_UP) */
				isDragging = 0; // no longer dragging
			}
		}
	}
}


//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	printf("\n\
-----------------------------------------------------------------------\n\
\t\t\t  Welcome to Flee! \n\
-----------------------------------------------------------------------\n\
-----------------------------------------------------------------------\n\
\t\t  Basic move \n\
-----------------------------------------------------------------------\n\
	- Hold arrows to move \n\
	- drag mouse to adjust the point you're looking at\n\
-----------------------------------------------------------------------\n\
\t\t  Commands \n\
-----------------------------------------------------------------------\n\
	- ESC to quit :( \n\
	- g for ghost mode 3:)\n\
	- c for computer mode :O\n\
-----------------------------------------------------------------------\n\
\t\t  objective \n\
-----------------------------------------------------------------------\n\
	- Find a gem before you run out of time :P :P\n\
	- Don't crash into any enemy ghost or you die :( :(  \n\
-----------------------------------------------------------------------\n\
\t\t  Ghost mode features \n\
-----------------------------------------------------------------------\n\
	- Use u to go up into the sky :3\n\
	- Or d to decend deep down :3 :3 :3\n\
	- Time is freezed \n\
	- Can walk through walls\n\
	- Enemy ghosts doesn't kill you\n\
	- So no one can win or lose\n\
-----------------------------------------------------------------------\n\
\t\t  PC mode features \n\
-----------------------------------------------------------------------\n\
	- Use u to go up into the sky :3\n\
	- Or d to decend deep down :3 :3 :3\n\
	- Time isn't freezed \n\
	- Can walk through walls\n\
	- Enemy ghosts doesn't kill you\n\
	- As you are observing ,\"the first ghost to find the gem wins\"\n\
-----------------------------------------------------------------------\n\
\t credits : Mohammed Alaa El Komy 3:) 3:) 3:) \n\
-----------------------------------------------------------------------\n\
\t\t\t Happy Gaming :) \n\
-----------------------------------------------------------------------\n"
);
	

	string textToprint = "***********************************************************************\n\t\Enter number of rows 10 ~ 50\n\
-----------------------------------------------------------------------\n\t";
while (!(noOfRows >= 10 && noOfRows <= 50))
{
	printf(textToprint.c_str());

	if (!(cin >> noOfRows)) {//input valdiation
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		textToprint="\n\t\please enter the number of rows properly 10 ~ 50\n\
-----------------------------------------------------------------------\n\t";
	}
}


textToprint="-----------------------------------------------------------------------\n\t\Enter number of columns 10 ~ 50\n\
-----------------------------------------------------------------------\n\t";
while (!(noOfColumns >= 10 && noOfColumns <= 50))
{
	printf(textToprint.c_str());
		
	if (!(cin >> noOfColumns)) {//input valdiation
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		textToprint="-----------------------------------------------------------------------\n\t\please enter the number of columns properly 10 ~ 50\n\
-----------------------------------------------------------------------\n\t";
	}
}



textToprint= "-----------------------------------------------------------------------\n\t\Enter number of ghosts 1 ~ 6\n\
-----------------------------------------------------------------------\n\t";
while (!(ghostCount >= 1 && ghostCount < 7))
{
	printf(textToprint.c_str());

	if (!(cin >> ghostCount)) {//input valdiation
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		textToprint="-----------------------------------------------------------------------\n\t\please enter the number of ghosts properly 1 ~ 6\n\
-----------------------------------------------------------------------\n\t";

	}
}

textToprint = "***********************************************************************\n\t\Enter game speed factor 1~5 (5 = fastest)\n\
-----------------------------------------------------------------------\n\t";

while (!(gameSpeedFactor >= 1 && gameSpeedFactor <= 5))
{
	printf(textToprint.c_str());

	if (!(cin >> gameSpeedFactor)) {//input valdiation
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		textToprint = "\n\t\please game speed factor  properly 1 ~ 5 (5 = fastest)\n\
-----------------------------------------------------------------------\n\t";
	}

}

gameSpeedFactor = 6 - gameSpeedFactor;

playerControlParameters.setGameControl();//player step for each clock cycle
ghostControlParameters.setParameters();//ghost step for each clock cycle



	srand(time(NULL));
	glutInit(&argc, argv);
	initializeGame(noOfRows, noOfColumns);
	
	GraphicalCell temp = getGraphicalRepresentionOfCell(StartCell);

	currentX = temp.getCenterX();
	currentY = temp.getCenterY();

	for (int i = 0; i < ghostCount; i++) {
		temp = getGraphicalRepresentionOfCell(GhostInitialLocations[i]);
		Ghosts[i].currentX = temp.getCenterX();
		Ghosts[i].currentY = temp.getCenterY();
	}	


	// general initializations
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Flee");
	
	glutEnterGameMode();
	
	// register callbacks
	glutReshapeFunc(changeSize); // window reshape callback

	glutDisplayFunc(renderScene); // (re)display callback

	glutTimerFunc(100, sustainingGameLogicUpdate, 0);//keep montoring the game logic (winners , losers and ghosts AI)
	glutTimerFunc(100, sustainingViewUpdate, 0);	//update the view

	glutTimerFunc(100, statusBarAknowledge, 0);

	glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down

	glutMouseFunc(mouseButton); // process mouse button push/release
	glutMotionFunc(mouseMove); // process mouse dragging motion
	
	glutKeyboardFunc(processNormalKeys); // process standard key clicks
	glutKeyboardUpFunc(processNormalKeysUp); // process standard key release

	glutSpecialFunc(pressSpecialKey); // process special key pressed
	glutSpecialUpFunc(releaseSpecialKey); // process special key release
	glutSetCursor(GLUT_CURSOR_NONE);
	glEnable(GL_DEPTH_TEST);

	

	// enter GLUT event processing cycle
	glutMainLoop();


	//free resources
	delete grid;
	delete GemsLocations;
	delete Ghosts;
	delete GhostInitialLocations;

	return 0; // this is just to keep the compiler happy
}
