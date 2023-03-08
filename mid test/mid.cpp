/***C/C++  http://bbs.csdn.net/topics/100091584***/
#define PI 3.14159265
#include <stdio.h> 
#include <stdlib.h>
#include<Math.h>
#include "iostream"
#include <vector>
#include <tuple>
#include <time.h>
/***freeglut***/
#include "C:\Opengllib\freeglut\include\GL\freeglut.h"
class vertex {
public:
	float x, y, r, g, b;
	vertex(int _x, int _y, float _r, float _g, float _b) :x(_x), y(_y), r(_r), g(_g), b(_b) {}
	vertex() {
		x = 0;
		y = 0;
		r = 0;
		g = 0;
		b = 0;
	}
};
void buildPopupMenu();
void menuSelect(int);
void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialkey(int, int, int);
void My_Mouse(int, int, int, int);
void RotateMartix(float, GLfloat, GLfloat);
void setPixel(int , int , float );
void midpoint(vertex v0, vertex v1);
void fillgrid(int,float);
void anti_aliasing(int, int, int, int, float);
void anti_aliasing_setPixel(int, int, float, float);
void crow(std::vector<vertex> );
void scanY(std::vector<vertex>, int);
void scanX(vertex* , vertex* , int );
void differenceY(vertex*, vertex*, vertex*, vertex*, int);
void difference(vertex*, vertex*, vertex*, vertex*, float, float);
void differenceX(vertex*, vertex*, vertex*, vertex*, int);
void increment(vertex*, vertex* );
float  size = 21, step = 0;
float pointx = 1, pointy, pointz;
float fillx = 1, filly = 1;
int p0_x = 0, p0_y = 0, p1_x = 5, p1_y = 0, antiflag = 0;
//點擊狀況
int flag = 0;
//存頂點vector
std::vector<vertex> vList;
// 是否選用crow演算法
int crowflag = 0;
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving cube");
	SetupRC();
	buildPopupMenu();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(My_Mouse);
	//glutSpecialFunc(mySpecialkey);



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
	glutAddMenuEntry("Rasterize  line", 3);
	glutAddMenuEntry("Rasterize  polygon", 4);
	glutAddMenuEntry("clear", 5);
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
	case 3:
		//antiflag = 0;
		//size = 41;

		glutPostRedisplay();

		break;
	case 4:
		crowflag = 1;
		glutPostRedisplay();

		break;
	case 5:
		glColor3f(0.0f, 0.0f, 0.0f);
		crowflag = false;
		if (vList.size()) {
			vList.clear();
		}
		
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void RenderScene(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	//繪製格子
	for (float i = -10; i <= 10; i += step)
	{
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-10, i, 0);
		glVertex3f(10, i, 0);
		glEnd();

	}
	for (float i = -10; i <= 10; i += step)
	{
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(i, -10, 0);
		glVertex3f(i, 10, 0);
		glEnd();
	}
	//
	if (crowflag == 0) {
		if (vList.size() == 1) {
			glColor3f(vList[0].r, vList[0].g, vList[0].b);
			setPixel(vList[0].x, vList[0].y, step);
		}
		else if (vList.size() > 1) {
			for (int i = 0; i < vList.size(); i++) {
				if (i < vList.size() - 1) {
					glColor3f(vList[i].r, vList[i].g, vList[i].b);
					midpoint(vList[i], vList[i + 1]);
				}
			}
		}
	}
	else if (crowflag == 1) {
		if (vList.size() >= 3) {
			crow(vList);
		}
		else {
			for (int i = 0; i < vList.size(); i++)
			{
				glColor3f(vList[i].r, vList[i].g, vList[i].b);
				setPixel(vList[i].x, vList[i].y,step);
			}
		}
	}
	

	glutSwapBuffers();
}
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
		
		break;

	default:
		break;

	}
	glutPostRedisplay();
}


void My_Mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			flag = 1;
			//std::cout << "x:"<<x<<"y:"<<y << std::endl;
			double left = -10, width = 400, top = 10, height = 400, cnear = -10, cfar = 20;
			//pointx = x;
			//y = height - y;

			pointx = ((x) / width) * (double)20 + (double)(-10);

			pointy = -((y) / height) * (double)20 + (double)10;
			pointz = (cnear + cfar) / (cnear - cfar);
			//std::cout << "x:" << pointx << "y:" << pointy << std::endl;
			step = 20.0 / size;
			fillgrid(size, step);
		}
		break;
	
	default:
		break;
	}
}
void setPixel(int x, int y, float step) {
	float point_x = (x * step - step / 2.0);
	float point_y = (y * step - step / 2.0);

	glRectf(point_x, point_y, point_x + step, point_y + step);
	glFlush();
}
void fillgrid(int size, float step)
{
	int temp = -((size - 1) / 2);
	int x = 0, y = 0;
	for (float i = -10; i <= 10; i += step) {
		if ((pointx > (i)) && (pointx < i + step)) {
			x = temp;
			p1_x = temp;
			//fillx = i;
		}
		if ((pointy > (i)) && (pointy < i + step)) {
			y = temp;
			p1_y = temp;
			//filly = i;
		}
		temp++;
	}
	//使用亂數設置顏色
	srand((unsigned) time(NULL));
	float min = 0;
	float max = 1.0;
	float r = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	float g = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	float b = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	vertex v((float)x, (float)y, r, g, b);
	vList.push_back(v);
	std::cout << "r:" << r << "g:" << g << "b:" << b << std:: endl;
	std::cout << "(" << x << "," << y << ")" <<std:: endl;
}
void midpoint(vertex v0, vertex v1) {
	int x0, x1, y0, y1;
	float r0, r1, g0, g1, b0, b1;
	x0 = (int)v0.x;
	x1= (int)v1.x;
	y0 = (int)v0.y;
	y1 = (int)v1.y;
	r0 = v0.r;
	r1 = v1.r;
	g0 = v0.g;
	g1 = v1.b;
	b0 = v0.b;
	b1 = v1.b;
	if (x1 < x0) {
		int temp;
		float colorstore;
		//前後交換
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
		//顏色前後交換
		colorstore = r0;
		r0 = r1;
		r1 = colorstore;
		colorstore = g0;
		g0 = g1;
		g1 = colorstore;
		colorstore = b0;
		b0 = b1;
		b1 = colorstore;

	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float m = 0;
	if (y1 == y0) {
		std::cout << "m=0" <<std:: endl;

	}
	else if (x1 == x0) {
		std::cout << "m=inf" <<std:: endl;
	}
	else {
		m = (float)dy / (float)dx;
		std::cout << "m=" << m << std::endl;
	}
	if (y1 == y0) {
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);
		setPixel(x, y, step);
		//紀錄開始地方用於改顏色
		int color_start_x = x0;
		while (x < x1) {
			x += 1;
			//顏色內插
			float r = r0 + dr * (x - color_start_x);
			float g = g0 + dg * (x - color_start_x);
			float b = b0 + db * (x - color_start_x);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
		while (x > x1) {
			x -= 1;
			float r = r0 + dr * (color_start_x -x);
			float g = g0 + dg * (color_start_x - x);
			float b = b0 + db * (color_start_x - x);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}

	}
	else if (x1 == x0) {
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);
		setPixel(x, y, step);
		int color_start_y = y0;
		while (y < y1) {
			y += 1;
			float r = r0 + dr * (y - color_start_y);
			float g = g0 + dg * (y - color_start_y);
			float b = b0 + db * (y - color_start_y);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
		while (y > y1) {
			y -= 1;
			float r = r0 + dr * (color_start_y - y);
			float g = g0 + dg * (color_start_y - y);
			float b = b0 + db * (color_start_y - y);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
	}
	else if (m > 0 && m <= 1) {
		int d = 2 * dy - dx;
		int delE = 2 * dy;
		int delNE = 2 * (dy - dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);
		setPixel(x, y, step);
		int color_start_x = x0;
		while (x < x1) {
			if (d <=0) {
				d += delE;
				x++;
			}
			else {
				d += delNE;
				x++;
				y++;
			}
			float r = r0 + dr * (x - color_start_x);
			float g = g0 + dg * (x - color_start_x);
			float b = b0 + db * (x - color_start_x);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
	}
	else if (m>1) {
		int d =  dy -2* dx;
		int delE = -2 * dx;
		int delNE = 2 * (dy - dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);
		setPixel(x, y, step);
		int color_start_y = y0;
		while (y < y1) {
			if (d > 0) {
				d += delE;
				y++;
			}
			else {
				d += delNE;
				x++;
				y++;
			}
			float r = r0 + dr * (y - color_start_y);
			float g = g0 + dg * (y - color_start_y);
			float b = b0 + db * (y - color_start_y);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
	}
	else if (m<0 &&m >= -1) {
		int d = 2*dy + dx;
		int delE = 2 * dy;
		int delNE = 2 * (dy + dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);
		int color_start_x = x0;
		setPixel(x, y, step);
		while (x < x1) {
			if (d > 0) {
				d += delE;
				x++;
			}
			else {
				d += delNE;
				x++;
				y--;
			}
			float r = r0 + dr * (x - color_start_x);
			float g = g0 + dg * (x - color_start_x);
			float b = b0 + db * (x - color_start_x);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
	}
	else if (m <-1 ) {
		int d = dy + 2 * dx;
		int delE = 2 * dx;
		int delNE = 2 * (dy +dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);
		int color_start_y = y0;
		setPixel(x, y, step);
		while (y > y1) {
			if (d <= 0) {
				d += delE;
				y--;
			}
			else {
				d += delNE;
				x++;
				y--;
			}
			float r = r0 + dr * (y - color_start_y);
			float g = g0 + dg * (y - color_start_y);
			float b = b0 + db * (y - color_start_y);
			glColor3f(r, g, b);
			setPixel(x, y, step);
		}
	}

}
void anti_aliasing(int x0, int y0, int x1, int y1, float step) {
	if (x1 < x0) {
		int temp;
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float m = 0;
	if (y1 == y0) {
		std::cout << "m=0" << std::endl;

	}
	else if (x1 == x0) {
		std::cout << "m=inf" << std::endl;
	}
	else {
		m = (float)dy / (float)dx;
		std::cout << "m=" << m << std::endl;
	}
	if (y1 == y0) {
		int x = x0, y = y0;
		anti_aliasing_setPixel(x, y, 0, step);
		while (x < x1) {
			x += 1;
			anti_aliasing_setPixel(x, y, 0, step);
		}
		while (x > x1) {
			x -= 1;
			anti_aliasing_setPixel(x, y, 0, step);
		}

	}
	else if (x1 == x0) {
		int x = x0, y = y0;
		anti_aliasing_setPixel(x, y, 0, step);
		while (y < y1) {
			y += 1;
			anti_aliasing_setPixel(x, y, 0, step);
		}
		while (y > y1) {
			y -= 1;
			anti_aliasing_setPixel(x, y, 0, step);
		}
	}
	else if (m > 0 && m <= 1) {
		float x = x0, y = y0;
		while (x <= x1) {
			int yi = floor(y);
			float f = y - yi;
			anti_aliasing_setPixel(x, yi, 1-f, step);
			anti_aliasing_setPixel(x, yi+1,  f, step);
			y += m;
			x++;
		}
	}
	else if (m > 1) {
	
		float x = x0, y = y0;
		float nm = 1 / m;
		while (y <=y1) {
			int xi = floor(x);
			float f = x - xi;
			anti_aliasing_setPixel(xi, y, 1 - f, step);
			anti_aliasing_setPixel(xi+1, y, f, step);
			
			x += nm;
			y++;
		}
	}
	else if (m < 0 && m >= -1) {
		float x = x0, y = y0;
		while (x <= x1) {
			int yi = floor(y);
			float f = y - yi;
			anti_aliasing_setPixel(x, yi, 1 - f, step);
			anti_aliasing_setPixel(x, yi + 1, f, step);
			y += m;
			x++;
		
		}
	}
	else if (m < -1) {
		float x = x0, y = y0;
		float nm = 1 / m;
		while (y >= y1) {
			int xi = floor(x);
			float f = x - xi;
			anti_aliasing_setPixel(xi, y, 1 - f, step);
			anti_aliasing_setPixel(xi + 1, y, f, step);
			
			x -= nm;
			y--;
		}
	}

}
void crow(std::vector<vertex> cvlist) {
	int iMin = 0;
	
	/*if (cvlist[i].y < cvlist[0].y) {
		iMin = i;
	}*/
	for (int i = 0; i < cvlist.size(); i++) {
		
		 if (cvlist[i].y < cvlist[iMin].y) {
			iMin = i;
		}
	}

	scanY(cvlist, iMin);
}
void scanY(std::vector<vertex> v, int i) {
	int li, ri; // 左右的 indices
	int ly, ry; // 左右的 y 值
	vertex l, dl; // 現在的 左邊界 and delta
	vertex r, dr; // 現在的 右邊界 and delta
	int rem; // 剩下的頂點
	int y; // 掃描線的值

	int n = v.size();
	li = ri = i;
	ly = ry = y = (int)ceil(v[i].y);
	for (rem = n; rem > 0;) {
		//找左邊界
		while (ly <= y && rem > 0) {
			rem--;
			i = li - 1;
			if (i < 0){
				i = n - 1; // go clockwise
			}
			ly = (int)ceil(v[i].y);
			if (ly > y) { // replace left edge
				differenceY(&v[li], &v[i], &l, &dl, y);
			}
			li = i; // index of the left endpoint
		}
		while (ry <= y && rem > 0) {
			rem--;
			i = ri + 1;
			if (i >= n) {
				i = 0;
			}
			ry = (int)ceil(v[i].y);
			if (ry > y) {
				differenceY(&v[ri], &v[i], &r, &dr, y);
			}
			ri = i;
		}

		for (; y <= ly && y <= ry; y++) {

			scanX(&l, &r, y);
			increment(&l, &dl);
			increment(&r, &dr);
		}
	}
}

/*           */
void scanX(vertex* l, vertex* r, int y) {
	
	int x, lx, rx;
	vertex s, ds;
	lx = ceil(l->x);
	rx = ceil(r->x);
	if (lx <= rx) {
		differenceX(l, r, &s, &ds, lx);
		for (x = lx; x < rx; x++) {
			if (x == lx) {
				glColor3f(l->r, l->g, l->b);
				setPixel(x, y,step);
			}
			else
			{
				increment(&s, &ds);
				setPixel(x, y, step);
			}
			
		}
	}
	else
	{
		differenceX(r, l, &s, &ds, rx);
		for (x = rx; x < lx; x++) {
			if (x == rx) {
				glColor3f(r->r, r->g, r->b);
				setPixel(x, y, step);
			}
			else
			{
				increment(&s, &ds);
				setPixel(x, y, step);
			}

		}
	}

}
void differenceY(vertex* v1, vertex* v2, vertex* e, vertex* de, int y) {
	float f = y - v1->y;
	float d = v2->y - v1->y;
	difference(v1, v2, e, de, d, f);

}
void differenceX(vertex* v1, vertex* v2, vertex* e, vertex* de, int x) {
	float f = x - v1->x;
	float d = v2->x - v1->x;
	difference(v1, v2, e, de, d, f);


}
void difference(vertex* v1, vertex* v2, vertex* e, vertex* de, float d, float f) {
	de->x = (v2->x - v1->x) / d;//計算Y每次增加X增加多少
	e->x = v1->x + f * de->x;//補償X
	//對顏色進行補償
	de->r = (v2->r - v1->r) / d;
	e->r = (v1->r) + f * (de->r);
	de->g = (v2->g - v1->g) / d;
	e->g = v1->g + f * (de->g);
	de->b = (v2->b - v1->b) / d;
	e->b = v1->b + f * (de->b);

}

void increment(vertex* v, vertex* dv) {
	v->x += dv->x;
	v->r += dv->r;
	v->g += dv->g;
	v->b += dv->b;
	glColor3f(v->r, v->g, v->b);
}
void anti_aliasing_setPixel(int x, int y, float c,float step) {
	glColor3f(c, c, c);
	float point_x = (x * step - step / 2.0);
	float point_y = (y * step - step / 2.0);

	glRectf(point_x, point_y, point_x + step, point_y + step);
	glFlush();
}



