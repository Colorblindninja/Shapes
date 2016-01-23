#include "shapes.h"

std::vector<Shape*> g_shapes;
Vertex g_points[3];
int g_numClicks = 0;
Button rect_Button = Button(Vertex(10, 10), 100, 15, "Rectangle");
Button tri_Button = Button(Vertex(10, 40), 100, 15, "Triangle");
Button cir_Button = Button(Vertex(10, 70), 100, 15, "Circle");
Button clear_Button = Button(Vertex(10, 250), 100, 15, "Clear");
Button undo_Button = Button(Vertex(10, 220), 100, 15, "Undo");
Button load_Button = Button(Vertex(10, 280), 100, 15, "Load");
Button save_Button = Button(Vertex(10, 310), 100, 15, "Save");
Button quit_Button = Button(Vertex(10, 340), 100, 15, "Quit");
Button lines_button = Button(Vertex(10, 100), 100, 15, "Outline");
Slider g_bBlue = Slider(Vertex(10, 130), 100, 15, "Blue");
Slider g_bGreen = Slider(Vertex(10, 160), 100, 15, "Green");
Slider g_bRed = Slider(Vertex(10, 190), 100, 15, "Red");
double g_red, g_blue, g_green;
Circle C = Circle(Vertex(0, 0), 4);
Circle C2 = Circle(Vertex(0, 0), 4);
Rectangle line_rect = Rectangle(Vertex(0, 0), 0, 0);
Triangle line_triangle = Triangle(Vertex(0, 0), Vertex(0, 0), Vertex(0, 0));
Circle line_circle = Circle(Vertex(0, 0), 0);
bool draw_circle = false;
bool draw_circle2 = false;
bool draw_line = false;
bool draw_outline = false;
Vertex g_mouse = Vertex(0, 0);
Vertex line_v = Vertex(0, 0);
int g_mode = 0; //1=Rectangle, 2=Triangle, 3=circle
double g_screen_x, g_screen_y;
int main(int argc, char **argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	glutInit(&argc, argv);
	g_screen_x = 700;
	g_screen_y = 500;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(g_screen_x, g_screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen)
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	}
	else
	{
		glutCreateWindow("Time to shape up!");
	}

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(moveMouse);

	glColor3d(0, 0, 0); // forground color
	glClearColor(1, 1, 1, 0); // background color

	glutMainLoop();
	_CrtDumpMemoryLeaks();
	return 0;
}
void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 'q':
	case 27: // escape character means to quit the program
		exit(0);
		break;
	case 'b':
		// do something when 'b' character is hit.
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	g_screen_x = w;
	g_screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i;
	for (i = 0; i < g_shapes.size(); i++)
	{
		g_shapes[i]->draw();
	}
	glColor3d(.3, 0, 0);
	if (draw_circle)
	{
		C.draw();
	}
	if (draw_circle2)
	{
		C2.draw();
	}
	glColor3d(g_red, g_green, g_blue);
	if ((g_mode == 2) && g_numClicks == 1)
	{
		glBegin(GL_LINE_STRIP);
		glVertex2d(line_v.getX(), line_v.getY());
		glVertex2d(g_mouse.getX(), g_mouse.getY());
		glEnd();
	}
	if (g_mode == 2 && g_numClicks == 2)
	{
		line_triangle.setV3(g_mouse);
		line_triangle.draw_lines();
	}
	if (g_mode == 1 && g_numClicks == 1)
	{
		double width, height;
		width = g_mouse.getX() - g_points[0].getX();
		height = g_mouse.getY() - g_points[0].getY();
		line_rect.setWidth(width);
		line_rect.setHeight(height);
		line_rect.draw_lines();
	}
	if (g_mode == 3 && g_numClicks == 1)
	{
		double radius = (std::sqrt(std::pow((g_mouse.getX() - g_points[0].getX()), 2) + std::pow((g_mouse.getY() - g_points[0].getY()), 2)));
		line_circle.setR(radius);
		line_circle.draw_lines();
	}
	rect_Button.draw();
	tri_Button.draw();
	cir_Button.draw();
	clear_Button.draw();
	undo_Button.draw();
	load_Button.draw();
	save_Button.draw();
	quit_Button.draw();
	if (draw_outline) {
		lines_button.setColors(0, 0, 0);
	}
	else {
		lines_button.setColors(.4, .4, .4);
	}
	lines_button.draw();
	g_bBlue.setColors(g_red, g_green, g_blue);
	g_bGreen.setColors(g_red, g_green, g_blue);
	g_bRed.setColors(g_red, g_green, g_blue);
	g_bBlue.draw();
	g_bGreen.draw();
	g_bRed.draw();
	glutPostRedisplay();
	glutSwapBuffers();
}

void mouse(int mouse_button, int state, int x, int y)
{
	// translate pixel coordinates to display coordinates
	int xdisplay = x;
	int ydisplay = g_screen_y - y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Vertex v = Vertex(xdisplay, ydisplay);
		if (rect_Button.isInside(v))
		{
			buttonClick(rect_Button);
			g_mode = 1;
			return;
		}
		else if (tri_Button.isInside(v))
		{
			buttonClick(tri_Button);
			g_mode = 2;
			return;
		}
		else if (lines_button.isInside(v))
		{
			buttonClick(lines_button);
			g_mode = 0;
			if (!draw_outline) {
				draw_outline = true;
			}
			else {
				draw_outline = false;
			}
			return;
		}
		else if (cir_Button.isInside(v))
		{
			buttonClick(cir_Button);
			g_mode = 3;
			return;
		}
		else if (clear_Button.isInside(v))
		{
			buttonClick(clear_Button);
			g_mode = 0;
			/*int i;
			for (i = 0; i < g_shapes.size(); i++)
			{

			}*/
			while (!g_shapes.empty())
			{
				Shape *item = g_shapes.back();
				g_shapes.pop_back();
				delete item;
			}
		}
		else if (undo_Button.isInside(v))
		{
			buttonClick(undo_Button);
			if (!g_shapes.empty())
			{
				Shape *item = g_shapes.back();
				g_shapes.pop_back();
				delete item;
			}
			return;
		}
		else if (load_Button.isInside(v))
		{
			buttonClick(load_Button);
			std::ifstream fin("ShapesSaved.txt");
			g_mode = 0;

			while (!g_shapes.empty())
			{
				Shape *item = g_shapes.back();
				g_shapes.pop_back();
				delete item;
			}

			while (fin)
			{
				std::string s;
				fin >> s;
				if (s == "Rectangle")
				{
					Shape *rect = new Rectangle(Vertex(0,0),0,0);
					rect->load(fin);
					g_shapes.push_back(rect);
				}
				if (s == "Triangle")
				{
					Shape *tri = new Triangle(Vertex(0,0), Vertex(0, 0), Vertex(0, 0));
					tri->load(fin);
					g_shapes.push_back(tri);
				}
				if (s == "Circle")
				{
					Shape *cir = new Circle(Vertex(0, 0), 0);
					cir->load(fin);
					g_shapes.push_back(cir);
				}
			}
			
			fin.close();
			return;
		}
		else if (save_Button.isInside(v))
		{
			buttonClick(save_Button);
			std::ofstream fout("ShapesSaved.txt");
			g_mode = 0;
			int i;
			for (i = 0; i < g_shapes.size(); i++)
			{
				g_shapes[i]->save(fout);
				fout << std::endl;
			}


			fout.close();
			return;
		}
		else if (quit_Button.isInside(v))
		{
			while (!g_shapes.empty())
			{
				Shape *item = g_shapes.back();
				g_shapes.pop_back();
				delete item;
			}
			
			exit(0);
			return;
		}
		else if (g_bBlue.isInside(v))
		{
			g_bBlue.findSliderPos(v);
			g_blue = g_bBlue.getColor();
			std::cout << "Blue is: " << g_blue << std::endl;
		}
		else if (g_bGreen.isInside(v))
		{
			g_bGreen.findSliderPos(v);
			g_green = g_bGreen.getColor();
			std::cout << "Green is: " << g_green << std::endl;
		}
		else if (g_bRed.isInside(v))
		{
			g_bRed.findSliderPos(v);
			g_red = g_bRed.getColor();
			std::cout << "Red is: " << g_red << std::endl;
		}
		else
		{
			if (g_mode != 0)
			{
				g_points[g_numClicks] = v;
				g_numClicks++;
				if (g_numClicks == 1)
				{
					C.setV(v);
					draw_circle = true;
				}
				if (g_numClicks == 2)
				{
					C2.setV(v);
					draw_circle2 = true;
				}
			}
			if (g_mode == 2 || g_mode == 1)
			{
				if (g_numClicks == 1)
				{
					line_v = g_points[0];

				}
			}
			if (g_mode == 2)
			{
				if (g_numClicks == 2)
				{
					line_triangle.setV1(g_points[0]);
					line_triangle.setV2(g_points[1]);
				}
			}
			if (g_mode == 1)
			{
				if (g_numClicks == 1)
				{
					line_rect.setV(g_points[0]);
				}
			}
			if (g_mode == 3)
			{
				if (g_numClicks == 1)
				{
					line_circle.setV(g_points[0]);
				}
			}
		}
		if (g_mode == 1 && g_numClicks == 2)
		{
			double width, height;
			width = g_points[1].getX() - g_points[0].getX();
			height = g_points[1].getY() - g_points[0].getY();
			Shape *rect = new Rectangle(g_points[0], width, height);
			rect->setColors(g_red, g_green, g_blue);
			rect->setLines(draw_outline);
			g_shapes.push_back(rect);
			g_numClicks = 0;
			draw_circle = false;
			draw_circle2 = false;
		}
		if (g_mode == 2 && g_numClicks == 3)
		{
			Shape *tri = new Triangle(g_points[0], g_points[1], g_points[2]);
			tri->setColors(g_red, g_green, g_blue);
			tri->setLines(draw_outline);
			g_shapes.push_back(tri);
			g_numClicks = 0;
			draw_circle = false;
			draw_circle2 = false;
		}
		if (g_mode == 3 && g_numClicks == 2)
		{
			double radius = (std::sqrt(std::pow((g_points[1].getX() - g_points[0].getX()), 2) + std::pow((g_points[1].getY() - g_points[0].getY()), 2)));
			Shape *cir = new Circle (g_points[0], radius);
			cir->setColors(g_red, g_green, g_blue);
			cir->setLines(draw_outline);
			g_shapes.push_back(cir);
			g_numClicks = 0;
			draw_circle = false;
			draw_circle2 = false;
		}
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
	glutPostRedisplay();
}

void moveMouse(int x, int y)
{
	g_mouse.setX(x);
	g_mouse.setY(g_screen_y - y);
}

void buttonClick(Button &butt)
{
	g_numClicks = 0;
	draw_circle = false;
	draw_circle2 = false;
	bool draw_line = false;
	rect_Button.setColors(.4, .4, .4);
	tri_Button.setColors(.4, .4, .4);
	cir_Button.setColors(.4, .4, .4);
	clear_Button.setColors(.4, .4, .4);
	undo_Button.setColors(.4, .4, .4);
	load_Button.setColors(.4, .4, .4);
	save_Button.setColors(.4, .4, .4);
	quit_Button.setColors(.4, .4, .4);

	butt.setColors(0, 0, 0);

}