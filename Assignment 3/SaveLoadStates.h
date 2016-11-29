/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __SAVELOADSTATES_H__
#define __SAVELOADSTATES_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "Object.h"

using namespace std;

class SaveLoadStates
{
public:
    static void saveState(string, list<Object*>);
    static list<Object*> loadState(string);
};

#endif
