#include "shapes.h"
extern double g_red, g_blue, g_green;
// globals 
//Globs::Globs()
//{
//	numShapes = 0;
//	screen_x = 700;
//	screen_y = 500;
//}

// Vertex Methods
Vertex::Vertex()
	:mX(0), mY(0)
{}

Vertex::Vertex(double x, double y)
{
	setX(x);
	setY(y);
}
bool Vertex::setX(double x)
{
	mX = x;
	return true;
}
bool Vertex::setY(double y)
{
	mY = y;
	return true;
}
double Vertex::getX()
{
	return mX;
}
double Vertex::getY()
{
	return mY;
}

// Parent Shape Methods
Shape::Shape()
{
	mV.setX(0);
	mV.setY(0);
}
Shape::Shape(Vertex v)
{
	setV(v);
}
void Shape::setColors(double red, double green, double blue)
{
	mRed = red;
	mGreen = green;
	mBlue = blue;
}
void Shape::setLines(bool lines)
{
	mLines = lines;
}
bool Shape::setV(Vertex v)
{
	mV = v;
	return true;
}
Vertex Shape::getV()
{
	return mV;
}

void Shape::save(std::ostream &os)
{
	os << mRed << " "
		<< mGreen << " "
		<< mBlue << " "
		<< mLines << " ";
}
void Shape::load(std::istream &is)
{
	is >> mRed >> mGreen >> mBlue >> mLines;
}



//Rectangle Methods
Rectangle::Rectangle()
	:Shape(), mWidth(0), mHeight(0)
{}
Rectangle::Rectangle(Vertex v, double width, double height)
	:Shape(v), mWidth(width), mHeight(height)
{}

bool Rectangle::setWidth(double width)
{
	mWidth = width;
	return true;
}
bool Rectangle::setHeight(double height)
{
	mHeight = height;
	return true;
}
double Rectangle::getWidth()
{
	return mWidth;
}
double Rectangle::getHeight()
{
	return mHeight;
}
void Rectangle::save(std::ostream &os)
{
	os  << "Rectangle "
		<< mV.getX() << " "
		<< mV.getY() << " "
		<< mWidth << " "
		<< mHeight << " ";
	Shape::save(os);
}
void Rectangle::load(std::istream &is)
{
	double x, y;
	is >> x >> y >> mWidth >> mHeight;
	mV.setX(x);
	mV.setY(y);
	Shape::load(is);
}


void Rectangle::draw()
{
	if (mLines == true)
	{
		draw_lines();
		return;
	}
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_QUADS);
	glVertex2d(mV.getX(), mV.getY());
	glVertex2d(mV.getX() + mWidth, mV.getY());
	glVertex2d(mV.getX() + mWidth, mV.getY() + mHeight);
	glVertex2d(mV.getX(), mV.getY() + mHeight);
	glEnd();
}
void Rectangle::draw_lines()
{
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_LINE_STRIP);
	glVertex2d(mV.getX(), mV.getY());
	glVertex2d(mV.getX() + mWidth, mV.getY());
	glVertex2d(mV.getX() + mWidth, mV.getY() + mHeight);
	glVertex2d(mV.getX(), mV.getY() + mHeight);
	glVertex2d(mV.getX(), mV.getY());
	glEnd();
}

//Button Methods
Button::Button()
	:Rectangle()
{
	mName[0] = 0;
}
Button::Button(Vertex v, double width, double height, const char *name)
	:Rectangle(v, width, height)
{
	setName(name);
	setColors(.4, .4, .4);
}
char *Button::getName()
{
	return mName;
}
bool Button::setName(const char *name)
{
	std::strcpy(mName, name);
	mName[std::strlen(mName) + 1] = 0;
	return true;
}
void Button::draw()
{
	Rectangle::draw();
	glColor3d(1, 1, 1);  // 0-1 1 being full color
	DrawText(mV.getX()+2, mV.getY()+2, mName);
}
bool Button::isInside(Vertex v)
{
	if (v.getX() >= mV.getX() && v.getX() <= (mV.getX() + mWidth) && v.getY() >= mV.getY() && v.getY() <= (mV.getY() + mHeight))
	{
		return true;
	}
	return false;

}

Slider::Slider()
	:Button()
{
	mSlider = Rectangle();
	mSlider.setColors(1, 0, 0);
}

Slider::Slider(Vertex v, double width, double height, const char *name)
	:Button(v, width, height, name)
{
	mSlider = Rectangle();
	mSlider.setColors(1, 0, 0);
	mSliderPos.setX(v.getX());
	double mid = height / 2;
	mSliderPos.setY(v.getY()-mid);
}

void Slider::setSlider()
{
	mSlider.setV(mSliderPos);
	mSlider.setWidth(5);
	mSlider.setHeight(27);
}

void Slider::draw()
{
	setSlider();
	mSlider.draw();
	Button::draw();
}

bool Slider::findSliderPos(Vertex v)
{
	if (isInside(v))
	{
		mSliderPos.setX(v.getX());
		double diff = v.getX() - mV.getX();
		mColor = diff / mWidth;
		return true;
	}
	return false;
}

double Slider::getColor()
{
	return mColor;
}
//Triangle Methods
Triangle::Triangle()
{
	mV1.setX(0);
	mV1.setY(0);
	mV2.setX(0);
	mV2.setY(0);
	mV3.setX(0);
	mV3.setY(0);
}
Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3)
{
	setV1(v1);
	setV2(v2);
	setV3(v3);
}

bool Triangle::setV1(Vertex v)
{
	mV1 = v;
	return true;
}
bool Triangle::setV2(Vertex v)
{
	mV2 = v;
	return true;
}
bool Triangle::setV3(Vertex v)
{
	mV3 = v;
	return true;
}
Vertex Triangle::getV1()
{
	return mV1;
}
Vertex Triangle::getV2()
{
	return mV2;
}
Vertex Triangle::getV3()
{
	return mV3;
}

void Triangle::draw()
{
	if (mLines == true)
	{
		draw_lines();
		return;
	}
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_TRIANGLES);
	glVertex2d(mV1.getX(), mV1.getY());
	glVertex2d(mV2.getX(), mV2.getY());
	glVertex2d(mV3.getX(), mV3.getY());
	glEnd();
}
void Triangle::draw_lines()
{
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_LINE_STRIP);
	glVertex2d(mV1.getX(), mV1.getY());
	glVertex2d(mV2.getX(), mV2.getY());
	glVertex2d(mV3.getX(), mV3.getY());
	glVertex2d(mV1.getX(), mV1.getY());
	glEnd();
}

void Triangle::save(std::ostream &os)
{
	os  << "Triangle "
		<< mV1.getX() << " " << mV1.getY() << " "
		<< mV2.getX() << " " << mV2.getY() << " "
		<< mV3.getX() << " " << mV3.getY() << " ";
	Shape::save(os);
}
void Triangle::load(std::istream &is)
{
	double x, y;
	is >> x >> y;
	mV1.setX(x);
	mV1.setY(y);

	is >> x >> y;
	mV2.setX(x);
	mV2.setY(y);

	is >> x >> y;
	mV3.setX(x);
	mV3.setY(y);

	Shape::load(is);
}

//Circle Methods
Circle::Circle()
	:mR(0), Shape(){}
Circle::Circle(Vertex v, double r)
	: Shape(v)
{
	setR(r);
}

bool Circle::setR(double r)
{
	mR = r;
	return true;
}
double Circle::getR()
{
	return mR;
}

void Circle::draw()
{
	if (mLines == true)
	{
		draw_lines();
		return;
	}
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_POLYGON);
	for (int i = 0; i<32; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = mV.getX() + mR * cos(theta);
		double y = mV.getY() + mR * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();

}
void Circle::draw_lines()
{
	glColor3d(mRed, mGreen, mBlue);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i<33; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = mV.getX() + mR * cos(theta);
		double y = mV.getY() + mR * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void Circle::save(std::ostream &os)
{
	os  << "Circle "
		<< mV.getX() << " "
		<< mV.getY() << " "
		<< mR << " ";
	Shape::save(os);
}
void Circle::load(std::istream &is)
{
	double x, y;
	is >> x >> y >> mR;
	mV.setX(x);
	mV.setY(y);
	Shape::load(is);

}



void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}

	glDisable(GL_BLEND);
}