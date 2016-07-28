#include "UI.h"
#include "texturingUtil.h"
#define _USE_MATH_DEFINES
#include "mazeModel.h"
#include "FleeCommons.h"

#include <list>
#include <vector>
#include <math.h>

bool displayListNotAvailable = true; GLuint index;
float ghostColors[7][3] = { {.15,0,0},{0,.15,0},{0,0,.15},{.15,0,.15},{.15,.15,0},{0,.15,.15},{0,1,0} };


GraphicalCell getGraphicalRepresentionOfCell(int curROW, int curCOL) {
	GraphicalCell temp;
	temp.left = columnStep*curCOL, temp.right = columnStep*(curCOL + 1), temp.top = -rowStep* curROW, temp.bottom = -rowStep*(curROW + 1);
	return temp;
}

GraphicalCell getGraphicalRepresentionOfCell(Coord coord) {
	GraphicalCell temp;
	int curCOL = coord.column, curROW = coord.row;
	temp.left = columnStep*curCOL, temp.right = columnStep*(curCOL + 1), temp.top = -rowStep* curROW, temp.bottom = -rowStep*(curROW + 1);
	return temp;
}

Coord getRowAndColumnOFXY(float pcurrentX, float pcurrentY) {
	Coord temp;
	temp.row = abs(pcurrentY / rowStep);
	temp.column = pcurrentX / columnStep;
	return temp;
}

void compileObjects() {
		glColor3f(0.578f,.747f,.901f);

		index = glGenLists(5);
		displayListNotAvailable = false;//we have a display list

		//-------------------------------------------------------------------
		glNewList(index, GL_COMPILE); //gem 
		glBegin(GL_QUADS);
	
		glVertex3d(-.5, -.5, 2);
		glVertex3d(-.5, .5, 2);
		glVertex3d(.5, .5, 2);
		glVertex3d(.5, -.5, 2);
	
		glVertex3d(-.5, -.5, 2);
		glVertex3d(-.5, .5, 2);
		glVertex3d(-1, 1, 1.5);
		glVertex3d(-1, -1, 1.5);

		glVertex3d(-.5, .5, 2);
		glVertex3d(-1, 1, 1.5);
		glVertex3d(1, 1, 1.5);
		glVertex3d(.5, .5, 2);

		glVertex3d(.5, -.5,2);
		glVertex3d(1, 1, 1.5);
		glVertex3d(1, -1, 1.5);
		glVertex3d(.5, -.5, 2);

		glVertex3d(-.5, -.5, 2);
		glVertex3d(-1, -1, 1.5);
		glVertex3d(1, -1, 1.5);
		glVertex3d(.5, -.5, 2);

		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		
		glVertex3d(0, 0, 0.5);

		glVertex3d(-1, -1, 1.5);
		glVertex3d(1, -1, 1.5);
		glVertex3d(1, 1, 1.5); 
		glVertex3d(-1, 1, 1.5);
		glVertex3d(-1, -1, 1.5);


		glEnd();

		glEndList();

		//-------------------------------------------------------------------
		glNewList(index + 4, GL_COMPILE);
			glPushMatrix();
				glutSolidSphere(1.75, 100, 100);
			glPopMatrix();
		glEndList();
		//-------------------------------------------------------------------
		glNewList(index + 3, GL_COMPILE);
		
	
		glPushMatrix();
			glTranslatef(0,0,1.75);
			glCallList(index +4); //sphere of 1.75 radius
		glPopMatrix();

		float x, y;

		glBegin(GL_QUAD_STRIP);
	
		for (float i = 0; i <= 2 * M_PI + .2; i += .1)
		{
			x = 1.75*cos(i);
			y = 1.75*sin(i);
			glVertex3f(x, y, 1.8);
			glVertex3f(x, y, .2*cos(i*5));
		}

	
		glEnd();
		
		glEndList();

		//-------------------------------------------------------------------
		glNewList(index + 2, GL_COMPILE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

		glBegin(GL_QUAD_STRIP);
	
		for (float i = 0; i < 2 * M_PI; i += .4)
		{
			x = .1*cos(i);
			y = .1*sin(i);

			glTexCoord2f(-.01667*i + 0.05, 1);
			glVertex3f(x, y, 4);

			glTexCoord2f(-.01667*i+0.05, 0);
			glVertex3f(x, y, 0);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glEndList();

		//-------------------------------------------------------------------
		glNewList(index + 1, GL_COMPILE); //intersection point of walls

		for (int i = 0; i < noOfColumns*columnStep; i += columnStep) {

			for (int j = 0; j < noOfRows*rowStep; j += rowStep) {
				glPushMatrix();
				glTranslatef(i, -j, 0);
				glCallList(index + 2); //cylinder

				glPopMatrix();
			}
		}

		for (int i = 0; i <= noOfColumns*columnStep; i += noOfColumns*columnStep) {

			for (int j = 0; j <= noOfRows*rowStep; j += noOfRows*rowStep) {
				glPushMatrix();
				glTranslatef(i, -j, 0);
				glCallList(index + 2); //cylinder

				glPopMatrix();
			}
		}



		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glColor3f(0,0,1);
		glBegin(GL_QUADS);
	
		//outer borders horizontal
		for (int i = 0; i <= noOfColumns*columnStep; i += noOfColumns*columnStep) {
			for (float j = 1; j <= noOfRows; j++)
			{
				glTexCoord2f(0.0, 0.0); //side
				glVertex3d(i-.1, -(j - 1)*rowStep , 0);

				glTexCoord2f(1, 0.0);
				glVertex3d(i - .1, -j*rowStep , 0);

				glTexCoord2f(1, 1);
				glVertex3d(i - .1, -j*rowStep, 4);

				glTexCoord2f(0.0, 1);
				glVertex3d(i - .1, -(j - 1)*rowStep , 4);



				glTexCoord2f(0.0, 0.0); //side
				glVertex3d(i + .1, -(j - 1)*rowStep, 0);
				

				glTexCoord2f(1, 0.0);
				glVertex3d(i + .1, -j*rowStep, 0);
			

				glTexCoord2f(1, 1);
				glVertex3d(i + .1, -j*rowStep, 4);

				glTexCoord2f(0.0, 1);
				glVertex3d(i + .1, -(j - 1)*rowStep, 4);


				glTexCoord2f(0.05, 0);//top
				glVertex3d(i - .1, -j*rowStep , 4);

				glTexCoord2f(0.05, 1);
				glVertex3d(i - .1, -(j - 1)*rowStep , 4);

				glTexCoord2f(-.05, 1);
				glVertex3d(i + .1, -(j - 1)*rowStep , 4);

				glTexCoord2f(-.05, 0);
				glVertex3d(i + .1, -j*rowStep , 4);


			}
		
		}
		

		//outer borders vertical
		for (int i = 0; i <= noOfRows*rowStep; i += noOfRows*rowStep) {
	
			for (int j = 1; j <= noOfColumns; j++)
			{
				glTexCoord2f(0.0, 1);//side
				glVertex3d((j - 1)*columnStep, -i-.1, 4);

				glTexCoord2f(0.0, 0.0);
				glVertex3d((j - 1)*columnStep, -i-.1, 0);

				glTexCoord2f(1, 0.0);
				glVertex3d(j*columnStep, -i-.1, 0);

				glTexCoord2f(1, 1);
				glVertex3d(j*columnStep, -i-.1, 4);


				glTexCoord2f(0.0, 1);//side
				glVertex3d((j - 1)*columnStep, -i+.1, 4);

				glTexCoord2f(0.0, 0.0);
				glVertex3d((j - 1)*columnStep, -i + .1, 0);

				glTexCoord2f(1, 0.0);
				glVertex3d(j*columnStep, -i + .1, 0);

				glTexCoord2f(1, 1);
				glVertex3d(j*columnStep, -i + .1, 4);


				glTexCoord2f(0.05, 0);//top
				glVertex3d(j*columnStep, -i - .1, 4);

				glTexCoord2f(0.05, 1);
				glVertex3d((j - 1)*columnStep, -i - .1, 4);

				glTexCoord2f(-.05, 1);
				glVertex3d((j - 1)*columnStep, -i + .1, 4);

				glTexCoord2f(-.05, 0);
				glVertex3d(j*columnStep, -i + .1, 4);					
			}
		}

		
		////inner walls
		GraphicalCell temp;
		for (int i = 0; i < noOfRows; ++i)
			for (int j = 0; j < noOfColumns - 1; ++j)
				if (!gridRelations->horizontal[i][j]) {
					temp = getGraphicalRepresentionOfCell(i, j); //draw an edge on the right

					glTexCoord2f(0.0, 0.0);//side
					glVertex3d(temp.right-.1, temp.bottom, 0);

					glTexCoord2f(1.0, 0.0);
					glVertex3d(temp.right-.1, temp.top, 0);

					glTexCoord2f(1.0, 1.0);
					glVertex3d(temp.right-.1, temp.top, 4);

					glTexCoord2f(0.0, 1.0);
					glVertex3d(temp.right-.1, temp.bottom, 4);

					glTexCoord2f(0.0, 0.0);//side
					glVertex3d(temp.right + .1, temp.bottom, 0);

					glTexCoord2f(1.0, 0.0);
					glVertex3d(temp.right+.1, temp.top, 0);

					glTexCoord2f(1.0, 1.0);
					glVertex3d(temp.right+.1, temp.top, 4);

					glTexCoord2f(0.0, 1.0);
					glVertex3d(temp.right+.1, temp.bottom, 4);

				
					glTexCoord2f(0.05, 0);//top
					glVertex3d(temp.right - .1, temp.bottom, 4);

					glTexCoord2f(0.05, 1);
					glVertex3d(temp.right - .1, temp.top, 4);
					
					glTexCoord2f(-.05, 1);
					glVertex3d(temp.right + .1, temp.top, 4);
					

					glTexCoord2f(-.05, 0);
					glVertex3d(temp.right + .1, temp.bottom, 4);
				}

		for (int i = 0; i < noOfColumns; ++i)
			for (int j = 0; j < noOfRows - 1; ++j)
				if (!gridRelations->vertical[i][j]) {
					temp = getGraphicalRepresentionOfCell(j, i); //draw an edge on the bottom

					glTexCoord2f(0.0, 0.0);//side
					glVertex3d(temp.right, temp.bottom-.1, 0);

					glTexCoord2f(1.0, 0.0);
					glVertex3d(temp.left, temp.bottom-.1, 0);

					glTexCoord2f(1.0, 1.0);
					glVertex3d(temp.left, temp.bottom-.1, 4);

					glTexCoord2f(0.0, 1.0);
					glVertex3d(temp.right, temp.bottom -.1, 4);

			
					glTexCoord2f(0.0, 0.0);//side
					glVertex3d(temp.right, temp.bottom +.1, 0);

					glTexCoord2f(1.0, 0.0);
					glVertex3d(temp.left, temp.bottom+.1, 0);

					glTexCoord2f(1.0, 1.0);
					glVertex3d(temp.left, temp.bottom+.1, 4);

					glTexCoord2f(0.0, 1.0);
					glVertex3d(temp.right, temp.bottom+.1, 4);
				

					glTexCoord2f(0.05, 0);//top
					glVertex3d(temp.left, temp.bottom + .1, 4);

					glTexCoord2f(0.05, 1);
					glVertex3d(temp.right, temp.bottom + .1, 4);

					glTexCoord2f(-.05, 1);

					glVertex3d(temp.right, temp.bottom - .1, 4);

					glTexCoord2f(-.05, 0);
					glVertex3d(temp.left, temp.bottom - .1, 4);
				}

		glEnd();

		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// Draw ground
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.1);

		glTexCoord2f(0, 1);
		glVertex3f(noOfColumns*columnStep, 0, 0.1);


		glTexCoord2f(1, 1);
		glVertex3f(noOfColumns*columnStep, -noOfRows*rowStep, 0.1);


		glTexCoord2f(1, 0);
		glVertex3f(0, -noOfRows*rowStep, 0.1);

		glEnd();

		glEndList();
}






