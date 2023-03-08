/***C/C++  http://bbs.csdn.net/topics/100091584***/
#define PI 3.14159265
#include <stdio.h> 
#include <stdlib.h>
#include<Math.h>
#include "iostream"
/***freeglut***/
#include "C:\Opengllib\freeglut\include\GL\freeglut.h"
 
void ChangeSize(int, int);
void RenderScene(void);		
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialkey(int, int, int);
void My_Mouse(int, int, int, int);
void RotateMartix(float, GLfloat, GLfloat);
float tx = 0, ty = 0, tz = 0,sx=1,sy=1,sz=1,mr=0;
float thetaX = 0, thetaY = 0, thetaZ = 0;
float pointx=1, pointy,pointz;
GLfloat xrotMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };
GLfloat yrotMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };
GLfloat zrotMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };
GLfloat transMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };
GLfloat scMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };
GLfloat muMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

int main(int argc, char** argv) 
{
	

   glutInit(&argc, argv); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
   glutInitWindowSize(400,400);				 
   glutInitWindowPosition(600,80);            
   glutCreateWindow("Moving cube");        
   SetupRC();
   glutReshapeFunc(ChangeSize);
   glutDisplayFunc(RenderScene);
   glutKeyboardFunc(myKeyboard);
   glutMouseFunc(My_Mouse);
   glutSpecialFunc(mySpecialkey);

    

   glutMainLoop();	
   return 0; 
} 
void SetupRC() {
	GLfloat whitelight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourcelight[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightpos[] = { 0.f,25.0f,20.0f,0.0f };
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whitelight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourcelight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourcelight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
}
void ChangeSize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void RenderScene(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(10.0f, 10.0f, 10.0f, 0, 0, 0, 0, 1, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-50, 0, 0);
	glVertex3f(50, 0, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, -50, 0);
	glVertex3f(0, 50, 0);
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, -50);
	glVertex3f(0, 0, 50);
	glEnd();
	
	//glRotated(thetaX, 1, 0, 0);
	xrotMatrix[5] = cos(thetaX * PI / 180);
	xrotMatrix[6] = sin(thetaX * PI / 180);
	xrotMatrix[9] = -sin(thetaX * PI / 180);
	xrotMatrix[10] = cos(thetaX * PI / 180);
	glMultMatrixf(xrotMatrix);
	yrotMatrix[0] = cos(thetaY * PI / 180);
	yrotMatrix[2] = -sin(thetaY * PI / 180);
	yrotMatrix[8] = sin(thetaY * PI / 180);
	yrotMatrix[10] = cos(thetaY * PI / 180);
	glMultMatrixf(yrotMatrix);
	zrotMatrix[0] = cos(thetaZ * PI / 180);
	zrotMatrix[1] = sin(thetaZ * PI / 180);
	zrotMatrix[4] = -sin(thetaZ * PI / 180);
	zrotMatrix[5] =cos(thetaZ * PI / 180);
	glMultMatrixf(zrotMatrix);
	//glRotated(thetaY, 0, 1, 0);
	//glRotated(thetaZ, 0, 0, 1);
	//glTranslatef(tx, ty, tz);
	transMatrix[12] = tx;
	transMatrix[13] = ty;
	transMatrix[14] = tz;
	glMultMatrixf(transMatrix);
	scMatrix[0] = sx;
	scMatrix[5] = sy;
	scMatrix[10] = sz;
	glMultMatrixf(scMatrix);
	RotateMartix(mr, pointx, pointy);
	glColor3f(1, 1, 0);
	
	glutSolidCube(6);

	glPointSize(6);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(pointx, pointy,0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, 0,0);
	glVertex3f(pointx, pointy, 0);
	glEnd();
	glutSwapBuffers();
}
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r':
		thetaX = 0;
		thetaY = 0;
		thetaZ = 0;
		tx = 0;
		ty = 0;
		tz = 0;
		mr = 0;
		break;
	case 'a':
		thetaX += 3;
		break;
	case 'd':
		thetaX -= 3;
		break;
	case 'w':
		thetaY += 3;
		break;
	case 's':
		thetaY -= 3;
		break;
	case 'z':
		thetaZ += 3;
		break;
	case 'x':
		thetaZ -= 3;
		break;

	case 't':
		sx -= 1;
		break;
	case 'y':
		sx += 1;
		break;
	case 'g':
		sy += 1;
		break;
	case 'h':
		sy -= 1;
		break;
	case 'b':
		sz += 1;
		break;
	case 'n':
		sz -= 1;

		break;
	case 'o':
		mr -= 3;
		break;
	case 'p':
		mr += 3;
		//RotateMartix(mr, pointx, pointy);
	default:
		break;

	}
	glutPostRedisplay();
}

void mySpecialkey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		tx -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		tx += 0.1;
		break;
	case GLUT_KEY_UP:
		ty += 0.1;
		break;
	case GLUT_KEY_DOWN:
		ty -= 0.1;
		break;
	
	default:
		break;
	}
	glutPostRedisplay();
}
void My_Mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON :
		if (state == GLUT_DOWN) {
			std::cout << "x:"<<x<<"y:"<<y << std::endl;
			double left = -10, width = 400,top=10,height=400,cnear=-10,cfar=20;
			//pointx = x;
			//y = height - y;
			pointx = ((x ) / width) * 20 + (-10);

			pointy = -((y) / height) * 20 + 10;
			pointz = (cnear + cfar) / (cnear - cfar);
			std::cout << "x:" << pointx << "y:" << pointy << std::endl;
		}
	default:
		break;
	}
}
void RotateMartix(float angle, GLfloat x, GLfloat y) {
	GLfloat Cos = cos(angle * PI / 180);
	GLfloat Sin = sin(angle * PI / 180);
	double magnitude = sqrt(x * x + y * y);
	if (magnitude == 0)
	{
		throw "error";
	}
	x /= magnitude;
	y /= magnitude;
	muMatrix[0] = Cos + (1 - Cos) * x * x;
	muMatrix[1] = (1 - Cos) * y * x;
	muMatrix[2] =- Sin * y;
	muMatrix[4] = (1 - Cos) * y * x;
	muMatrix[5] = Cos+(1 - Cos) * y * y;
	muMatrix[6] = Sin * x;
	muMatrix[8] = Sin * y;
	muMatrix[9] = -Sin * x;
	muMatrix[10] = Cos;
	glMultMatrixf(muMatrix);
}




