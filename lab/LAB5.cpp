/***C/C++  http://bbs.csdn.net/topics/100091584***/
#define PI 3.14159265
#include <stdio.h> 
#include <stdlib.h>
#include<Math.h>
#include "iostream"
/***freeglut***/
#include "C:\Opengllib\freeglut\include\GL\freeglut.h"
void buildPopupMenu();
void menuSelect(int);
void ChangeSize(int, int);
void RenderScene(void);		
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialkey(int, int, int);
void My_Mouse(int, int, int, int);
void RotateMartix(float, GLfloat, GLfloat);

float tx = 0, ty = 0, tz = 0, sx = 1, sy = 1, sz = 1, mr = 0, size = 21,step=0,flag=0;
float thetaX = 0, thetaY = 0, thetaZ = 0;
float pointx=1, pointy,pointz;
float fillx=1,filly = 1;

int main(int argc, char** argv) 
{
	

   glutInit(&argc, argv); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
   glutInitWindowSize(400,400);				 
   glutInitWindowPosition(600,80);            
   glutCreateWindow("Moving cube");        
   SetupRC();
   buildPopupMenu();
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
void buildPopupMenu()
{
	int menu_id;
	menu_id = glutCreateMenu(menuSelect);
	glutAddMenuEntry("10", 0);
	glutAddMenuEntry("15", 1);
	glutAddMenuEntry("20", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
void menuSelect(int option)
{
	switch (option)
	{
	case 0:
		flag = 0;
		size = 21;
		std::cout << size << std::endl;
		glutPostRedisplay();
		break;
	case 1:
		flag = 0;
		size = 31;
		std::cout << size << std::endl;
		glutPostRedisplay();

		break;
	case 2:
		flag = 0;
		size = 41;
		std::cout << size << std::endl;
		glutPostRedisplay();

		break;
	default:
		break;
	}
}
void RenderScene(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(10.0f, 10.0f, 10.0f, 0, 0, 0, 0, 1, 0);


	/*glLineWidth(1.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(size, size);
	glVertex2f(size + 1, size);
	glVertex2f(size + 1, size + 1);
	glVertex2f(size, size + 1);
	glEnd();*/
	step = 20.0 / size;
	for (float i = -10; i <= 10; i+=step)
	{
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-10, i, 0);
		glVertex3f(10, i, 0);
		glEnd();

	}
	for (float i = -10; i <= 10; i+=step)
	{
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(i, -10, 0);
		glVertex3f(i, 10, 0);
		glEnd();
	}
	if (flag == 1) {
		glColor3f(0.0f, 1.0f, 1.0f);
		glRectf(fillx, filly, fillx + step, filly + step);
		glFlush();
	}
	


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
			flag = 1;
			//std::cout << "x:"<<x<<"y:"<<y << std::endl;
			double left = -10, width = 400,top=10,height=400,cnear=-10,cfar=20;
			//pointx = x;
			//y = height - y;
			
			pointx = ((x ) / width) * 20 + (-10);

			pointy = -((y) / height) * 20 + 10;
			pointz = (cnear + cfar) / (cnear - cfar);
			//std::cout << "x:" << pointx << "y:" << pointy << std::endl;
			int indexx = 0,indexy = 0;
			int temp= -((size - 1) / 2);
			for (float i = -10; i <= 10; i += step)
			{
				if (pointx>i&&pointx<(i+step)) {
					fillx = i;
					indexx = temp;
				}
				if (pointy > i && pointy <( i + step)) {
					filly = i;
					indexy = temp;
				}
				temp++;
			}
			std::cout << "x:" << indexx << "y:" << indexy << std::endl;
		}
		break;

	default:
		break;
	}
}

/*void drawgridx(int size, int y) {
	glLineWidth(1.0);
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(-size, y, 0);
	glVertex3f(size, y, 0);
	glEnd();
}*/




