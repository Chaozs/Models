#ifndef __HOUSE_H__
#define __HOUSE_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class House
{
public:
	House(); 
	void drawHouse();

private:
	void buildNormal(float, float, float, int);
	float normVec[9][3]; //normal vector for each x,z
};

#endif