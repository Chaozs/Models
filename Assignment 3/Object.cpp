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
}

void Object::buildNormal(float x, float y, float z, int index)
{
    float vector1[3]; //down vector
    float vector2[3]; //left vector
    float vector3[3]; //up vector
    float vector4[3]; //right vector

    float crossProduct[3]; //normal of top right quad
    float crossProduct2[3]; //normal of bottom right quad
    float crossProduct3[3]; //normal of bottom left quad
    float crossProduct4[3]; //normal of top left quad

    vector4[0] = x+1.0;
    vector4[1] = y;
    vector4[2] = z;

    vector3[0] = x;
    vector3[1] = y;
    vector3[2] = z+1.0;

    vector2[0] = x-1.0;
    vector2[1] = y;
    vector2[2] = z;

    vector1[0] = x;
    vector1[1] = y;
    vector1[2] = z-1.0;

    //calculate normal of top right quad
    crossProduct[0] = vector1[1]*vector2[2] - vector1[2]*vector2[1];
    crossProduct[1] = vector1[2]*vector2[0] - vector1[0]*vector2[2];
    crossProduct[2] = vector1[0]*vector2[1] - vector1[1]*vector2[0];
    float unit = sqrt(crossProduct[0]*crossProduct[0] + crossProduct[1]*crossProduct[1] + crossProduct[2]*crossProduct[2]);
    crossProduct[0] = crossProduct[0]/unit;
    crossProduct[1] = crossProduct[1]/unit;
    crossProduct[2] = crossProduct[2]/unit;
    //calculate normal of top left quad
    crossProduct2[0] = vector2[1]*vector3[2] - vector2[2]*vector3[1];
    crossProduct2[1] = vector2[2]*vector3[0] - vector2[0]*vector3[2];
    crossProduct2[2] = vector2[0]*vector3[1] - vector2[1]*vector3[0];
    float unit2 = sqrt(crossProduct2[0]*crossProduct2[0] + crossProduct2[1]*crossProduct2[1] + crossProduct2[2]*crossProduct2[2]);
    crossProduct2[0] = crossProduct2[0]/unit2;
    crossProduct2[1] = crossProduct2[1]/unit2;
    crossProduct2[2] = crossProduct2[2]/unit2;

    //calculate normal of bottom right quad
    crossProduct3[0] = vector3[1]*vector4[2] - vector3[2]*vector4[1];
    crossProduct3[1] = vector3[2]*vector4[0] - vector3[0]*vector4[2];
    crossProduct3[2] = vector3[0]*vector4[1] - vector3[1]*vector4[0];
    float unit3 = sqrt(crossProduct3[0]*crossProduct3[0] + crossProduct3[1]*crossProduct3[1] + crossProduct3[2]*crossProduct3[2]);
    crossProduct3[0] = crossProduct3[0]/unit3;
    crossProduct3[1] = crossProduct3[1]/unit3;
    crossProduct3[2] = crossProduct3[2]/unit3;

    //calculate normal of bottom left quad
    crossProduct4[0] = vector4[1]*vector1[2] - vector4[2]*vector1[1];
    crossProduct4[1] = vector4[2]*vector1[0] - vector4[0]*vector1[2];
    crossProduct4[2] = vector4[0]*vector1[1] - vector4[1]*vector1[0];
    float unit4 = sqrt(crossProduct4[0]*crossProduct4[0] + crossProduct4[1]*crossProduct4[1] + crossProduct4[2]*crossProduct4[2]);
    crossProduct4[0] = crossProduct4[0]/unit4;
    crossProduct4[1] = crossProduct4[1]/unit4;
    crossProduct4[2] = crossProduct4[2]/unit4;

    //get average
    float tempAverage[3];
    tempAverage[0] = (crossProduct[0]+crossProduct2[0]+crossProduct3[0]+crossProduct4[0]);
    tempAverage[1] = (crossProduct[1]+crossProduct2[1]+crossProduct3[1]+crossProduct4[1]);
    tempAverage[2] = (crossProduct[2]+crossProduct2[2]+crossProduct3[2]+crossProduct4[2]);

    //calculate unit length
    float unit5 = sqrt(tempAverage[0]*tempAverage[0] + tempAverage[1]*tempAverage[1] + tempAverage[2]*tempAverage[2]);
    //set normal vector of each face using top right vector
    normVec[index][0] = tempAverage[0]/unit5;
    normVec[index][1] = tempAverage[1]/unit5;
    normVec[index][2] = tempAverage[2]/unit5;
}

void Object::drawHouse()
{
    glBegin(GL_TRIANGLES);
    //bottom face
    // glVertex3f(0,0,1);
    // glVertex3f(1,0,1);
    // glVertex3f(0,0,0);

    // glVertex3f(1,0,1);
    // glVertex3f(1,0,0);
    // glVertex3f(0,0,0);

    // //back left
    // glVertex3f(1,1,0);
    // glVertex3f(1,0,0);
    // glVertex3f(0,0,0);

    // glVertex3f(0,1,0);
    // glVertex3f(1,1,0);
    // glVertex3f(0,0,0);

    // //back right
    // glVertex3f(0,1,1);
    // glVertex3f(0,0,1);
    // glVertex3f(0,0,0);

    // glVertex3f(0,1,0);
    // glVertex3f(0,1,1);
    // glVertex3f(0,0,0);

    // //back left
    // glVertex3f(1,1,0);
    // glVertex3f(1,0,0);
    // glVertex3f(0,0,0);

    // glVertex3f(0,1,0);
    // glVertex3f(1,1,0);
    // glVertex3f(0,0,0);

    //bottom
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);

    glVertex3f(-1,-1,-1);
    glVertex3f(1,-1,1);
    glVertex3f(1,-1,-1);

    //front right
    glVertex3f(-1,-1,1);
    glVertex3f(-1,0,1);
    glVertex3f(-1,0,-1);

    glVertex3f(-1,-1,1);
    glVertex3f(-1,0,-1);
    glVertex3f(-1,-1,-1);

    //front left
    glVertex3f(1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,0,-1);

    glVertex3f(1,-1,-1);
    glVertex3f(-1,0,-1);
    glVertex3f(1,0,-1);

    //back right
    glVertex3f(1,-1,1);
    glVertex3f(1,0,1);
    glVertex3f(-1,0,1);

    glVertex3f(1,-1,1);
    glVertex3f(-1,0,1);
    glVertex3f(-1,-1,1);

    //back left
    glVertex3f(1,-1,-1);
    glVertex3f(1,0,-1);
    glVertex3f(1,-1,1);

    glVertex3f(1,-1,1);
    glVertex3f(1,0,-1);
    glVertex3f(1,0,1);




    glVertex3f(-1,0,-1);
    glVertex3f(-1,0,1);
    glVertex3f(0,1,0);


    glVertex3f(-1,0,1);
    glVertex3f(1,0,1);
    glVertex3f(0,1,0);


    glVertex3f(1,0,1);
    glVertex3f(1,0,-1);
    glVertex3f(0,1,0);


    glVertex3f(1,0,-1);
    glVertex3f(-1,0,-1);
    glVertex3f(0,1,0);

    glEnd();

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
    case House:
        return 5;
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
    case House:
        glPushMatrix();
        glScalef(objectScale/2, objectScale/2, objectScale/2);
        drawHouse();
        glPopMatrix();
        break;
    }

    //draws wireframe for selected object
    if (isSelected)
    {
        glColor3f(0, 0, 1);
        switch(type)
        {
        case Cube:
            glutWireCube(objectScale);             //draws unit length cube
            break;
        case Sphere:
            glutWireSphere(objectScale/2, 16, 16);   //draws unit length sphere
            break;
        case Octahedron:
            glPushMatrix();
            glScalef(objectScale/2, objectScale/2, objectScale/2);
            glutWireOctahedron();                  //draws unit length octahedron
            glPopMatrix();
            break;
        case Cone:
            glPushMatrix();
            glTranslatef(0, 0, -objectScale/2);
            glutWireCone(objectScale/2, objectScale, 16, 16);    //draws unit length cone
            glPopMatrix();
            break;
        case Torus:
            glPushMatrix();
            glScalef(1, 1, 3);
            glutWireTorus(objectScale/3-objectScale/6, objectScale/3, 16, 16);     //draws unit length torus
            glPopMatrix();
            break;
        case House:
            glPushMatrix();
            glScalef(objectScale/2, objectScale/2, objectScale/2);
            drawHouse();
            glPopMatrix();
            break;
        }
    }
    glPopMatrix();
}
