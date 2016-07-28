#include <string>
#include <sstream>

#include "GameLogic.h"
#include <GL\glew.h>
#include <GL\freeglut.h>

#include <stdlib.h> // standard definitions
#include "texturingUtil.h"
#include "FleeCommons.h"
#include "mazeModel.h"
#include "UI.h"
#include <iostream>


struct  {
	bool left,right,up,down;
	void setMembers(bool Pleft,bool Pright,bool Pdown,bool Pup) {
		left = Pleft,right = Pright,up = Pup,down = Pdown;
	}
}CanGo;

StatusBar statusBar;

float deltaHeight = 0, cameraHeight = 2.0, deltaAngleDueTokeyboard = 0, deltaMove = 0.0  /*initially camera doesn't move*/
, Camx = 0.0, Camy = 1.0 /*camera points initially along y-axis*/
, lx = 0.0, ly = 1.0 // camera points initially along y-axis
, angle = 0.0 // angle of rotation for the camera direction
, deltaAngleDueToMouse = 0.0 // additional angle change when dragging
, currentX = 0, currentY = 0 //Camera position
, TwoBywindowhight = 0
, lookingUPAngle = 0
, heightMem,
torusHight = 20;


short nearestX, nearestY, nearestXLOG, nearestYLOG, MnearestX, MnearestY, MnearestXLOG, MnearestYLOG;

int MCol, MRow, w = 0, h = 0

// Mouse drag control
, isDragging = 0 // true when dragging
, yDragStart = 0// records the x-coordinate when dragging starts
, statusTimeRemaining = 0,
previousY = 0;

bool someoneWon = false, isMoving = false, draw = false, aknowledged = false;
string printOutText;


string getWinnerText(int i) {
	glColor3fv(ghostColors[i]);
	switch (i) {
	case 0:
		glTranslatef(-50, 15, 0);
		glScalef(.7f, .7f, 0);
		return "Dark red wins";
	case 1:
		glTranslatef(-190, 20, 0);
		glScalef(.7f, .7f, 0);
		return "Dark green wins";
	case 2:
		glTranslatef(-100, 20, 0);
		glScalef(.7f, .7f, 0);
		return "Dark blue wins";
	case 3:
		glTranslatef(-210, 0, 0);
		glScalef(.6f, .6f, 0);
		return "Dark magenta wins";
	case 4:
		glTranslatef(-110, 10, 0);
		glScalef(.65f, .65f, 0);
		return "Dark yellow wins";
	case 5:
		glTranslatef(-120, 20, 0);
		glScalef(.7f, .7f, 0);
		return "Dark cyan wins";
	default:
		glTranslatef(200, 20, 0);
		return "You win";
	}
}



void statusBarAknowledge(int par)
{
	if (statusTimeRemaining <= 0)
		statusBar = stableMode;
	else
		statusTimeRemaining -= 100;

	glutTimerFunc(100, statusBarAknowledge, 0);
}


//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{
	void * font = GLUT_STROKE_ROMAN;


	// Clear color and depth buffers
	glClearColor(0, 0, 0, 1); // sky color is black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	deployTheWorld();

	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 400, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);


	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	glTranslatef(40, 50, 0);
	glScalef(.3f, .3f, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(180, 0, 0, 1);

	stringstream timeLeft;

	int secs = TimeRemainingInMSC * 20 / 1000;
	int mins = secs / 60;
	secs = secs - mins * 60;

	timeLeft << mins << ":" << secs << ":" << (int)(((TimeRemainingInMSC * 20) % 1000) / 10);//time
	
	printOutText = timeLeft.str();


	for (string::iterator i = printOutText.begin(); i != printOutText.end(); ++i)
	{
		char c = *i;
		//this does nothing, color is fixed for Bitmaps when calling glRasterPos
		//glColor3f(1.0, 0.0, 1.0); 
		glutStrokeCharacter(font, c);

	}

	glLineWidth(3);
	if ( someoneWon ) {
		GAMEOVER = true;
		glColor3f(0.0, 1.0, 0);
		printOutText = getWinnerText(winnerIndex);
	}
	else if (draw) {
		GAMEOVER = true;
		glColor3f(0.0, 1.0, 0);
		glPushMatrix();
		glTranslatef(400, 0, 0);
		printOutText = "Draw !!";
		glPopMatrix();
	}


	else if (!TimeRemainingInMSC)
	{
		GAMEOVER = true;
		glColor3f(0.0, 0.0, 1.0);
		printOutText = "GAME OVER";

	}


	if (GAMEOVER) {
		glScalef(3, 3, 0);
		glTranslatef(-80, -240, 0);

		for (string::iterator i = printOutText.begin(); i != printOutText.end(); ++i)
		{
			char c = *i;
			//this does nothing, color is fixed for Bitmaps when calling glRasterPos
			//glColor3f(1.0, 0.0, 1.0); 
			glutStrokeCharacter(font, c);
		}
	}
	else if (statusBar != stableMode)
	{//status bar condtions
		if (statusBar == ghostOn)
			printOutText = "Ghost mode on !! 3:)";
		else if (statusBar == ghostOff)
			printOutText = "Ghost mode off  :( :( ";
		else if (statusBar == computerOn)
			printOutText = "Computer mode on 3:) ";
		else if (statusBar == computerOff)
			printOutText = "Computer mode off  :( :( ";

		glLineWidth(1);
		glColor3f(0.0, 1.0, 0.0);
		glLoadIdentity();
		glRotatef(180, 0, 1, 0);
		glRotatef(180, 0, 0, 1);
		glTranslatef(550, -30, 0);
		glScalef(.15, .15, 0);

		for (string::iterator i = printOutText.begin(); i != printOutText.end(); ++i)
		{
			char c = *i;
			glutStrokeCharacter(font, c);
		}
	}

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	// Reset transformations
	glLoadIdentity();


	// Set the camera centered at (x,y,1) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
		currentX, currentY, cameraHeight,
		//10, 10, 0,
		currentX + lx, currentY + ly, cameraHeight + lookingUPAngle,
		0, 0, 1);

	glutSwapBuffers(); // Make it all visible
}


void deployTheWorld() {
	if (displayListNotAvailable)
		compileObjects();

	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < ghostCount; i++) { //call a ghost list

		glPushAttrib(GL_CURRENT_BIT);
		glPushMatrix();
		glColor3fv(ghostColors[i]);
		glTranslatef(Ghosts[i].currentX, Ghosts[i].currentY, .5);

		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1, 1, 1);
		glRotatef(Ghosts[i].angle, 0, 0, 1);
		glPushMatrix();
		glTranslatef(-.75, 1.2f, 2);

		glPushMatrix();
			glScalef(.285f,.285,.285);
			glCallList(index + 4); //left eye
			glPopMatrix();

			glTranslatef(1.75, 0, 0);

			glPushMatrix();
				glScalef(.285f, .285, .285);
				glCallList(index + 4);  //right eye
			glPopMatrix();
		glPopMatrix();

		glPopAttrib();

		
		glRotatef(delta, 0, 0, 1);

		if (GAMEOVER &&  i == winnerIndex) {
			glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1,1,0);
			glPushMatrix();
				glTranslatef(0, 0,( (torusHight - (!(pcModeOn || ghostModeOn) ? .05 : .2) ) > 4 ? torusHight -=(!(pcModeOn || ghostModeOn) ? .05 : .2)  : 4) );
				glScalef(1,1,.5);
				glutSolidTorus(.3, 1, 20, 30);
			glPopMatrix();
			glPopAttrib();
		}

		glCallList(index + 3);


		glPopMatrix();
		glPopAttrib();

	}





	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor4f(.1, .1, 1, .9);


	for (int i = 0; i < gemsCount; i++) { //call a gem list
	
			if (!(GemsLocations[i].row == Ghosts[winnerIndex].currentCell.row &&GemsLocations[i].column == Ghosts[winnerIndex].currentCell.column && pcModeOn))
			{
				GraphicalCell temp = getGraphicalRepresentionOfCell(GemsLocations[i].row, GemsLocations[i].column);

				glPushMatrix();
				glTranslatef(temp.getCenterX(), temp.getCenterY(), 0);

				glRotatef(delta, 0, 0, 1);
				glCallList(index);
				glPopMatrix();
			}
		
	}

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glCallList(index + 1);//walls
}



void XY_Monitor(Coord Lcell) {
	//grid border rejection
	if (currentX < columnStepTolerance || (currentX > noOfColumns*columnStep - columnStepTolerance))
		currentX -= deltaMove * lx * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);

	if ((-currentY <   columnStepTolerance) || (-currentY > noOfRows*rowStep - columnStepTolerance))
		currentY -= deltaMove * ly * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);

	nearestYLOG = (short)currentY;

	float remainder = currentY - (short)(currentY / columnStep) *columnStep;

	if (-remainder > 2.5)
		nearestY = currentY - remainder - columnStep;

	else
		nearestY = currentY - remainder;

	nearestY = -noOfRows*columnStep == nearestY ? nearestY + columnStep : nearestY;

	nearestYLOG = -nearestY / 5 - 1;
	if (nearestYLOG == -1)nearestYLOG = 0;

	nearestXLOG = currentX;

	remainder = currentX - (short)(currentX / rowStep) *rowStep;

	if (remainder > 2.5)
		nearestX = currentX - remainder + rowStep;

	else
		nearestX = currentX - remainder;

	nearestX = noOfColumns*rowStep == nearestX ? nearestX - rowStep : nearestX;

	nearestXLOG = nearestX / 5 - 1;
	if (nearestXLOG == -1)nearestXLOG = 0;


	bool isInRejectedSquare = false;
	if (nearestY + columnStepTolerance > currentY && nearestY - columnStepTolerance < currentY && nearestY != 0 && nearestX + rowStepTolerance > currentX && nearestX - rowStepTolerance < currentX)
		isInRejectedSquare = true;


	bool isInRejectedXbar = false;
	if (!gridRelations->horizontal[Lcell.row][nearestXLOG] && nearestX + rowStepTolerance > currentX && nearestX - rowStepTolerance < currentX)
		isInRejectedXbar = true;


	bool isInRejectedYbar = false;
	if (!gridRelations->vertical[Lcell.column][nearestYLOG] && nearestY + columnStepTolerance > currentY && nearestY - columnStepTolerance < currentY && nearestY != 0)
		isInRejectedYbar = true;

	Coord ComplementeryLcell = getRowAndColumnOFXY(MnearestX * 2 - currentX, MnearestY * 2 - currentY);
	bool Corner = false;

	if (!(ghostModeOn || pcModeOn) && isInRejectedSquare && (gridRelations->horizontal[MRow][MnearestXLOG] && gridRelations->vertical[MCol][MnearestYLOG]) &&
		abs(ComplementeryLcell.row - MRow) == 1 && abs(ComplementeryLcell.column - MCol) == 1 &&
		(!gridRelations->horizontal[ComplementeryLcell.row][MnearestXLOG] && !gridRelations->vertical[ComplementeryLcell.column][MnearestYLOG]))
	{
		Corner = true;
		currentX -= deltaMove * lx * .4f*(ghostModeOn || pcModeOn ? 1 : velocity);
		currentY -= deltaMove * ly * .4f*(ghostModeOn || pcModeOn ? 1 : velocity);
	}

	if (!Corner&&!(isInRejectedSquare || isInRejectedYbar || isInRejectedXbar) || ghostModeOn || pcModeOn)
	{
		MnearestX = nearestX,
			MnearestY = nearestY,

			MnearestXLOG = nearestXLOG,
			MnearestYLOG = nearestYLOG,

			MCol = Lcell.column,
			MRow = Lcell.row;

		return;
	}
	else {
		if (isInRejectedXbar)
			currentX -= deltaMove * lx * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);

		if (isInRejectedYbar)
			currentY -= deltaMove * ly * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);

		if (isInRejectedSquare && !(isInRejectedYbar || isInRejectedXbar)) {
			currentX -= deltaMove * lx * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);
			currentY -= deltaMove * ly * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);

			if (gridRelations->horizontal[MRow][MnearestXLOG]) {//       |...|	
				if (deltaMove * lx >0)
					currentX += .2f;
				else currentX -= .2f;
			}

			if (gridRelations->vertical[MCol][MnearestYLOG]) {
				if (deltaMove * ly >0)
					currentY += .2f;
				else currentY -= .2f;
			}


		}

	}
	if (!Corner)
		if (!gridRelations->horizontal[MRow][MnearestXLOG] && MnearestX + rowStepTolerance > currentX && MnearestX - rowStepTolerance < currentX)
		{
			if (deltaMove * lx > 0)
				currentX += .2f;
			else currentX -= .2f;
		}

		else if (!gridRelations->vertical[MCol][MnearestYLOG] && MnearestY + columnStepTolerance > currentY && MnearestY - columnStepTolerance < currentY && MnearestY != 0)
		{
			if (deltaMove * ly > 0)
				currentY += .2f;
			else currentY -= .2f;
		}

	MnearestX = nearestX,
		MnearestY = nearestY,

		MnearestXLOG = nearestXLOG,
		MnearestYLOG = nearestYLOG,

		MCol = Lcell.column,
		MRow = Lcell.row;
}


//----------------------------------------------------------------------
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void sustainingViewUpdate(int params) {

	Coord Lcell = getRowAndColumnOFXY(currentX, currentY);

	if (GAMEOVER) velocity = .05f;

	if (deltaMove) { // update camera position
		currentX += deltaMove * lx * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);
		currentY += deltaMove * ly * .2f*(ghostModeOn || pcModeOn ? 1 : velocity);
		XY_Monitor(Lcell);
	}

	if (deltaAngleDueTokeyboard) {
		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngleDueTokeyboard)*(ghostModeOn || pcModeOn ? 1 : velocity);
		ly = cos(angle + deltaAngleDueTokeyboard)*(ghostModeOn || pcModeOn ? 1 : velocity);
		angle += deltaAngleDueTokeyboard; // update camera turning angle
	}

	if (deltaHeight)
		cameraHeight += deltaHeight;

	if (cameraHeight < 2)cameraHeight = 2;

	glutPostRedisplay(); // redisplay everything

	glutTimerFunc(10, sustainingViewUpdate, 0);//repeat
}


void sustainingGameLogicUpdate(int gameLogicClock)
{
	if (gameLogicClock < 0) return;

	Coord playercell = getRowAndColumnOFXY(currentX, currentY);
	GraphicalCell playerGcell=getGraphicalRepresentionOfCell(playercell);


	if (playerGcell.left < currentX && playerGcell.right > currentX && playerGcell.top > currentY && playerGcell.bottom < currentY)//isStrictlyIncell = true;

	if (!(ghostModeOn || pcModeOn || someoneWon))
	{
		for (int i = 0; i < gemsCount; i++)
			if ((GemsLocations[i].row == playercell.row) &&( GemsLocations[i].column == playercell.column))
			{

				 someoneWon = true;
			
				 deltaMove = .2;
				 deltaAngleDueTokeyboard = -.005f;
				 lookingUPAngle -= .1;  
				 deltaHeight = .01f;

				 glutTimerFunc(5, sustainingGameLogicUpdate, gameLogicClock - 5);
				 return;
			}

		for (int i = 0; i < ghostCount; i++)
			if ( (Ghosts[i].currentCell.row == playercell.row)  && (Ghosts[i].currentCell.column == playercell.column) )
			{
				someoneWon = true;  

				winnerIndex = i; 

				deltaMove = .2;
				deltaAngleDueTokeyboard = -.005f; 
				lookingUPAngle -= .1; 
				deltaHeight = .02f;

				glutTimerFunc(5, sustainingGameLogicUpdate, gameLogicClock - 5);
				return;
			}
	}

	
	if (gameLogicClock == 0 &&  winnerIndex <6 && !(ghostModeOn || pcModeOn) && someoneWon && !aknowledged) {
		aknowledged = true;
		gameLogicClock = 500 * gameSpeedFactor;
		if (abs(currentX - Ghosts[winnerIndex].currentX) > abs(currentY - Ghosts[winnerIndex].currentY)) //look at the loser
																										 //left of right
			if (currentX - Ghosts[winnerIndex].currentX > 0)
			{
				Ghosts[winnerIndex].updateAngleStep(270);//right
				Ghosts[winnerIndex].Direction = RIGHT;
			}
			else {
				Ghosts[winnerIndex].updateAngleStep(90);//left
				Ghosts[winnerIndex].Direction = LEFT;
			}

		else //up or down

			if (currentY - Ghosts[winnerIndex].currentY > 0)
			{
				Ghosts[winnerIndex].updateAngleStep(0);//up
				Ghosts[winnerIndex].Direction = UP;
			}
			else {
				Ghosts[winnerIndex].updateAngleStep(180);//down
				Ghosts[winnerIndex].Direction = DOWN;
			}
			gameLogicClock = 500 * gameSpeedFactor;
			
		
		
	}

	if (gameLogicClock == 0 && !GAMEOVER) { //randomly pick a path for ghosts every 1 second (ghost next move)
		//this is the trigger moment occurs every 500 ms :)

		gameLogicClock=  500 * gameSpeedFactor;
		bool done = false;
		int winnersCount = 0;
		for (int i = 0; i < ghostCount; i++) {
			Coord ghostCell = getRowAndColumnOFXY(Ghosts[i].currentX, Ghosts[i].currentY);
			Ghosts[i].currentCell = ghostCell;

			if (pcModeOn) {
				for (int j = 0; j < gemsCount; j++)
				{
					if ( (GemsLocations[j].row == ghostCell.row)  &&  ( GemsLocations[j].column == ghostCell.column) )
					{
							someoneWon = true;    winnersCount++;
					}

					if (winnersCount > 1) {
						draw = true; someoneWon = false; return;
					}
				}

				if (1 == winnersCount) {
					winnerIndex = i;
					glutTimerFunc(5, sustainingGameLogicUpdate, gameLogicClock - 5);
					return;
				}
			}

 			CanGo.setMembers(gridRelations->horizontal[ghostCell.row][ghostCell.column - 1]
				, gridRelations->horizontal[ghostCell.row][ghostCell.column]
				, gridRelations->vertical[ghostCell.column][ghostCell.row]
				, gridRelations->vertical[ghostCell.column][ghostCell.row - 1]);

			while (!done){
				int path;

				if (!(ghostModeOn || pcModeOn))
				{
					if (CanGo.left && (playercell.row == ghostCell.row) &&( playercell.column == ghostCell.column - 1))
						path = LEFT;
					
					else if (CanGo.right &&( playercell.row == ghostCell.row) &&( playercell.column == ghostCell.column + 1))
						path = RIGHT; 
					
					else if (CanGo.up && (playercell.row == ghostCell.row - 1 )&&( playercell.column == ghostCell.column))
						path = UP; 
					
					else if (CanGo.down &&( playercell.row == ghostCell.row + 1) &&( playercell.column == ghostCell.column))
						path = DOWN; 
					else
						path = (int)(((float)rand() / (RAND_MAX)) * 4);
					
				}
				else if (pcModeOn) {

					for (int j = 0; j < gemsCount; j++) {
				
						if (CanGo.left &&( GemsLocations[j].row == ghostCell.row )&&( GemsLocations[j].column == ghostCell.column - 1))
						{
							 path = LEFT; done = true; break;
						} 
						else if (CanGo.right && (GemsLocations[j].row == ghostCell.row )&& (GemsLocations[j].column == ghostCell.column + 1))
						{
								path = RIGHT; done = true; break;
						}
						else if (CanGo.up && (GemsLocations[j].row == ghostCell.row - 1 )&&( GemsLocations[j].column == ghostCell.column))
						{
								path = UP; done = true; break;
						}
						else if (CanGo.down &&( GemsLocations[j].row == ghostCell.row + 1) &&( GemsLocations[j].column == ghostCell.column))
						{
							    path = DOWN; done = true; break;
						}
						else
						{
							path = (int)(((float)rand() / (RAND_MAX)) * 4); 
						}

					}

				}
				else
					path = (int)(((float)rand() / (RAND_MAX)) * 4);


				switch (path) {
				case LEFT:

					if ((Ghosts[i].Direction != RIGHT || done )   && ghostCell.column  > 0 && CanGo.left) //left is a path
					{
						Ghosts[i].Direction = LEFT;
						done = true;
						Ghosts[i].updateAngleStep(90);
					}
					break;

				case RIGHT:
					if ((Ghosts[i].Direction != LEFT || done) &&ghostCell.column < noOfColumns - 1 && CanGo.right )	// right is a path 
					{
						Ghosts[i].Direction = RIGHT;
						Ghosts[i].updateAngleStep(270);
						done = true;
					}
					break;

				case DOWN:
					if ((Ghosts[i].Direction != UP || done) &&ghostCell.row < noOfRows - 1 && CanGo.down )			// down is a path 
					{
						Ghosts[i].Direction = DOWN;
						Ghosts[i].updateAngleStep(180);
						done = true;
					}

					break;

				case UP:
					if ((Ghosts[i].Direction != DOWN || done) && ghostCell.row > 0 &&  CanGo.up)	// up is a path 
					{
						Ghosts[i].Direction = UP;
						Ghosts[i].updateAngleStep(0);
						done = true;
					}

					break;

				default:
					Ghosts[i].Direction = NO_DIRECTION;

				}
			}

			done = false;//the next ghost
		}
	}

	if (someoneWon && pcModeOn) return;

		//gradual ghost move implementation
		for (int i = 0; i < ghostCount; i++) {
			if (( !someoneWon ||  winnerIndex == i && someoneWon  && !(ghostModeOn || pcModeOn)) ) {

				switch (Ghosts[i].Direction) {
				case LEFT:
					Ghosts[i].currentX -= ghostControlParameters.comlumFactor;

					break;

				case RIGHT:
					Ghosts[i].currentX += ghostControlParameters.comlumFactor;
					break;

				case DOWN:
					Ghosts[i].currentY -= ghostControlParameters.rowFator;
					break;

				case UP:
					Ghosts[i].currentY += ghostControlParameters.rowFator;
					break;
				}

				if (gameLogicClock >   gameSpeedFactor * 375)
					Ghosts[i].angle += Ghosts[i].angleStep;
		

		}
		}
		

	glutTimerFunc(5, sustainingGameLogicUpdate, gameLogicClock - 5);//repeat

}

void smoothDecrease(int par) {
	if (par >0) {
		cameraHeight -= (heightMem - 2) / (20);
		glutTimerFunc(10, smoothDecrease, par - 10);
	}
}
