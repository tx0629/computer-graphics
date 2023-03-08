/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
/***freeglut***/
#include "C:\Opengllib\freeglut\include\GL\freeglut.h"
#define X .525731112119133606
#define Z .850650808352039932
static GLfloat vdata[12][3] = {
{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
{1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
{1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
{3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
{10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
};
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_shininess = { 20.0 };
void icoNormVec(int);
void ChangeSize(int, int);
void RenderScene(void);		
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialkey(int, int, int);
void normCrossProd(GLfloat u[3], GLfloat v[3], GLfloat out[3]);
void normalize(float v[3]);
void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth);
void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]);
void buildPopupMenu();
void display(void);
float tx = 0, ty = 0, tz = 0;
float thetaX = 0, thetaY = 0, thetaZ = 0;
GLfloat d1[3], d2[3], n[3];
int depth = 3,fill=1;
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
   glutSpecialFunc(mySpecialkey);
   buildPopupMenu();

    

   glutMainLoop();	
   return 0; 
} 
void SetupRC() {
	GLfloat whitelight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourcelight[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightpos[] = { 0.f,25.0f,20.0f,0.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	//glShadeModel(GL_FLAT); /*enable flat shading */
	glShadeModel(GL_SMOOTH); /*enableSMOOTH shading */
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
	glOrtho(-1, 1, -1, 1, -1, 2);
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
	glViewport(0, 0, 200, 200);
	glRotatef(thetaX, 0, 1, 0);
	glRotatef(thetaY, 1, 0, 0);
	if (fill == 0) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++) {
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++) {
			icoNormVec(i);
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
		}
		glEnd();
	}
	glViewport(0, 200, 200, 200);

	if (fill == 0) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++)
		{
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++)
		{
			glNormal3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glNormal3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glNormal3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
		}
		glEnd();
	}

	//display();
	glViewport(200, 200, 200, 200);

	display();

	glEnd();
	glutSwapBuffers();
}
void menuSelect(int option)
{
	switch (option)
	{
	case 0:
		fill = 0;
		glutPostRedisplay();
		break;
	case 1:
		fill = 1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void buildPopupMenu()
{
	int menu_id;
	menu_id = glutCreateMenu(menuSelect);
	glutAddMenuEntry("line", 0);
	glutAddMenuEntry("fill", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void icoNormVec(int i)
{
	for (int k = 0; k < 3; k++) {
		d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
		d2[k] = vdata[tindices[i][1]][k]- vdata[tindices[i][2]][k];
	}
	normCrossProd(d1, d2, n);
	glNormal3fv(n);
}
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{

	case 'a':
		if (depth <5) {
			depth++;
		}
		break;
	case 'd':
		if (depth > 0) {
			depth--;
		}
		break;

	default:
		break;
	}
	glutPostRedisplay();
}
void mySpecialkey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		thetaX += 3;
		break;
	case GLUT_KEY_RIGHT:
		thetaX -= 3;
		break;
	case GLUT_KEY_UP:
		thetaY += 3;
		break;
	case GLUT_KEY_DOWN:
		thetaY -= 3;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void normCrossProd(GLfloat u[3], GLfloat v[3], GLfloat out[3])
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);
}
void normalize(float v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; v[1] /= d; v[2] /= d;
}
void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], int depth) {
	GLfloat v12[3], v23[3], v31[3]; int i;
	if (depth == 0) {
		
	drawTriangle(v1, v2, v3); //Draw if no further subdivision requested
		return;
	}
	for (i = 0; i < 3; i++) {
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}
void display(void)
{
		for (int i = 0; i < 20; i++) {
			subdivide(&vdata[tindices[i][0]][0],
				&vdata[tindices[i][1]][0],
				&vdata[tindices[i][2]][0],
				depth);
		}
		glFlush();
}
void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]) {
	if (fill == 0) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++) {
		
			glVertex3fv(v1);
			glVertex3fv(v2);
			glVertex3fv(v2);
			glVertex3fv(v3);
			glVertex3fv(v3);
			glVertex3fv(v1);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++) {
			glNormal3fv(v1);
			glVertex3fv(v1);
			glNormal3fv(v2);
			glVertex3fv(v2);
			glNormal3fv(v3);
			glVertex3fv(v3);
		}
		glEnd();
	}
	
}




