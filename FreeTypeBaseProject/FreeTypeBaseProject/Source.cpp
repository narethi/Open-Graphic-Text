/*
	This project is designed as a test
	so that using FreeType will be easier
	later, i.e. this is used for instructional
	purposes only
*/
/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */


#include "stdafx.h"
#include "CharacterCatalogue.h"
#include "GraphicString.h"
#include "GraphicCharacter.h"
#include "TextBox.h"

float (*colorArray)[4] = new float[WIDTH*HEIGHT][4];
int imageWidth = 0, imageHeight = 0;

CharacterCatalogue testCatalogue("test", 0);
TextBox testTextBox(&testCatalogue, 2, 600, 700);

int previousTime = GetTickCount64();
int limit = 1000;//1000 / 45;
int idleCount = 0;
//the idle function is used to
//test the speed at which we can
//switch our char buffer
void idle(void)
{
	if((GetTickCount64() - previousTime) > limit)
	{
		previousTime = GetTickCount64();
		//testTextBox.incLine();
		glutPostRedisplay();
	}
}

GLuint texture;
void resphape(int x, int y)
{
	glutPostRedisplay();
}
void display(void)
{
	int left, right, bottom, top;
	
	left = testTextBox.getLeft();
	right = testTextBox.getWidth() + left;
	bottom = testTextBox.getBottom();
	top = testTextBox.getHeight() + bottom;
	
	glClearColor(0, 1, 0, 0);
	glEnable(GL_DEPTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	imageWidth = testTextBox.getWidth(), imageHeight = testTextBox.getTextureHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_FLOAT, testTextBox.getTexture());
	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	float * coords = testTextBox.getTextureCoords();
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(coords[3], coords[2]);
	glVertex2i(left, bottom);
	glTexCoord2f(coords[3], coords[0]);
	glVertex2i(left, top);
	glTexCoord2f(coords[1], coords[0]);
	glVertex2i(right, top);

	glTexCoord2f(coords[1], coords[0]);
	glVertex2i(right, top);
	glTexCoord2f(coords[1], coords[2]);
	glVertex2i(right, bottom);
	glTexCoord2f(coords[3], coords[2]);
	glVertex2i(left, bottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPointSize(2);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for(int i = 0; i < 200; i++)
	{
		if(i % 2 == 0) glVertex2i(1, i * 2);
	}
	glEnd();
	delete[] coords;
	glutSwapBuffers();
	glFlush();
}

int main(int argc, char**  argv)
{
	testCatalogue = CharacterCatalogue("testFont", 0);
	testCatalogue.addFont("Arial.ttf", 20);
	testTextBox = TextBox(&testCatalogue, 1, 50, 50, 400, 150);
	
	CharacterCatalogue testCatalogue2("restFine", 1);
	testCatalogue2.addFont("Arial.ttf", 12);

	CharacterCatalogue testCatalogue3("restFine", 1);
	testCatalogue3.addFont("Arial.ttf", 50);

	testTextBox.addCatalogue(&testCatalogue2);
	testTextBox.addCatalogue(&testCatalogue3);

	float * testColor = new float[3];
	testColor[0] = 0, testColor[1] = 0, testColor[2] = 1;
	testTextBox.addColor(testColor);

	testColor = new float[3];
	testColor[0] = 1, testColor[1] = 0, testColor[2] = 1;
	testTextBox.addColor(testColor);

	testTextBox.concatenate("abcdefghijklmnopqrstuvwxyz1234567890-_+=,./", 0, 0);
	testTextBox.concatenate("abcdefghijklmnopqrstuvwxyz1234567890-_+=,./", 0, 0);
	testTextBox.concatenate(" This is a story");
	testTextBox.concatenate(" all about how, my life got flipped.");
	testTextBox.concatenate(" turned upside down.");
	testTextBox.concatenate(" I want you to take a moment.", 0, 2);
	testTextBox.concatenate(" so sit right there.");
	testTextBox.concatenate(" and listen to the story." , 0, 0);
	testTextBox.concatenate(" of how I became the prince.", 0, 1);
	testTextBox.concatenate(" of Belaire.", 0, 2);
	testTextBox.concatenate(" I was playing some b-ball outside of school.", 0, 1);
	
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutCreateWindow("TestWindow");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(resphape);
	glutMainLoop();

	return 0;
}
