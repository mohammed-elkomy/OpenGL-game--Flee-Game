#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Board.h"
#include "UI.h"
#include <gl/glut.h>
#include <conio.h>
#include <windows.h>

int main(int iArgc, char** cppArgv) {
	srand(time(NULL));
	int n = 30, m = 20;

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(200, 120);
	glutCreateWindow("KOKO GRAPHICS");

	initGrid(n, m);
	init(n, m);

	glutDisplayFunc(Draw);
	glutMainLoop();
	delete grid;//free resources
	return 0;
}
