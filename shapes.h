#pragma once
#ifndef _SHAPES_H_
#define _SHAPES_H_
#define _CRT_SECURE_NO_WARNINGS
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#include <cstdlib>
#ifdef _WIN32
  #include "glut.h"
#else
  #ifdef __APPLE__
    #include <GLUT/glut.h>
  #else
    #include <GL/glut.h>
  #endif
#endif
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

/*class Globs
{
public:
	Globs();
//	Shape *Shapes[100];
	int numShapes;
	double screen_x, screen_y;
};
*/

class Vertex
{
public:
	Vertex();
	Vertex(double x, double y);
	bool setX(double x);
	bool setY(double y);
	double getX();
	double getY();

protected:
	double mX, mY;
};

class Shape
{
public:
	Shape();
	Shape(Vertex v);
	Vertex getV();
	bool setV(Vertex v);
	void setColors(double red, double green, double blue);
	virtual void draw() = 0;
	virtual void save(std::ostream &os);
	virtual void load(std::istream &is);
	void setLines(bool lines);

protected:
	Vertex mV;
	double mRed;
	double mGreen;
	double mBlue;
	bool mLines;
};

class Rectangle: public Shape
{
public:
	Rectangle();
	Rectangle(Vertex v, double width, double height);
	bool setWidth(double width);
	bool setHeight(double height);
	double getWidth();
	double getHeight();
	virtual void draw();
	void draw_lines();
	virtual void save(std::ostream &os);
	virtual void load(std::istream &is);

protected:
	double mWidth, mHeight;

};
class Button : public Rectangle
{
public:
	Button();
	Button(Vertex v, double width, double height, const char *name);
	char *getName();
	bool setName(const char*name);
	virtual void draw();
	bool isInside(Vertex v);
protected:
	char mName[50];
};

class Slider : public Button
{
public:
	Slider();
	Slider(Vertex v, double width, double height, const char *name);
	virtual void draw();
	void setSlider();
	double getColor();
	bool findSliderPos(Vertex v);
protected:
	Rectangle mSlider;
	Vertex mSliderPos;
	double mColor;
};

class Triangle: public Shape
{
public:
	Triangle();
	Triangle(Vertex v1, Vertex v2, Vertex v3);
	bool setV1(Vertex v);
	bool setV2(Vertex v);
	bool setV3(Vertex v);
	Vertex getV1();
	Vertex getV2();
	Vertex getV3();
	virtual void draw();
	void draw_lines();
	virtual void save(std::ostream &os);
	virtual void load(std::istream &is);
protected:
	Vertex mV1, mV2, mV3;

};

class Circle : public Shape
{
public:
	Circle();
	Circle(Vertex v, double r);
	bool setR(double r);
	double getR();
	virtual void draw();
	void draw_lines();
	virtual void save(std::ostream &os);
	virtual void load(std::istream &is);
	

protected:
	double mR;
};

void DrawText(double x, double y, const char *string);
void display();
void mouse(int mouse_button, int state, int x, int y);
void moveMouse(int x, int y);
void DrawRectangle(double x1, double y1, double x2, double y2);
void reshape(int w, int h);
void keyboard(unsigned char c, int x, int y);
void buttonClick(Button &butt);



#endif
