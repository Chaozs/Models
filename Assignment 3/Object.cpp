/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Object.h"

//constructor
Object::Object(ObjectType t)
{
    type = t;

    //sets initial position to origin
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;

    //sets initial orientation
    if(t==Cone)
    {
        orientation[0] = -90;
    }
    else
    {
        orientation[0] = 0;
    }
    orientation[1] = 0;
    orientation[2] = 0;

    //sets initial scale to unit size
    objectScale = 1;
}

void Object::setPosition(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Object::setOrientation(float x, float y, float z)
{
    orientation[0] = x;
    orientation[1] = y;
    orientation[2] = z;
}

float Object::getPosX()
{
    return position[0];
}

float Object::getPosY()
{
    return position[1];
}

float Object::getPosZ()
{
    return position[2];
}

float Object::getOrientationX()
{
    return orientation[0];
}

float Object::getOrientationY()
{
    return orientation[1];
}

float Object::getOrientationZ()
{
    return orientation[2];
}

float Object::getScale()
{
    return objectScale;
}

void Object::setScale(float scale)
{
    objectScale = scale;
}

void Object::drawObject()
{
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);            //move object to its position
    glRotatef(orientation[0], 1, 0, 0);                             //rotates object to its orientation
    glRotatef(orientation[1], 0, 1, 0);
    glRotatef(orientation[2], 0, 0, 1);
    switch(type)
    {
    case Cube:
        glutSolidCube(objectScale);             //draws cube
        break;
    case Sphere:
        glutSolidSphere(objectScale, 16, 16);   //draws sphere
        break;
    case Teapot:
        glutSolidTeapot(objectScale);           //draws teapot
        break;
    case Cone:
        glutSolidCone(objectScale, objectScale, 16, 16);    //draws cone
        break;
    case Torus:
        glutSolidTorus(objectScale-objectScale/2, objectScale, 16, 16);     //draws torus
        break;
    }
    glPopMatrix();
}

void Object::drawWireframe()
{
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);            //set translation of object
    glRotatef(orientation[0], 1, 0, 0);                             //set rotation of object
    glRotatef(orientation[1], 0, 1, 0);
    glRotatef(orientation[2], 0, 0, 1);
    glColor3f(1, 0, 0); //color wireframe red
    switch(type)
    {
    case Cube:
        glutWireCube(objectScale);
        break;
    case Sphere:
        glutWireSphere(objectScale, 16, 16);
        break;
    case Teapot:
        glutWireTeapot(objectScale);
        break;
    case Cone:
        glutWireCone(objectScale, objectScale, 16, 16);
        break;
    case Torus:
        glutWireTorus(objectScale-objectScale/2, objectScale, 16, 16);
        break;
    }
    glPopMatrix();
}
