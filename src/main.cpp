/*********************************/
/* CS 590CGS Lab framework       */
/* bbenes ~ at ~ purdue.edu      */
/* Press +,- to subdivide surface*/
/*       <,> to adjust width     */
/*       , and . to adjust angle */
/*       ctrl+s to save obj      */
/*********************************/

#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <string>

#include <GL/freeglut.h>
#include "treeNode.h"
//in house created libraries
#include "trackball.h"
#include "subDivMethod.h"
#include "objGen.h"
#include "math\triangle.h"
#define DarkGreen vec3(0.05, 0.12, 0.06)
#define DarkPurple vec3(0.18, 0.1,0.5)

#pragma warning(disable : 4996)
#pragma comment(lib, "freeglut.lib")
using namespace glm;
using namespace std;

//some trackball variables -> used in mouse feedback
TrackBallC trackball;
bool mouseLeft, mouseMid, mouseRight;

int subDivDegree = 0;
int subDivisionCounter = 0;
float slope = 60.f, treeSize = 0.5f;
treeMesh *surf;
//window size
GLint wWindow=1200;
GLint hWindow=800;

/*********************************
Some OpenGL-related functions DO NOT TOUCH
**********************************/
//displays the text message in the GL window
void GLMessage(char *message)
{
	static int i;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.f, 100.f, 0.f, 100.f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3ub(0, 0, 255);
	glRasterPos2i(10, 10);
	for (i = 0; i<(int)strlen(message); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

//called when a window is reshaped
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);
	//remember the settings for the camera
	wWindow = w;
	hWindow = h;
}

//Some simple rendering routines using old fixed-pipeline OpenGL
//draws line from a to b with color 
void DrawLine(vec3 a, vec3 b, vec3 color) {

	glColor3fv(&color[0]);
	glBegin(GL_LINES);
		glVertex3fv(&a[0]);
		glVertex3fv(&b[0]);
	glEnd();
}

//draws point at a with color 
void DrawPoint(vec3 a, vec3 color) {

	glColor3fv(&color[0]);
	glPointSize(5);
	glBegin(GL_POINTS);
		glVertex3fv(&a[0]);
	glEnd();
}

void drawTree(treeMesh *surf) {

	vector<Edge*>::iterator it = surf->edgeList_treeMesh.begin();
	for (it; it != surf->edgeList_treeMesh.end(); it++) {
		DrawLine((*it)->a->pointPos, (*it)->b->pointPos, DarkGreen);
		DrawPoint((*it)->a->pointPos, DarkPurple);
	}
}

/**********************
LAB related MODIFY
***********************/

//This defines the actual curve 
inline vec3 P(GLfloat t)
{
	const float rad = 0.2f;
	const float height = 1.f;
	const float rot = 5.f;
	return vec3(rad*(float)sin(rot*M_PI*t),height*t,rad*(float)cos(rot*M_PI*t)); //spiral with radius rad, height, and rotations rot
}


void initMesh() {
	if (slope <= 45.f)
		slope = 50.f;
	if (slope >= 90.f)
		slope = 90.f;
	treeNode *tree = new treeNode(slope, treeSize);
	surf = new treeMesh(tree->treeNodeList);
	subDivisionCounter = 0;
}

void SubDivision() {
	if (subDivisionCounter > subDivDegree)
		initMesh();
	while (subDivisionCounter < subDivDegree) {
		surf = surf->CatmullClarkSubdiv();
		subDivisionCounter++;
	}
}

void SaveMesh() {
	vector<Face*>::iterator i = surf->faceList_treeMesh.begin();
	vector<TriangleC> tri;
	for (i; i < surf->faceList_treeMesh.end(); i++) {
		TriangleC t;
		t.Set((*i)->edge0->a->pointPos, (*i)->edge0->b->pointPos, (*i)->edge2->b->pointPos);
		tri.push_back(t);
		t.Set((*i)->edge2->a->pointPos, (*i)->edge0->a->pointPos, (*i)->edge2->b->pointPos);
		tri.push_back(t);
	}
	SaveOBJ(&tri, "TreeMesh.obj");
}
//returns random number from <-1,1>
inline float random11() { 
	return 2.f*rand() / (float)RAND_MAX - 1.f;
}

//randomizes an existing curve by adding random number to each coordinate
void Randomize(vector <vec3> *a) {
	const float intensity = 0.01f;
	for (unsigned int i = 0; i < a->size(); i++) {
		vec3 r(random11(), random11(), random11());
		a->at(i) = a->at(i) + intensity*r;
	}
}

//display coordinate system
void CoordSyst() {
	vec3 a, b, c;
	vec3 origin(0, 0, 0);
	vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	//draw the coordinate system 
	a = vec3(1, 0, 0);
	b = vec3(0, 1, 0);
	c = vec3(cross(a, b)); //use cross product to find the last vector
	glLineWidth(4);
	DrawLine(origin, a, red);
	DrawLine(origin, b, green);
	DrawLine(origin, c, blue);
	glLineWidth(1);

}

//this is the actual code for the lab
void Lab01() {
	vec3 a,b,c;
	vec3 origin(0, 0, 0);
	vec3 red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), almostBlack(0.1f, 0.1f, 0.1f), yellow(1, 1, 0);

	drawTree(surf);
	CoordSyst();
}

//the main rendering function
void RenderObjects()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//set camera
	glMatrixMode(GL_MODELVIEW);
	trackball.Set3DViewCamera();
	//call the student's code from here
	Lab01();
}

//Add here if you want to control some global behavior
//see the pointFlag and how is it used
void Kbd(unsigned char a, int x, int y)//keyboard callback
{
	switch (a)
	{
	case 27: exit(0); break;

	case '<': {
		treeSize -= 0.01f;
		initMesh();
		break;
	}
	case '>': {
		treeSize += 0.01f;
		initMesh();
		break;
	}
	case ',':
		slope -= 0.5f;
		initMesh();
		break;
	case '.':
		slope += 0.5f;
		initMesh();
		break;
	case '-':
		subDivDegree--;
		if (subDivDegree < 0)
			subDivDegree = 0;
		SubDivision();
		break;
	case '+':
		subDivDegree++;
		SubDivision();
		break;
	case 32:
		SaveMesh();
		cout << "Save Mesh" << endl;
		break;
	}
	
	glutPostRedisplay();
}


/*******************
OpenGL code. Do not touch.
******************/
void Idle(void)
{
	glutPostRedisplay();
}

void Display(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1); //background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLMessage("Lab 3 - CS 590CGS");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (GLfloat)wWindow / (GLfloat)hWindow, 0.01, 100); //set the camera
	glMatrixMode(GL_MODELVIEW); //set the scene
	glLoadIdentity();
	gluLookAt(0, 10, 10, 0, 0, 0, 0, 1, 0); //set where the camera is looking at and from. 
	//static GLfloat angle = 0;
	//angle += angleIncrement;
	//if (angle >= 360.f) angle = 0.f;
	//glRotatef(Sign*angle, 0, 1, 0);
	RenderObjects();
	glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseLeft = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(false, x, y);
		mouseLeft = false;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseMid = true;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseMid = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		trackball.Set(true, x, y);
		mouseRight = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		trackball.Set(true, x, y);
		mouseRight = false;
	}
}

void MouseMotion(int x, int y) {
	if (mouseLeft)  trackball.Rotate(x, y);
	if (mouseMid)   trackball.Translate(x, y);
	if (mouseRight) trackball.Zoom(x, y);
	glutPostRedisplay();
}


int main(int argc, char **argv)
{ 
  glutInitDisplayString("stencil>=2 rgb double depth samples");
  glutInit(&argc, argv);
  glutInitWindowSize(wWindow,hWindow);
  glutInitWindowPosition(500,100);
  glutCreateWindow("Surface of Revolution");
  
  initMesh();

  glutDisplayFunc(Display);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Kbd); //+ and -
  glutSpecialUpFunc(NULL); 
  glutSpecialFunc(NULL);
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMotion);

  glutMainLoop();
	
  return 0;        
}
