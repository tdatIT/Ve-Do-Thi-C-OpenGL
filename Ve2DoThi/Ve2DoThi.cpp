#define CTRL_COUNT 100

#include<iostream>
#include<fstream>
#include"glut.h"
#include<cmath>

using namespace std;
const float PI = atan(1) * 4;
struct point
{
	float x, y;
	//Luu toa do cac diem
};
int A[100][100], n;
point P[100];
// Khai bao bien ve duong cong
int ctrlPointsCount;
double ctrlPointsX[CTRL_COUNT], ctrlPointsY[CTRL_COUNT];

void input(int A[][100], int &n)
{
	//Doc file dau vao
	ifstream fileIn("Input.txt");
	if (fileIn.fail())
	{
		cout << "Khong doc duoc file!!!";
		exit;
	}
	fileIn >> n;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fileIn >> A[i][j];
		}
	}
	cout << "Doc file thanh cong\n";
	fileIn.close();
}
void setPointPosition(point P[], int n)
{
	P[0].x = 0;
	P[0].y = 0.6;
	float angle = float(360) / n;
	float temp = angle*(PI / float(180));
	for (int i = 1; i < n; i++)
	{
		temp = angle*(PI / float(180))*i;

		if (i % 2 == 0)
		{
			P[i].x = 0.6*sin(temp)+0.07;
			P[i].y = 0.6*cos(temp)+0.05;
		}
		else
		{
			P[i].x = 0.6*sin(temp);
			P[i].y = 0.6*cos(temp);
		}
		

	}
}
void drawCircle(float cx, float cy, float cR)
{

	GLfloat x = cx, y = cy, R = cR;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
		glVertex3d(R*cos(2 * PI*i / 360) + x, R*sin(2 * PI*i / 360) + y, 0.0);
	//glVertex3f(R*cos(i) + x,R*sin(i) +y ,0.0);
	glEnd();
	/*glBegin(GL_LINE_LOOP);
	glVertex3f(cx - 0.1, cy + 0.1, 0.0);
	glVertex3f(cx + 0.1, cy + 0.1, 0.0);
	glVertex3f(cx + 0.1, cy - 0.1, 0.0);
	glVertex3f(cx - 0.1, cy - 0.1, 0.0);
	glEnd();*/
}
float getNextBezierPointX(float t)
{
	float x = 0.0;

	for (int i = 0; i<ctrlPointsCount; i++)
	{
		int c;
		if (i == 0 || i == ctrlPointsCount - 1)
			c = 1;
		else
		{
			c = ctrlPointsCount - 1;
		}
		x += c * pow(t, i) * pow(1 - t, ctrlPointsCount - 1 - i) * ctrlPointsX[i];
	}

	return x;
}
float getNextBezierPointY(float t)
{
	float y = 0.0;
	for (int i = 0; i<ctrlPointsCount; i++)
	{
		int c;
		if (i == 0 || i == ctrlPointsCount - 1)
			c = 1;
		else
		{
			c = ctrlPointsCount - 1;
		}
		y += c * pow(t, i) * pow(1 - t, ctrlPointsCount - 1 - i) *        ctrlPointsY[i];
	}
	return y;
}
void findPoint(float x1, float y1, float x2, float y2)
{
	point vectoAB{ vectoAB.x = (x2 - x1),vectoAB.y = (y2 - y1) };
	point I{ I.x = ((x1 + x2) / 2) ,I.y = ((y1 + y2) / 2) };
	float bienX = I.x + 0.05;
	float bienY = -(vectoAB.x / vectoAB.y)*bienX + ((vectoAB.x*I.x + vectoAB.y + I.y) / vectoAB.y);
	ctrlPointsX[0] = x1;
	ctrlPointsY[0] = y1;
	ctrlPointsX[2] = x2;
	ctrlPointsY[2] = y2;
	ctrlPointsX[1] = bienX;
	ctrlPointsY[1] = bienY;
}
void drawCurve(float x1, float y1, float x2, float y2)
{
	ctrlPointsCount = 3;
	if (x1 != x2)
		findPoint(x1, y1, x2, y2);
	else
	{
		ctrlPointsX[0] = x1;
		ctrlPointsY[0] = y1;
		ctrlPointsX[2] = x2;
		ctrlPointsY[2] = y2;
		ctrlPointsX[1] = (x1 + x2) / 2;
		ctrlPointsY[1] = ((y1 + y2) / 2) + 0.1;
	}

	float oldX = ctrlPointsX[0], oldY = ctrlPointsY[0];
	for (double t = 0.0; t <= 1.0; t += 0.01)
	{
		float x = getNextBezierPointX(t);
		float y = getNextBezierPointY(t);
		//glColor3f(1.0,t,1.0);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(oldX, oldY);
		glVertex2f(x, y);
		glEnd();
		glFlush();
		oldX = x;
		oldY = y;
	}
}
void drawPoint()
{
	glColor3f(1.0, 0.0, 0.0);
	char namePoint[100] = { '0','1','2','3','4','5','6','7','8','9' };

	for (int i = 0; i < n; i++)
	{

		drawCircle(P[i].x, P[i].y, 0.05);
		glRasterPos2f(P[i].x + 0.05, P[i].y + 0.05);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, namePoint[i]);

	}
}
void drawCurveUnder(float x1, float y1, float x2, float y2)
{
	ctrlPointsCount = 3;
	point vectoAB{ vectoAB.x = (x2 - x1),vectoAB.y = (y2 - y1) };
	point I{ I.x = ((x1 + x2) / 2) ,I.y = ((y1 + y2) / 2) };
	float bienY = I.y - 0.05;
	float bienX = -( vectoAB.y/vectoAB.x)*bienY + ((vectoAB.x*I.x + vectoAB.y + I.y) / vectoAB.x);
	ctrlPointsX[0] = x1;
	ctrlPointsY[0] = y1;
	ctrlPointsX[2] = x2;
	ctrlPointsY[2] = y2;
	ctrlPointsX[1] = bienX;
	ctrlPointsY[1] = bienY;

	float oldX = ctrlPointsX[0], oldY = ctrlPointsY[0];
	for (double t = 0.0; t <= 1.0; t += 0.01)
	{
		float x = getNextBezierPointX(t);
		float y = getNextBezierPointY(t);
		//glColor3f(1.0,t,1.0);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glVertex2f(oldX, oldY);
		glVertex2f(x, y);
		glEnd();
		glFlush();
		oldX = x;
		oldY = y;
	}
}
void drawLineConnect()
{
	glColor3f(0, 250.0, 154.0);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (A[i][j] != 0 )
			{
				glBegin(GL_LINES);
				glVertex3d(P[i].x, P[i].y, 0);
				glVertex3d(P[j].x, P[j].y, 0);
				glEnd();
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (A[i][i] == 1)
		{
			glColor3f(255.0, 255.0, 255.0);
			drawCircle(P[i].x, P[i].y + 0.1, 0.1);
		}
	}


}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//Khoi tao mau nen cho man hinh
}
void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT);//Xoa cac pixel cua man hinh lam viec
	drawPoint();
	drawLineConnect();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (A[i][j] == 2 )
				drawCurveUnder(P[j].x, P[j].y, P[i].x, P[i].y);
			if (A[i][j] == 3 )
			{
				drawCurve(P[j].x, P[j].y, P[i].x, P[i].y);
				drawCurveUnder(P[j].x, P[j].y, P[i].x, P[i].y);
			}
				
		}
	}
	
	glFlush();
}


int main(int argc, char ** argv)
{

	input(A, n);
	setPointPosition(P, n);
	cout << "Nhap phim bat ky de ve!!!\n";
	system("pause");
	//Doc file input thiet lap toa do cac diem tren man hin
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ve do thi!");
	//Khoi tao cua so Windows
	glutDisplayFunc(display);
	//Goi ham hien thi cong viec ve do thi
	init();
	glutMainLoop();
	return 0;
}