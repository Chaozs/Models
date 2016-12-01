/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#  include <GL/freeglut.h>
#endif

#include "Object.h"
#include "MaterialStruct.h"
#include "SaveLoadStates.h"
#define BOUND_OFFSET 0.55

float camPos[] = {10, 10, 10};	            //initial camera location
float camUp[] = {0, 1, 0};                  //up vector of the camera
float camTarget[] = {0, 0, 0};	            //point camera is looking at
float camSpeed = 0.1f;
float light0Pos[] = {-5, 3, 0, 1};          //initial light0 position
float light1Pos[] = {5, 3, 0, 1};           //initial light1 positon
int materialCounter = 1;                    //default material is brass
list<Object*> objectList;                   //list of object addresses
Object* selectedObject;                     //pointer to currently selected object
int WINDOW_SIZE_HEIGHT = 800;
int WINDOW_SIZE_WIDTH = 800;
int mouseX = 0, mouseY = 0;                 //global vars to save mouse x/y coord

//*********************Texture***************
int height, width, k;
GLubyte* img_data;
//*********************Texture***************

struct TeapotStruct{
    float posX, posY, posZ;
    bool intersect;
} teapot;


void setMaterial(int i)
{
    MaterialStruct current;
    switch(i)
    {
    case 0:
        current = redPlastic;
        break;
    case 1:
        current = brass;
        break;
    case 2:
        current = chrome;
        break;
    case 3:
        current = silver;
        break;
    case 4:
        current = greenRubber;
        break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, current.ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, current.diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, current.specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, current.shininess);
}

//calculate weather an intersection of our ray hits the teapot
void CalcIntersections(){
    //---Construct ray-----------------------------------------------------
    //construct Ray
    GLdouble R0[3], R1[3], Rd[3];
    GLdouble modelMat[16], projMat[16];
    GLint viewMat[4];

    //populate mpv matricies
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    glGetIntegerv(GL_VIEWPORT, viewMat);

    //calculate near point
    gluUnProject(mouseX, mouseY, 0.0, modelMat, projMat, viewMat, &R0[0], &R0[1], &R0[2]);
    //calculate far point
    gluUnProject(mouseX, mouseY, 1.0, modelMat, projMat, viewMat, &R1[0], &R1[1], &R1[2]);

    //calcualte our ray from R0 and R1
    Rd[0] = R1[0] - R0[0];
    Rd[1] = R1[1] - R0[1];
    Rd[2] = R1[2] - R0[2];

    //turn ray Rd into unit ray 
    GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
    Rd[0] /= m;
    Rd[1] /= m;
    Rd[2] /= m;

    printf("R0: %f, %f, %f | ", R0[0], R0[1], R0[2]);
    printf("R1: %f, %f, %f | ", R1[0], R1[1], R1[2]);
    printf("Rd: %f, %f, %f | ", Rd[0], Rd[1], Rd[2]);

    //---calculate intersection point now-----------------------------------
    //approx the teapot with a box of radius 1 centered around the teapot centered
    //goes against the xy plane to test the Intersection
    //NOTE: this is not the code from slides, but rather proof of concept
    //using assumtions which are true for this example only. 

    //calculate t value from z dir;
    double t = (((double)teapot.posZ) - R0[2])/Rd[2];

    printf("t: %f | ", t);

    //use t value to find x and y of our intersection point
    double pt[3];
    pt[0] = R0[0] + t * Rd[0];
    pt[1] = R0[1] + t * Rd[1];
    pt[2] = teapot.posZ;
    
    printf("pt: %f, %f, %f | ", pt[0], pt[1], pt[2]);

    //now that we have our point on the xy plane at the level of the teapot,
    //use it to see if this point is inside a box centered at the teapots
    //location
    if(pt[0] > teapot.posX - BOUND_OFFSET && pt[0] < teapot.posX + BOUND_OFFSET &&
        pt[1] > teapot.posY - BOUND_OFFSET && pt[1] < teapot.posY + BOUND_OFFSET &&
        pt[2] > teapot.posZ - BOUND_OFFSET && pt[2] < teapot.posZ + BOUND_OFFSET)
        teapot.intersect = true;
    else
        teapot.intersect = false;

    printf("\n");
}


//**************************************************************Texture***************
GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
    GLubyte* img;
    FILE *fd;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;

    /* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
    fd = fopen(file, "r");
    fscanf(fd,"%[^\n] ",b);
    if(b[0]!='P'|| b[1] != '3')
    {
        exit(0);
    }
    fscanf(fd, "%c",&c);

    /* next, skip past the comments - any line starting with #*/
    while(c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        fscanf(fd, "%c",&c);
    }
    ungetc(c,fd);

    /* now get the dimensions and max colour value from the image */
    fscanf(fd, "%d %d %d", &n, &m, &k);

    /* calculate number of pixels and allocate storage for this */
    nm = n*m;
    img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    s=255.0/k;

    /* for every pixel, grab the read green and blue values, storing them in the image data array */
    for(i=0; i<nm; i++)
    {
        fscanf(fd,"%d %d %d",&red, &green, &blue );
        img[3*nm-3*i-3]=red*s;
        img[3*nm-3*i-2]=green*s;
        img[3*nm-3*i-1]=blue*s;
    }

    /* finally, set the "return parameters" (width, height, max) and return the image array */
    *width = n;
    *height = m;
    *max = k;

    return img;
}
//****************************************************************Texture***************

void special(int key, int x, int y)
{
    //translation of objects
    if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosX() > -50)
        {
            selectedObject->setPosition(selectedObject->getPosX()-0.3,
                                        selectedObject->getPosY(), selectedObject->getPosZ()); //translate object in -x direction
            selectedObject->setMinPointX(selectedObject->getMinPointX()-0.3);
            selectedObject->setMaxPointX(selectedObject->getMaxPointX()-0.3);
        }
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosX() < 50)
        {
            selectedObject->setPosition(selectedObject->getPosX()+0.3,
                                        selectedObject->getPosY(), selectedObject->getPosZ()); //translate object in +x direction
            selectedObject->setMinPointX(selectedObject->getMinPointX()+0.3);
            selectedObject->setMaxPointX(selectedObject->getMaxPointX()+0.3);
        }
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosZ() < 50)
        {
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY(), selectedObject->getPosZ()+0.3); //translate object in +z direction
            selectedObject->setMinPointZ(selectedObject->getMinPointZ()+0.3);
            selectedObject->setMaxPointZ(selectedObject->getMaxPointZ()+0.3);
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosZ() > -50)
        {
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY(), selectedObject->getPosZ()-0.3); //translate object in -z direction
            selectedObject->setMinPointZ(selectedObject->getMinPointZ()-0.3);
            selectedObject->setMaxPointZ(selectedObject->getMaxPointZ()-0.3);
        }
    }
    else if (key == GLUT_KEY_PAGE_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY()+0.3, selectedObject->getPosZ()); //translate object in +y direction
            selectedObject->setMinPointY(selectedObject->getMinPointY()+0.3);
            selectedObject->setMaxPointY(selectedObject->getMaxPointY()+0.3);
        }
    }
    else if (key == GLUT_KEY_PAGE_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosY() > 0)
        {
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY()-0.3, selectedObject->getPosZ()); //translate object in -y direction
            selectedObject->setMinPointY(selectedObject->getMinPointY()-0.3);
            selectedObject->setMaxPointY(selectedObject->getMaxPointY()-0.3);
        }
    }

    //scaling of objects
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        if (selectedObject != 0 && selectedObject->getScale() < 50)
        {
            float pointScaleChange = 0.05;
            selectedObject->setScale(selectedObject->getScale()+0.1);   //upscale object
            selectedObject->setMinPointX(selectedObject->getMinPointX()-pointScaleChange);
            selectedObject->setMinPointY(selectedObject->getMinPointY()-pointScaleChange);
            selectedObject->setMinPointZ(selectedObject->getMinPointZ()-pointScaleChange);
            selectedObject->setMaxPointX(selectedObject->getMaxPointX()+pointScaleChange);
            selectedObject->setMaxPointY(selectedObject->getMaxPointY()+pointScaleChange);
            selectedObject->setMaxPointZ(selectedObject->getMaxPointZ()+pointScaleChange);
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        if (selectedObject != 0 && selectedObject->getScale() > 0.01)
        {
            float pointScaleChange = 0.05;
            selectedObject->setScale(selectedObject->getScale()-0.1);   //downscale object
            selectedObject->setMinPointX(selectedObject->getMinPointX()+pointScaleChange);
            selectedObject->setMinPointY(selectedObject->getMinPointY()+pointScaleChange);
            selectedObject->setMinPointZ(selectedObject->getMinPointZ()+pointScaleChange);
            selectedObject->setMaxPointX(selectedObject->getMaxPointX()-pointScaleChange);
            selectedObject->setMaxPointY(selectedObject->getMaxPointY()-pointScaleChange);
            selectedObject->setMaxPointZ(selectedObject->getMaxPointZ()-pointScaleChange);
        }
    }

    //rotation of objects
    else if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX()-2,
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ());  //rotate in -x axis
        }
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX()+2,
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ());  //rotate in +x axis
        }
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ()+2);  //rotate in +z axis
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ()-2);  //rotate in -z axis
        }
    }
    else if (key == GLUT_KEY_PAGE_UP && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY()+2,
                                           selectedObject->getOrientationZ());  //rotate in +y axis
        }
    }
    else if (key == GLUT_KEY_PAGE_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY()-2,
                                           selectedObject->getOrientationZ());  //rotate in -y axis
        }
    }

    //camera control
    else if (key == GLUT_KEY_LEFT)
    {
        camPos[0] = camPos[0]*cos(0.03)-camPos[2]*sin(0.03);    //rotate around y-axis in positive direction
        camPos[2] = camPos[0]*sin(0.03)+camPos[2]*cos(0.03);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        camPos[0] = camPos[0]*cos(-0.03)-camPos[2]*sin(-0.03);  //rotate around y-axis in negative direction
        camPos[2] = camPos[0]*sin(-0.03)+camPos[2]*cos(-0.03);
    }
    else if (key == GLUT_KEY_UP)
    {
        camPos[0] += camSpeed;      //rotate x-axis in positive direction
    }
    else if (key == GLUT_KEY_DOWN)
    {
        camPos[0] -= camSpeed;      //rotate x-axis in negative direction
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
    case 27:
        exit(0);    //exits program is q or ESC is pressed
        break;
    //detect shift key (capital letters)
    case 'A':
        light1Pos[0]-=2;       //move light1 left
        break;
    case 'D':
        light1Pos[0]+=2;       //move light1 right
        break;
    case 'W':
        light1Pos[1]+=2;       //move light1 up
        break;
    case 'S':
        light1Pos[1]-=2;       //move light1 down
        break;
    case 'r':
        objectList.clear();     // clears the object list
    case 'a':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[0]-=2;    //move light0 left
        }
        break;
    case 'd':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[0]+=2;    //move light0 right
        }
        break;
    case 'w':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[1]+=2;    //move light0 up
        }
        break;
    case 's':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[1]-=2;    //move light0 down
        }
        else
        {
            string fileNameSave = "";
            bool hasDot;
            while (fileNameSave == "")
            {
                cout << "Enter a file name: ";
                cin >> fileNameSave;

                if (fileNameSave.find('.') != string::npos)
                {
                    fileNameSave = "";
                    cout << "invalid file name.";
                }
                else
                {
                    SaveLoadStates::saveState(fileNameSave, objectList);
                    cout << "File has been saved!\n";
                    break;
                }
            }
        }
        break;
    case 'l':
    {
        string fileNameLoad = "";
        while (fileNameLoad == "")
        {
            cout << "Which file would you like to load?: ";
            cin >> fileNameLoad;
            if (fileNameLoad.find('.') != string::npos)
            {
                fileNameLoad = "";
                cout << "Invalid file name. Try again and make sure you haven't added any '.'s\n";
            }
            else
            {
                fileNameLoad = fileNameLoad + ".csv";
                if (FILE *file = fopen(fileNameLoad.c_str(), "r"))
                {

                    fclose(file);
                    objectList.clear();
                    selectedObject = 0;
                    objectList = SaveLoadStates::loadState(fileNameLoad);
                    cout << "File has been loaded!\n";
                    break;
                }
                else
                {
                    cout << "File doesn't exist\n";
                    break;
                }
            }
        }
        break;
    }
    //set current object to current material
    case 'm':
        selectedObject->storeMaterial(materialCounter);
        break;
    //disable texture
    case 'u':
        glDisable(GL_TEXTURE_2D);
        break;
    case 'i':
        img_data = LoadPPM("interface.ppm", &width, &height, &k);
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case 'o':
        img_data = LoadPPM("interface.ppm", &width, &height, &k);
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    case 'p':
        img_data = LoadPPM("interface.ppm", &width, &height, &k);
        glEnable(GL_TEXTURE_2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        break;
    //set material to redPlastic
    case '1':
        materialCounter = 0;
        break;
    //set material to brass
    case '2':
        materialCounter = 1;
        break;
    //set material to chrome
    case '3':
        materialCounter = 2;
        break;
    //set material to silver
    case '4':
        materialCounter = 3;
        break;
    //set material to green rubber
    case '5':
        materialCounter = 4;
        break;
    //create a cube
    case '6':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Cube);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    //create a sphere
    case '7':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Sphere);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    //create a octahedron
    case '8':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Octahedron);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    //create a cone
    case '9':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Cone);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    //create a torus
    case '0':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Torus);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
        break;
    }
    break;
    }
    glutPostRedisplay();
}

/*//calculate whether an intersection of our ray hits the teapot
bool CalcIntersections(Object* object)
{
    //RAY INTERSECTION
    //construct ray
    GLdouble R0[3], R1[3], Rd[3];
    GLdouble modelMat[16], projMat[16];
    GLint viewMat[4];

    //populate mpv matricies
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    glGetIntegerv(GL_VIEWPORT, viewMat);

    gluUnProject(mouseX, mouseY, 0.0, modelMat, projMat, viewMat, &R0[0], &R0[1], &R0[2]);  //calculate near point
    gluUnProject(mouseX, mouseY, 1.0, modelMat, projMat, viewMat, &R1[0], &R1[1], &R1[2]);  //calculate far point

    //calculate our ray from R0 and R1
    Rd[0] = R1[0] - R0[0];
    Rd[1] = R1[1] - R0[1];
    Rd[2] = R1[2] - R0[2];

    //turn ray Rd into unit ray
    GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
    Rd[0] /= m;
    Rd[1] /= m;
    Rd[2] /= m;

    return false;
}*/

//initialize
void init(void)
{
    glClearColor(0.3, 0.3, 0.3, 0);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    //**************************************************************Texture********************************
    glRasterPos2i(width,0);
    glPixelZoom(-1, 1);
    glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, img_data);
    //**************************************************************Texture********************************

    setMaterial(0);

    //enable backface culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

void addLights()
{
    //set light colours
    float diff0[4] = {0, 0.2, 1, 1};            //blue light (light0)
    float diff1[4] = {1, 0.6, 0, 1};            //amber light (light1)
    float amb0[4] = {0.2f, 0.2f, 0.2f, 1};
    float amb1[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec0[4] = {0.2f, 0.2f, 0.2f, 1};
    float spec1[4] = {0.2f, 0.2f, 0.2f, 1};

    //set light position and properties
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);

    //draw sphere for light0
    glColor3f(0, 0.2, 1);
    glPushMatrix();
    glTranslatef(light0Pos[0], light0Pos[1], light0Pos[2]);
    glutWireSphere(0.1, 16, 16);
    glPopMatrix();

    //draw sphere for light1
    glColor3f(1, 0.6, 0);
    glPushMatrix();
    glTranslatef(light1Pos[0], light1Pos[1], light1Pos[2]);
    glutWireSphere(0.1, 16, 16);
    glPopMatrix();
}

//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //set camera position and direction
    gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], camUp[0], camUp[1], camUp[2]);

    addLights();

    setMaterial(0);
    glPushMatrix();
    glColor3f(0.7,0.7,0.7);
    glTranslatef(0,-1,0);
    glScalef(1,0.01,1);
    glutSolidCube(100);     //draws plane
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5,0.5,0.5);
    glTranslatef(-8,45,0);
    glScalef(0.01,1,1);
    glutSolidCube(100);     //draws plane
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.3,0.3,0.3);
    glTranslatef(0,45,-8);
    glScalef(1,1,0.01);
    glutSolidCube(100);     //draws plane
    glPopMatrix();

    CalcIntersections();
    glPushMatrix();
        glTranslatef(teapot.posX, teapot.posY, teapot.posZ);

        if(teapot.intersect)
            glColor3f(1,0,0); //red
        else
            glColor3f(1, 1, 0); // yellow

        glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.5,0.5,0.5);
    //draws all objects
    glFrontFace(GL_CCW);
    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
    glEnable(GL_TEXTURE_GEN_T);
    for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
    {
        Object* objP = *it;
        Object obj = *objP;
        setMaterial(obj.getMaterial());
        obj.drawObject(objP == selectedObject);
    }
    glDisable(GL_TEXTURE_GEN_S); //disable texture coordinate generation
    glDisable(GL_TEXTURE_GEN_T);
    glFrontFace(GL_CW);

    glutSwapBuffers();
}

/* prints control instructions to console */
void printInstructions()
{
    cout << "MODELLER" << endl;
    cout << "Susan Yuen / yuens2 / 001416198" << endl;
    cout << "Thien Trandinh / trandit / 001420634" << endl;
    cout << "* Note: CAPS LOCK must be turned off for the commands to behave correctly" << endl << endl;
    cout << "* ARROW KEYS = control camera movement" << endl;
    cout << "* CTRL + ARROW KEYS/PAGE UP/PAGE DOWN = control translation of currently selected object" << endl;
    cout << "* SHIFT + UP ARROW/DOWN ARROW = manipulate scale of currently selected object" << endl;
    cout << "* ALT + ARROW KEYS/PAGE UP/PAGE DOWN = control rotation of currently selected object" << endl;
    cout << "* q or ESC = exits the program" << endl;
    cout << "* ALT + wasd = control movement of light source 1" << endl;
    cout << "* SHIFT + wasd = control movement of light source 2" << endl;
    cout << "* s = saves the current object scene as a .txt file" << endl;
    cout << "* l = loads the object scene at specified .txt file" << endl;
    cout << "* r = clears the scene" << endl;
    cout << "* KEYS 1 to 5 = changes current drawing material" << endl;
    cout << "* m = applies current drawing material to selected object" << endl;
    cout << "* KEYS 6 to 0 = creates a cube, sphere, octahedron, cone, torus respectively" << endl;
    cout << "* u = disables textures" << endl;
    cout << "* i, o, p = enables various textures" << endl << endl;

}

//save our mouse coords when they change
void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //handle camera movement
        mouseX = x;
        mouseY = WINDOW_SIZE_HEIGHT - y;
    }
    glutPostRedisplay();
}

//main method
int main(int argc, char** argv)
{
    printInstructions();

    glutInit(&argc, argv);              //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("3GC3 Assignment 3");  //creates the window with game name
    glutDisplayFunc(display);           //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    init();
    glutMainLoop();                     //starts the event loop

    return(0);
}
