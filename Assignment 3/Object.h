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

using namespace std;

class Object
{
    public:
        enum ObjectType {Cube, Sphere, Teapot, Cone, Torus};
        Object(ObjectType);   //constructor
        void setPosition(float,float,float);
        void setOrientation(float,float,float);
        void setScale(float);
        float getPosX();
        float getPosY();
        float getPosZ();
        vector<float> getOrientation();
        float getScale();
        void drawObject();
        string getType();

    private:
        float position[3];
        float orientation[3];
        //TODO add material
        //TDO Bounding planes
        float objectScale;
        ObjectType type;
};

#endif
