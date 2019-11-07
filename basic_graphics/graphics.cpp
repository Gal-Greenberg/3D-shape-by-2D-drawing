#include "GLUT.H"
#include <stdio.h>
#include <math.h>
#include <time.h>

const int WIDTH = 800;
const int HEIGHT = 400;

const double PI = 3.14;

typedef struct Point
{
	double x;
	double y;
};

Point* points;
int numPoints = 0;
double offset = 0;

void init()
{
	glClearColor(0.4, 0.4, 0.4, 0); // set background color

	glEnable(GL_DEPTH_TEST); // show the nearest object
}

// n is a number of sides
void DrawCylinder1(int n, double topr, double bottomr)
{
	double alpha, teta = 2 * PI / n;

	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), bottomr*(bottomr - topr), cos(alpha)); // normal to surface

		glColor3d(fabs(sin(alpha)), 1 - fabs(cos(alpha)), fabs(cos(alpha)));
		glVertex3d(topr*sin(alpha), 1, topr*cos(alpha)); // 1
		glColor3d(fabs(sin(alpha + teta)), 1 - fabs(cos(alpha + teta)), fabs(cos(alpha + teta)));
		glVertex3d(topr*sin(alpha + teta), 1, topr*cos(alpha + teta)); // 2
		glColor3d(0.7*fabs(sin(alpha + teta)), 1 - 0.5*fabs(cos(alpha + teta)), 1 - 0.5*fabs(sin(alpha + teta)));
		glVertex3d(bottomr*sin(alpha + teta), 0, bottomr*cos(alpha + teta)); // 3
		glColor3d(0.7*fabs(sin(alpha)), 1 - 0.5* fabs(cos(alpha)), 1 - 0.5*fabs(sin(alpha)));
		glVertex3d(bottomr*sin(alpha), 0, bottomr*cos(alpha)); // 4
		glEnd();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clean frame buffer and Z-Buffer

	//left view
	glViewport(0, 0, WIDTH / 2, HEIGHT);
	glMatrixMode(GL_PROJECTION); // start defining the projection matrix
	glLoadIdentity();
	// left, right, bottom, top , near, far
	glFrustum(-1, 1, -1, 1, 0.8, 250);
	gluLookAt(0, 4, 10, // eye coordinates
		0, 3, 0,  // Point of interest
		0, 1, 0);// UP
				 // go back to "MODEL-VIEW" matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double h = 0.0;

	for (int i = 1; i < numPoints; i++)
	{
		glPushMatrix();
		h += (points[i - 1].y - points[i].y);
		glRotated(offset, 0, 1, 0);
		glTranslated(0, -h, 0);
		glScaled(4, points[i - 1].y - points[i].y, 4);
		DrawCylinder1(40, points[i - 1].x, points[i].x);
		glPopMatrix();
	}

	//right view
	glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
	glMatrixMode(GL_PROJECTION); // start defining the projection matrix
	glLoadIdentity();
	// left, right, bottom, top , near, far
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (int i = 0; i < numPoints; i++)
	{
		glPointSize(5);
		glColor3d(1, 0, 0);
		glBegin(GL_POINTS);
		glVertex3d(points[i].x, points[i].y, 0);
		glVertex3d(-points[i].x, points[i].y, 0);
		glEnd();
	}

	glLineWidth(2);
	glColor3d(0.8, 0.8, 0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numPoints && numPoints > 1; i += 1)
	{
		glVertex3d(points[i].x, points[i].y, 0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numPoints && numPoints > 1; i += 1)
	{
		glVertex3d(-points[i].x, points[i].y, 0);
	}
	glEnd();

	glLineWidth(2);
	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3d(0, -1, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	glColor3d(0.6, 0.4, 0.8);
	glBegin(GL_POLYGON);
	glVertex3d(-1, -1, 0);
	glVertex3d(-1, 1, 0);
	glVertex3d(1, 1, 0);
	glVertex3d(1, -1, 0);
	glEnd();

	glutSwapBuffers();
}

void idle()
{
	offset += 0.3;
	glutPostRedisplay(); // call to refresh window
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (600 <= x && x <= 800)
		{
			points = (Point*)realloc(points, (numPoints + 1) * sizeof(Point));
			points[numPoints].x = ((double)x / 200) - 3;
			points[numPoints].y = -((double)y / 200) + 1;
			numPoints++;
		}
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	// add 2 frame buffers and Z-Buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Basic graphics app");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}