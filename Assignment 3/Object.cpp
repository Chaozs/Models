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

//draws custom house object. If wireFrame is true, only draw wireframe
void Object::drawHouse(bool wireFrame)
{
    if(wireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glBegin(GL_TRIANGLES);

    //bottom
    glNormal3fv(normVec[0]);

    glVertex3f(1,-1,1);
    glVertex3f(-1,-1,1);
    glVertex3f(-1,-1,-1);

    
    glVertex3f(1,-1,-1);
    glVertex3f(1,-1,1);
    glVertex3f(-1,-1,-1);

    //front right
    glNormal3fv(normVec[1]);

    glVertex3f(-1,-1,1);
    glVertex3f(-1,0,-1);
    glVertex3f(-1,-1,-1);

    glVertex3f(-1,-1,1);
    glVertex3f(-1,0,1);
    glVertex3f(-1,0,-1);

    //front left
    glNormal3fv(normVec[2]);

    glVertex3f(1,-1,-1);
    glVertex3f(-1,-1,-1);
    glVertex3f(-1,0,-1);

    glVertex3f(1,-1,-1);
    glVertex3f(-1,0,-1);
    glVertex3f(1,0,-1);

    //back right
    glNormal3fv(normVec[3]);

    glVertex3f(1,-1,1);
    glVertex3f(1,0,1);
    glVertex3f(-1,0,1);

    glVertex3f(1,-1,1);
    glVertex3f(-1,0,1);
    glVertex3f(-1,-1,1);

    //back left
    glNormal3fv(normVec[4]);

    glVertex3f(1,-1,-1);
    glVertex3f(1,0,-1);
    glVertex3f(1,-1,1);

    glVertex3f(1,-1,1);
    glVertex3f(1,0,-1);
    glVertex3f(1,0,1);

    //draw triangle part of house shape
    glNormal3fv(normVec[5]);
    glVertex3f(-1,0,-1);
    glVertex3f(-1,0,1);
    glVertex3f(0,1,0);

    glNormal3fv(normVec[6]);
    glVertex3f(-1,0,1);
    glVertex3f(1,0,1);
    glVertex3f(0,1,0);

    glNormal3fv(normVec[7]);
    glVertex3f(1,0,1);
    glVertex3f(1,0,-1);
    glVertex3f(0,1,0);

    glNormal3fv(normVec[8]);
    glVertex3f(1,0,-1);
    glVertex3f(-1,0,-1);
    glVertex3f(0,1,0);

    glEnd();

    if(wireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
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

//stores texture index
void Object::setTexture(int t)
{
    texture = t;
}

//return texture index
int Object::getTexture()
{
    return texture;
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

//sets position of object
void Object::setPosition(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

//sets orientation of object
void Object::setOrientation(float x, float y, float z)
{
    orientation[0] = x;
    orientation[1] = y;
    orientation[2] = z;
}

//gets X position of object
float Object::getPosX()
{
    return position[0];
}

//gets Y position of object
float Object::getPosY()
{
    return position[1];
}

//get Z position of object
float Object::getPosZ()
{
    return position[2];
}

//gets X orientation of object
float Object::getOrientationX()
{
    return orientation[0];
}

//gets Y orientation of object
float Object::getOrientationY()
{
    return orientation[1];
}

//gets Z orientation of object
float Object::getOrientationZ()
{
    return orientation[2];
}

//gets scale of object
float Object::getScale()
{
    return objectScale;
}

//sets scale of object
void Object::setScale(float scale)
{
    objectScale = scale;
}

//draws the object
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
    //draws unit length cube
    case Cube:
        bindTextures();
        glutSolidCube(objectScale);
        unbindTextures();
        break;
    //draws unit length sphere
    case Sphere:
        bindTextures();
        glutSolidSphere(objectScale/2, 16, 16);
        unbindTextures();
        break;
    //draws unit length octahedron
    case Octahedron:
        glPushMatrix();
        glScalef(objectScale/2, objectScale/2, objectScale/2);
        bindTextures();
        glutSolidOctahedron();
        unbindTextures();
        glPopMatrix();
        break;
    //draws unit length cone
    case Cone:
        glPushMatrix();
        glTranslatef(0, 0, -objectScale/2);
        bindTextures();
        glutSolidCone(objectScale/2, objectScale, 16, 16);
        unbindTextures();
        glPopMatrix();
        break;
    //draws unit length torus
    case Torus:
        glPushMatrix();
        glScalef(1, 1, 3);
        bindTextures();
        glutSolidTorus(objectScale/3-objectScale/6, objectScale/3, 16, 16);
        unbindTextures();
        glPopMatrix();
        break;
    //draws unit length house
    case House:
        glPushMatrix();
        glScalef(objectScale/2, objectScale/2, objectScale/2);
        bindTextures();
        drawHouse(false);
        unbindTextures();
        glPopMatrix();
        break;
    }

    //draws wireframe for selected object
    if (isSelected)
    {
        glColor3f(1, 0, 0); //color wireframe red
        switch(type)
        {
        //draws unit length cube
        case Cube:
            glutWireCube(objectScale);
            break;
        //draws unit length sphere
        case Sphere:
            glutWireSphere(objectScale/2, 16, 16);
            break;
        //draws unit length octahedron
        case Octahedron:
            glPushMatrix();
            glScalef(objectScale/2, objectScale/2, objectScale/2);
            glutWireOctahedron();
            glPopMatrix();
            break;
        //draws unit length cone
        case Cone:
            glPushMatrix();
            glTranslatef(0, 0, -objectScale/2);
            glutWireCone(objectScale/2, objectScale, 16, 16);
            glPopMatrix();
            break;
        //draws unit length torus
        case Torus:
            glPushMatrix();
            glScalef(1, 1, 3);
            glutWireTorus(objectScale/3-objectScale/6, objectScale/3, 16, 16);
            glPopMatrix();
            break;
        //draws unit length house
        case House:
            glPushMatrix();
            glScalef(objectScale/2, objectScale/2, objectScale/2);
            drawHouse(true);
            glPopMatrix();
            break;
        }
    }
    glPopMatrix();
}

//stores texture to myTex1
void Object::setTexture1(GLuint* texture1)
{
    myTex1 = texture1;
}

//stores texture to myTex2
void Object::setTexture2(GLuint* texture2)
{
    myTex2 = texture2;
}

//stores texture to myTex3
void Object::setTexture3(GLuint* texture3)
{
    myTex3 = texture3;
}

//binds texture to shape; called before glutSolid____ or drawHouse()
void Object::bindTextures()
{
    if (texture != 0)
    {
        glEnable(GL_TEXTURE_GEN_S);     //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        switch(texture)
        {
        case 1:
            glBindTexture(GL_TEXTURE_2D, *myTex1);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, *myTex2);
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D, *myTex3);
            break;
        }
    }
}

//unbind all textures; called after glutSolid_____ or drawHouse()
void Object::unbindTextures()
{
    glDisable(GL_TEXTURE_GEN_S);    //enable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, 0);    //unbind texture
}
