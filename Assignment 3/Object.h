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

#include <math.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Object
{
public:
    enum ObjectType {Cube, Sphere, Octahedron, Cone, Torus, House};
    Object();   //constructor
    void setPosition(float, float, float);
    void setOrientation(float, float, float);
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
    void setTexture(int);
    int getTexture();
    void drawObject(bool);
    int getType();
    void setType(ObjectType);
    void drawHouse();
    void setTexture1(GLuint*);
    void setTexture2(GLuint*);
    void setTexture3(GLuint*);

private:
    float position[3];
    float orientation[3];
    float normVec[9][3] = {{0,1,0}, {-1,0,0}, {0,0,-1}, {0,0,1}, {1,0,0}, {-1,1,0}, {0,1,1}, {1,1,0}, {0,1,-1} };
    int material;
    int texture;
    bool intersect;
    float objectScale;
    GLuint *myTex1;
    GLuint *myTex2;
    GLuint *myTex3;
    void initializeNormals();
    void bindTextures();
    ObjectType type;
};

#endif
