#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"
#include <math.h>


bool textureMode = true;
bool lightMode = true;
void vipuklvver(double O[], double I[], double J[], double O1[], double I1[], double J1[]);
void vpukl2vver(double C[], double C1[]);
void tex2(const char* name, const int NumberOfTexId);
void povorot(double i, double* x, double* y);
void fig1(double A[], double B[], double C[], double D[], double E[], double F[], double G[]);
void vipukl(double O[], double I[], double J[], double O1[], double I1[], double J1[]);
void vpukl(double N[], double L[], double N1[], double L1[]);
void vpukl2(double C[], double C1[]);
void fig(double A[], double B[], double C[], double D[], double E[], double F[], double G[]);
void quad(double A[], double B[], double C[], double D[], double E[], double F[], double G[], double A1[], double B1[], double C1[], double D1[], double E1[], double F1[], double G1[]);
double* norm_coord(double A[], double B[], double C[]);
//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId[2];

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	tex2("texture.bmp",0);
	tex2("texture1.bmp", 1);
	

	

	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


void tex2(const char* name, const int NumberOfTexId)
{
	RGBTRIPLE* texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP(name, &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);



	//���������� �� ��� ��������
	glGenTextures(1, &texId[NumberOfTexId]);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId[NumberOfTexId]);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


}


void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(0.2,0.4,0.3,0.9);


	//��������� ���������
	/*GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };*/
	GLfloat amb[] = { 1, 1, 1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  


	//������ ��������� ���������� ��������
	double A[] = { 3, 0.5, 0 };
	double B[] = { -0.5, 3.5, 0 };
	double C[] = { -0.5, -1, 0 };
	double D[] = { -3.5, 1.5, 0 };
	double E[] = { -2.5, -3, 0 };
	double F[] = { 0.5, -2, 0 };
	double G[] = { 3,-3.5, 0 };

	double A1[] = { 3, 0.5, 1 };
	double B1[] = { -0.5, 3.5, 1 };
	double C1[] = { -0.5, -1, 1 };
	double D1[] = { -3.5, 1.5, 1 };
	double E1[] = { -2.5, -3, 1 };
	double F1[] = { 0.5, -2, 1 };
	double G1[] = { 3,-3.5, 1 };

	double H[] = { 1.25, 2, 0 };
	double I[] = { 1.75, -1.5, 0 };
	double J[] = { 1.7630231686098606577377038594076, -1.4846714474741025469174783727867, 0 };

	double I1[] = { 1.75, -1.5, 1 };
	double J1[] = { 1.7630231686098606577377038594076, -1.4846714474741025469174783727867, 1 };

	double O[] = { 0,0,0 };
	double O1[] = { 0,0, 1 };

	double M[] = { -2.25, -1.5 , 0 };
	double M1[] = { -2.25, -1.5 , 1 };

	double N[] = { 4.08929, -1.45238 , 0 };
	double L[] = { 4.101808538, -1.416639363 , 0 };

	double N1[] = { 4.08929, -1.45238 , 1 };
	double L1[] = { 4.101808538, -1.416639363 , 1 };



	fig(A, B, C, D, E, F, G);
	
	quad(A, B, C, D, E, F, G, A1, B1, C1, D1, E1, F1, G1);
	vipukl(O, I, J, O1, I1, J1);
	
	vpukl2(C, C1);
	vpukl(N, L, N1, L1);
	fig1(A1, B1, C1, D1, E1, F1, G1);
	vipuklvver(O, I, J, O1, I1, J1);
	vpukl2vver(C, C1);
	//����� ��������� ���������� ��������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}
void vpukl2(double C[], double C1[])
{
	double y2 = 0, x2;
	double F[] = { -0.6, -0.916, 0 };
	double F1[] = { -0.6, -0.916, 1 };
	for (double i = 1.5; i >= -3; i -= 0.0002)
	{
		x2 = sqrt(4.34 * 4.34 - (i + 1.54762) * (i + 1.54762)) - 6.58929;
		glBegin(GL_TRIANGLES);

		glColor3d(1, 0, 0);
		double X[] = { x2, i, 0 };
		double* N = norm_coord(C, F, X);
		for (int i = 0; i < 3; i++)
		{
			N[i] *= -1;
		}
		glNormal3dv(N);
		glTexCoord2d(0.320, 0.5195);
		glVertex3dv(C);
		glTexCoord2d((F[1] + 3.5) / 7.805, (3.555 - F[0]) / 7.805);
		glVertex3dv(F);
		//glVertex3d(-0.6, -0.916, 0);
		double x, y;
		x = (i + 3.5) / 7.805;
		y = (3.555 - x2) / 7.805;
		glTexCoord2d(x, y);
		glVertex3d(x2, i, 0);
		glEnd();
	}
	/*for (double i = 1.5; i >= -3; i -= 0.0002)
	{
		x2 = sqrt(4.34 * 4.34 - (i + 1.54762) * (i + 1.54762)) - 6.58929;
		glBegin(GL_TRIANGLES);

		glColor3d(1, 0, 0);
		double X[] = { x2, i, 1 };
		double* N = norm_coord(C, F, X);
		for (int i = 0; i < 3; i++)
		{
			N[i] *= -1;
		}
		glNormal3dv(N);
		glTexCoord2d(0.320, 0.5195);
		glVertex3dv(C1);
		glTexCoord2d((F[1] + 3.5) / 7.805, (3.555 - F[0]) / 7.805);
		glVertex3dv(F1);
		double x, y;
		x = (i + 3.5) / 7.805;
		y = (3.555 - x2) / 7.805;
		glTexCoord2d(x, y);
		glVertex3d(x2, i, 1);
		glEnd();
	}*/


}
void vpukl2vver(double C[], double C1[])
{
	double y2 = 0, x2;
	double F[] = { -0.6, -0.916, 0 };
	double F1[] = { -0.6, -0.916, 1 };
	
	for (double i = 1.5; i >= -3; i -= 0.0002)
	{
		x2 = sqrt(4.34 * 4.34 - (i + 1.54762) * (i + 1.54762)) - 6.58929;
		glBegin(GL_TRIANGLES);

		glColor4d(1, 0, 0,0);
		double X[] = { x2, i, 1 };
		double* N = norm_coord(C, F, X);
		for (int i = 0; i < 3; i++)
		{
			N[i] *= -1;
		}
		glNormal3dv(N);
		glTexCoord2d(0.320, 0.5195);
		glVertex3dv(C1);
		glTexCoord2d((F[1] + 3.5) / 7.805, (3.555 - F[0]) / 7.805);
		glVertex3dv(F1);
		double x, y;
		x = (i + 3.5) / 7.805;
		y = (3.555 - x2) / 7.805;
		glTexCoord2d(x, y);
		glVertex3d(x2, i, 1);
		glEnd();
	}


}
void vpukl(double N[], double L[], double N1[], double L1[])
{
	for (double i = 0; i <= 63.8; i += 0.1)
	{
		glPushMatrix();
		glTranslated(-6.58929, -1.54762, 0);
		glRotated(i, 0, 0, 1);

		glBegin(GL_QUADS);

		glColor3d(1, 0, 1);
		double* W = norm_coord(N, L, L1);
		for (int i = 0; i < 3; i++)
		{
			W[i] *= -1;
		}
		glNormal3dv(W);
		glVertex3dv(N);
		glVertex3dv(L);
		glVertex3dv(L1);
		glVertex3dv(N1);

		glEnd();

		glPopMatrix();




	}
}
void povorot(double i, double* x, double* y)
{
	/*
	*x = (*x) * cos((i * 3.14159265) / 180) - (*y) * sin((i * 3.14159265) / 180);
	*y = (*x) * sin((i * 3.14159265) / 180) + (*y) * cos((i * 3.14159265) / 180);*/
	for (double j = 0.5; j <= i; j += 0.5)
	{
		*x = *x * cos(0.00873) - *y * sin(0.00873);
		*y = *x * sin(0.00873) + *y * cos(0.00873);
	}
	*x += 1.25;
	*y += 2;
	double n = *x;
	*x = *y;
	*y = n;
	*x = (*x + 3.5) / 7.805;
	*y = (3.555 - *y) / 7.805;

}
void vipukl(double O[], double I[], double J[], double O1[], double I1[], double J1[])
{
	/*double x2 = 0.514455932418437;
	double y2 = 0.063033546622696;
	double y21 = 0.0694396965;
	double x0 = 0.7046;
	double y0 = 0.2632;
	double x1 = 0.512;
	double y1 = 0.0711;*/
	double x0 = 0.7046;
	double y0 = 0.2632;
	double x1 = I[0];
	double y1 = I[1];
	double x2 = J[0];
	double y2 = J[1];
	
	double x33 = 0.002777777777778;
	double x31 = 0;

	for (double i = 0; i <= 180; i += 0.5)
	{
		povorot(i, &x1, &y1);
		povorot(i, &x2, &y2);
		glPushMatrix();
		glTranslated(1.25, 2, 0);
		glRotated(i, 0, 0, 1);

		glBegin(GL_TRIANGLES);
		glColor3d(1, 0, 1);
		double* W = norm_coord(O, I, J);
		for (int i = 0; i < 3; i++)
		{
			W[i] *= -1;
		}
		glNormal3dv(W);
		glTexCoord2d(x0, y0);
		glVertex3dv(O);
		glTexCoord2d(x1, y1);
		glVertex3dv(I);
		glTexCoord2d(x2, y2);
		glVertex3dv(J);

		
		glEnd();

		glBegin(GL_QUADS);

		glColor3d(1, 0, 1);
		 W = norm_coord(I, J, J1);
		glNormal3dv(W);	
		glTexCoord2d(x31, 0);
		glVertex3dv(I);
		glTexCoord2d(x33, 0);
		glVertex3dv(J);
		glTexCoord2d(x33, 1);
		glVertex3dv(J1);
		glTexCoord2d(x31, 1);
		glVertex3dv(I1);

		glEnd();

		glPopMatrix();

		/*double x = x1 * cos((i * 3.14159265) / 180) - y1 * sin((i * 3.14159265) / 180);
		double y = x1 * sin(i * 3.14159265 / 180) + y1 * cos(i * 3.14159265 / 180);
		x1 = x;
		y1 = y;
		x = x2 * cos((i * 3.14159265) / 180) - y2 * sin((i * 3.14159265) / 180);
		y = x2 * sin((i * 3.14159265) / 180) + y2 * cos((i * 3.14159265) / 180);
		x2 = x;
		y2 = y;*/
		/*for (double j = 0.5; j <= i; j += 0.5)
		{
			x1 = x1 * cos(0.00873) - y1 * sin(0.00873);
			y1 = x1 * sin(0.00873) + y1 * cos(0.00873);
			x2 = x2 * cos(0.00873) - y2 * sin(0.00873);
			y2 = x2 * sin(0.00873) + y2 * cos(0.00873);
		}*/
		/*x1 = x1 * cos((i * 3.14159265) / 180) - y1 * sin((i * 3.14159265) / 180);
		y1 = x1 * sin(i * 3.14159265 / 180) + y1 * cos(i * 3.14159265 / 180);
		x2 = x2 * cos((i * 3.14159265) / 180) - y2 * sin((i * 3.14159265) / 180);
		y2 = x2 * sin((i * 3.14159265) / 180) + y2 * cos((i * 3.14159265) / 180);*/
		x31 += 0.002777777777778;
		x33 += 0.002777777777778;
		x1 = I[0];
		y1 = I[1];
		x2 = J[0];
		y2 = J[1];

	}
}
void vipuklvver(double O[], double I[], double J[], double O1[], double I1[], double J1[])
{
	/*double x2 = 0.514455932418437;
	double y2 = 0.063033546622696;
	double y21 = 0.0694396965;
	double x0 = 0.7046;
	double y0 = 0.2632;
	double x1 = 0.512;
	double y1 = 0.0711;*/
	double x0 = 0.7046;
	double y0 = 0.2632;
	double x1 = I[0];
	double y1 = I[1];
	double x2 = J[0];
	double y2 = J[1];

	double x33 = 0.002777777777778;
	double x31 = 0;

	for (double i = 0; i <= 180; i += 0.5)
	{
		povorot(i, &x1, &y1);
		povorot(i, &x2, &y2);
		glPushMatrix();
		glTranslated(1.25, 2, 0);
		glRotated(i, 0, 0, 1);

		glBegin(GL_TRIANGLES);
		glColor4d(1, 0, 1,0);
		

		double* W = norm_coord(O1, I1, J1);
		for (int i = 0; i < 3; i++)
		{
			W[i] *= -1;
		}
		glNormal3dv(W);
		glTexCoord2d(x0, y0);
		glVertex3dv(O1);
		glTexCoord2d(x1, y1);
		glVertex3dv(I1);
		glTexCoord2d(x2, y2);
		glVertex3dv(J1);

		glEnd();

		

		glPopMatrix();

		/*double x = x1 * cos((i * 3.14159265) / 180) - y1 * sin((i * 3.14159265) / 180);
		double y = x1 * sin(i * 3.14159265 / 180) + y1 * cos(i * 3.14159265 / 180);
		x1 = x;
		y1 = y;
		x = x2 * cos((i * 3.14159265) / 180) - y2 * sin((i * 3.14159265) / 180);
		y = x2 * sin((i * 3.14159265) / 180) + y2 * cos((i * 3.14159265) / 180);
		x2 = x;
		y2 = y;*/
		/*for (double j = 0.5; j <= i; j += 0.5)
		{
			x1 = x1 * cos(0.00873) - y1 * sin(0.00873);
			y1 = x1 * sin(0.00873) + y1 * cos(0.00873);
			x2 = x2 * cos(0.00873) - y2 * sin(0.00873);
			y2 = x2 * sin(0.00873) + y2 * cos(0.00873);
		}*/
		/*x1 = x1 * cos((i * 3.14159265) / 180) - y1 * sin((i * 3.14159265) / 180);
		y1 = x1 * sin(i * 3.14159265 / 180) + y1 * cos(i * 3.14159265 / 180);
		x2 = x2 * cos((i * 3.14159265) / 180) - y2 * sin((i * 3.14159265) / 180);
		y2 = x2 * sin((i * 3.14159265) / 180) + y2 * cos((i * 3.14159265) / 180);*/
		x31 += 0.002777777777778;
		x33 += 0.002777777777778;
		x1 = I[0];
		y1 = I[1];
		x2 = J[0];
		y2 = J[1];

	}
}
void vpukl2()
{

}
void quad(double A[], double B[], double C[], double D[], double E[], double F[], double G[], double A1[], double B1[], double C1[], double D1[], double E1[], double F1[], double G1[])
{
	glBegin(GL_QUADS);

	glColor3d(0.2, 0.7, 0.7);

	double* W = norm_coord(A, B, B1);
	glNormal3dv(W);
	glTexCoord2d(0, 0);
	glVertex3dv(A);
	glTexCoord2d(1, 0);
	glVertex3dv(B);
	glTexCoord2d(1, 1);
	glVertex3dv(B1);
	glTexCoord2d(0, 1);
	glVertex3dv(A1);

	 W = norm_coord(B, C, C1);
	glNormal3dv(W);
	glTexCoord2d(0, 0);
	glVertex3dv(B);
	glTexCoord2d(1, 0);
	glVertex3dv(C);
	glTexCoord2d(1,1);
	glVertex3dv(C1);
	glTexCoord2d(0,1);
	glVertex3dv(B1);

	 W = norm_coord(C, D, D1);
	 for (int i = 0; i < 3; i++)
	 {
		 W[i] *= -1;
	 }
	glNormal3dv(W);
	glTexCoord2d(0,0);
	glVertex3dv(C);
	glTexCoord2d(1, 0);
	glVertex3dv(D);
	glTexCoord2d(1,1);
	glVertex3dv(D1);
	glTexCoord2d(0, 1);
	glVertex3dv(C1);

	/*glVertex3dv(D);
	glVertex3dv(E);
	glVertex3dv(E1);
	glVertex3dv(D1);*/

	W = norm_coord(E, F, F1);
	
	glNormal3dv(W);
	glTexCoord2d(0, 0);
	glVertex3dv(E);
	glTexCoord2d(1, 0);
	glVertex3dv(F);
	glTexCoord2d(1, 1);
	glVertex3dv(F1);
	glTexCoord2d(0, 1);
	glVertex3dv(E1);

	 W = norm_coord(F, G, G1);
	glNormal3dv(W);
	glTexCoord2d(0, 0);
	glVertex3dv(F);
	glTexCoord2d(1, 0);
	glVertex3dv(G);
	glTexCoord2d(1, 1);
	glVertex3dv(G1);
	glTexCoord2d(0, 1);
	glVertex3dv(F1);

	 W = norm_coord(G, A, A1);
	glNormal3dv(W);
	glTexCoord2d(0, 0);
	glVertex3dv(G);
	glTexCoord2d(1, 0);
	glVertex3dv(A);
	glTexCoord2d(1, 1);
	glVertex3dv(A1);
	glTexCoord2d(0, 1);
	glVertex3dv(G1);

	glEnd();
}

void fig1(double A[], double B[], double C[], double D[], double E[], double F[], double G[])
{

	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glBegin(GL_TRIANGLES);


	glColor4d(1, 0, 0,0);
	double* W = norm_coord(A, B, C);
	for (int i = 0; i < 3; i++)
	{
		W[i] *= -1;
	}
	glNormal3dv(W);
	glTexCoord2d(0.512, 0.0711);
	glVertex3dv(A);
	glTexCoord2d(0.8968, 0.5195);
	glVertex3dv(B);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	glColor4d(1, 1, 0,0);
	W = norm_coord(A, G, F);
	glNormal3dv(W);
	glTexCoord2d(0.512, 0.0711);
	glVertex3dv(A);
	glTexCoord2d(0, 0.0711);
	glVertex3dv(G);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);

	glColor4d(1, 0, 1,0);
	W = norm_coord(A, F, C);
	glNormal3dv(W);
	glTexCoord2d(0.512, 0.0711);
	glVertex3dv(A);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	glColor4d(0.5, 1, 0,0);
	W = norm_coord(E, F, C);
	for (int i = 0; i < 3; i++)
	{
		W[i] *= -1;
	}
	glNormal3dv(W);
	glTexCoord2d(0.064, 0.839);
	glVertex3dv(E);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	/*glColor3d(0, 0.2, 0.6);
	glVertex3dv(C);
	glVertex3dv(D);
	glVertex3dv(E);*/


	glEnd();

}
void fig(double A[], double B[], double C[], double D[], double E[], double F[], double G[])
{
	glBindTexture(GL_TEXTURE_2D,texId[1]);
	glBegin(GL_TRIANGLES);

	
	glColor3d(1, 0, 0);
	double* W = norm_coord(A, B, C);
	for (int i = 0; i < 3; i++)
	{
		W[i] *= -1;
	}
	glNormal3dv(W);
	glTexCoord2d(0.512, 0.0711);
	glVertex3dv(A);
	glTexCoord2d(0.8968, 0.5195);
	glVertex3dv(B);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	glColor3d(1, 1, 0);
	 W = norm_coord(A, G, F);
	glNormal3dv(W);
	glTexCoord2d(0.512,0.0711);
	glVertex3dv(A);
	glTexCoord2d(0, 0.0711);
	glVertex3dv(G);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);

	glColor3d(1, 0, 1);
	 W = norm_coord(A, F, C);
	glNormal3dv(W);
	glTexCoord2d(0.512, 0.0711);
	glVertex3dv(A);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	glColor3d(0.5, 1, 0);
	 W = norm_coord(E, F, C);
	 for (int i = 0; i < 3; i++)
	 {
		 W[i] *= -1;
	 }
	glNormal3dv(W);
	glTexCoord2d(0.064, 0.839);	
	glVertex3dv(E);
	glTexCoord2d(0.192, 0.3914);
	glVertex3dv(F);
	glTexCoord2d(0.320, 0.5195);
	glVertex3dv(C);

	/*glColor3d(0, 0.2, 0.6);
	glVertex3dv(C);
	glVertex3dv(D);
	glVertex3dv(E);*/


	glEnd();

}
double* norm_coord(double A[], double B[], double C[])
{
	double I[] = {A[0]-B[0], A[1] - B[1], A[2] - B[2] };
	double J[] = { C[0] - B[0], C[1] - B[1], C[2] - B[2] };
	double x = I[1] * J[2] - J[1] * I[2];
	double y = -1 * I[0] * J[2] + J[0] * I[2];
	double z = I[0] * J[1] - J[0] * I[1];
	double Z[] = { x, y, z };
	return Z;

}