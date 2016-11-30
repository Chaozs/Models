/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#ifndef __OBJECT_H__
#define __OBJECT_H__

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

using namespace std;

class Object
{
public:
    enum ObjectType {Cube, Sphere, Octahedron, Cone, Torus};
    Object();   //constructor
    void setPosition(float,float,float);
    void setOrientation(float,float,float);
    void setScale(float);
    void storeMaterial(int);
    float getPosX();
    float getPosY();
    float getPosZ();
    float getOrientationX();
    float getOrientationY();
    float getOrientationZ();
    float getScale();
    int getMaterial();
    void drawObject(bool);
    void setType(ObjectType);
    int getType();
    bool getIntersection();
    void setIntersection(bool);

private:
    float position[3];
    float orientation[3];
    int material;
    bool intersect;
    //TDO Bounding planes
    float objectScale;
    ObjectType type;
};

#endif
