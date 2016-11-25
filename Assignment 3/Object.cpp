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

vector<float> Object::getOrientation()
{
    vector<float> o;
    o[0] = orientation[0];
    o[1] = orientation[1];
    o[2] = orientation[2];
    return o;
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
    glTranslatef(position[0], position[1], position[2]);
    glRotatef(orientation[0], 1, 0, 0);
    glRotatef(orientation[1], 0, 1, 0);
    glRotatef(orientation[2], 0, 0, 1);
    switch(type)
    {
    case Cube:
        glPushMatrix();
        glTranslatef(0, objectScale/2, 0);
        glutSolidCube(objectScale);
        glPopMatrix();
        break;
    case Sphere:
        glPushMatrix();
        glTranslatef(0, objectScale/2, 0);
        glutSolidSphere(objectScale, 16, 16);
        glPopMatrix();
        break;
    case Teapot:
        glPushMatrix();
        glTranslatef(0, objectScale/2, 0);
        glutSolidTeapot(objectScale);
        glPopMatrix();
        break;
    case Cone:
        glPushMatrix();
        glTranslatef(0, objectScale/2, 0);
        glutSolidCone(objectScale, objectScale, 16, 16);
        glPopMatrix();
        break;
    case Torus:
        glPushMatrix();
        glTranslatef(0, objectScale/2, 0);
        glutSolidTorus(objectScale-objectScale/2, objectScale, 16, 16);
        glPopMatrix();
        break;
    }
    glPopMatrix();
}

string Object::getType()
{
    switch(type)
    {
    case Cube:
        return "cube";
    case Sphere:
        return "sphere";
    case Teapot:
        return "teapot";
    case Cone:
        return "cone";
    case Torus:
        return "torus";
    }
}
