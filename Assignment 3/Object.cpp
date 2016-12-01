/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "Object.h"

//constructor
Object::Object()
{
    //sets initial position to origin
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;

    //sets initial orientation
    orientation[0] = 0;
    orientation[1] = 0;
    orientation[2] = 0;

    //sets initial scale to unit size
    objectScale = 1;

    //sets initial position of min and max points
    minPoint[0] = -0.5;
    minPoint[1] = -0.5;
    minPoint[2] = -0.5;
    maxPoint[0] = 0.5;
    maxPoint[1] = 0.5;
    maxPoint[2] = 0.5;
}

void Object::setType(ObjectType t)
{
    type = t;
    if(type==Cone)
    {
        orientation[0] = -90;
    }
}

//store material: where 0=redPlastic, 1=brass, 2=chrome, 3=silver, 4=greenrubber
void Object::storeMaterial(int mat)
{
    material = mat;
}

//return material index number
int Object::getMaterial()
{
    return material;
}

//return the objectType as an int, where 0=cube, 1=sphere, 2=octahedron, 3=cone, 4=torus
int Object::getType()
{
    switch(type)
    {
    case Cube:
        return 0;
        break;
    case Sphere:
        return 1;
        break;
    case Octahedron:
        return 2;
        break;
    case Cone:
        return 3;
        break;
    case Torus:
        return 4;
        break;
    }
    return -1;
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

void Object::drawObject(bool isSelected)
{
    glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);    //move object to its position
    glRotatef(orientation[0], 1, 0, 0);                     //rotates object to its orientation
    glRotatef(orientation[1], 0, 1, 0);
    glRotatef(orientation[2], 0, 0, 1);
    glColor3f(0.5, 0.5, 0.5);
    switch(type)
    {
    case Cube:
        glutSolidCube(objectScale);             //draws unit length cube
        break;
    case Sphere:
        glutSolidSphere(objectScale/2, 16, 16);   //draws unit length sphere
        break;
    case Octahedron:
        glPushMatrix();
        glScalef(objectScale/2, objectScale/2, objectScale/2);
        glutSolidOctahedron();                  //draws unit length octahedron
        glPopMatrix();
        break;
    case Cone:
        glPushMatrix();
        glTranslatef(0, 0, -objectScale/2);
        glutSolidCone(objectScale/2, objectScale, 16, 16);    //draws unit length cone
        glPopMatrix();
        break;
    case Torus:
        glPushMatrix();
        glScalef(1, 1, 3);
        glutSolidTorus(objectScale/3-objectScale/6, objectScale/3, 16, 16);     //draws unit length torus
        glPopMatrix();
        break;
    }
    glPopMatrix();

    //draws bounding box for selected object
    if (isSelected)
    {
        glColor3f(1, 0, 0);
        //draws each face based on max/min point of bounding box
        glBegin(GL_LINE_LOOP);
        glVertex3f(minPoint[0], minPoint[1], minPoint[2]);
        glVertex3f(minPoint[0], maxPoint[1], minPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], minPoint[2]);
        glVertex3f(maxPoint[0], minPoint[1], minPoint[2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(maxPoint[0], minPoint[1], minPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], minPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], minPoint[1], maxPoint[2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(minPoint[0], minPoint[1], minPoint[2]);
        glVertex3f(minPoint[0], maxPoint[1], minPoint[2]);
        glVertex3f(minPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(minPoint[0], minPoint[1], maxPoint[2]);

        glBegin(GL_LINE_LOOP);
        glVertex3f(minPoint[0], minPoint[1], minPoint[2]);
        glVertex3f(minPoint[0], minPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], minPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], minPoint[1], minPoint[2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(minPoint[0], minPoint[1], maxPoint[2]);
        glVertex3f(minPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], minPoint[1], maxPoint[2]);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(minPoint[0], maxPoint[1], minPoint[2]);
        glVertex3f(minPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], maxPoint[2]);
        glVertex3f(maxPoint[0], maxPoint[1], minPoint[2]);
        glEnd();
    }
}


void Object::setIntersection(bool set)
{
    intersect = set;
}

bool Object::getIntersection()
{
    return intersect;
}

void Object::setMinPointX(float x)
{
    minPoint[0] = x;
}

void Object::setMinPointY(float y)
{
    minPoint[1] = y;
}

void Object::setMinPointZ(float z)
{
    minPoint[2] = z;
}

void Object::setMaxPointX(float x)
{
    maxPoint[0] = x;
}

void Object::setMaxPointY(float y)
{
    maxPoint[1] = y;
}

void Object::setMaxPointZ(float z)
{
    maxPoint[2] = z;
}

float Object::getMinPointX()
{
    return minPoint[0];
}

float Object::getMinPointY()
{
    return minPoint[1];
}

float Object::getMinPointZ()
{
    return minPoint[2];
}

float Object::getMaxPointX()
{
    return maxPoint[0];
}

float Object::getMaxPointY()
{
    return maxPoint[1];
}

float Object::getMaxPointZ()
{
    return maxPoint[2];
}
