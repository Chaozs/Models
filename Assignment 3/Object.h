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
    Object();                   //constructor
    void setPosition(float, float, float);      //set position of object
    void setOrientation(float, float, float);   //set orientation of object
    void setScale(float);       //set scale of object
    void storeMaterial(int);    //set material index of object
    float getPosX();            //get x position of object
    float getPosY();            //get y positon of object
    float getPosZ();            //get z position of object
    float getOrientationX();    //get x orientation
    float getOrientationY();    //get y orientation
    float getOrientationZ();    //get z orientation
    float getScale();           //get scale
    int getMaterial();          //get material
    void setTexture(int);       //set texture index
    int getTexture();           //get texture index
    void drawObject(bool);      //draws object
    int getType();              //get object type index
    void setType(ObjectType);   //set object type
    void setTexture1(GLuint*);  //store texture 1 (interface.ppm)
    void setTexture2(GLuint*);  //store texture 2 (marble.ppm)
    void setTexture3(GLuint*);  //store texture 3 (snail_a.ppm)

private:
    float position[3];      //position of object
    float orientation[3];   //orientation of object
    float normVec[9][3] = {{0,-1,0}, {-1,0,0}, {0,0,-1}, {0,0,1},
    {1,0,0}, {-1,1,0}, {0,1,1}, {1,1,0}, {0,1,-1} }; //normal vector for custom shape
    int material;           //material index
    int texture;            //texture index; 1 = interface, 2 = marble, 3 = snail
    bool intersect;         //whether or not raypicking intersects
    float objectScale;      //object scale
    GLuint *myTex1;         //storing texture1
    GLuint *myTex2;         //storing texture2
    GLuint *myTex3;         //storing texture3
    void drawHouse(bool);   //draw custom shape house, where if boolean is true, wireframe is drawn only
    void bindTextures();    //bind textures
    void unbindTextures();  //unbind textures
    ObjectType type;        //the object type
};

#endif
