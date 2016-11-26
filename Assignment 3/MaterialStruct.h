/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __MATERIALSTRUCT_H__
#define __MATERIALSTRUCT__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using namespace std;
//Material structure
struct MaterialStruct
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
};

//list 5 different materials
MaterialStruct redPlastic =
{
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    0.25
};
MaterialStruct brass =
{
    {0.33f, 0.22f, 0.27f, 1.f},
    {0.78f, 0.57f, 0.11f, 1.f},
    {0.99f, 0.94f, 0.8f, 1.f},
    0.22f
};
MaterialStruct chrome =
{
    {0.25f, 0.25f, 0.25f, 1.f},
    {0.4f, 0.4f, 0.4f, 1.f},
    {0.75f, 0.75f, 0.75f, 1.f},
    0.6f
};
MaterialStruct silver =
{
    {0.2f, 0.2f, 0.2f, 1.f},
    {0.51f, 0.51f, 0.51f, 1.f},
    {0.51f, 0.51f, 0.51f, 1.f},
    0.4f
};
MaterialStruct greenRubber =
{
    {0.0f, 0.05f, 0.f, 1.f},
    {0.4f, 0.5f, 0.4f, 1.f},
    {0.04f, 0.7f, 0.04f, 1.f},
    0.078f
};

#endif
