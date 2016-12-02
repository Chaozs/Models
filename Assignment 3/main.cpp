/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include <stdio.h>
#include <stdlib.h>
#include <list>
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

/* CAMERA */
float camPos[] = {10, 20, 10};          //initial camera location
float camUp[] = {0, 1, 0};              //up vector of the camera
float camTarget[] = {0, 0, 0};          //point camera is looking at

/* LIGHTING */
float light0Pos[] = {-5, 3, 0, 1};      //initial light0 position
float light1Pos[] = {5, 3, 0, 1};       //initial light1 positon

/* MATERIAL */
int materialCounter = 1;                //default material is brass

/* OBJECTS */
list<Object*> objectList;               //list of object addresses
Object* selectedObject;                 //pointer to currently selected object

/* WINDOW */
int WINDOW_SIZE_HEIGHT = 800;
int WINDOW_SIZE_WIDTH = 800;
int mouseX = 0, mouseY = 0;             //global vars to save mouse x/y coord

/* TEXTURE */
int height, width, k;
GLubyte* image;
GLubyte* image2;
GLubyte* image3;
GLuint myTex[3];

/* Sets the current to the specified material Enum */
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

/* Calculates whether an intersection of our ray hits the specified object */
bool isClicked(Object* obj)
{
    /* CONSTRUCT RAY */
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

    //calculate our ray from R0 and R1
    Rd[0] = R1[0] - R0[0];
    Rd[1] = R1[1] - R0[1];
    Rd[2] = R1[2] - R0[2];

    //turn ray Rd into unit ray
    GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
    Rd[0] /= m;
    Rd[1] /= m;
    Rd[2] /= m;

    /* INTERSECTION POINT */
    //approx the object with a hitbox of radius objectScale/2 centered around the teapot centered

    //calculate t value for all three directions
    double tX = (((double)obj->getPosX()) - R0[0])/Rd[0];
    double tY = (((double)obj->getPosY()+1) - R0[1])/Rd[1];
    double tZ = (((double)obj->getPosZ()) - R0[2])/Rd[2];

    //use t value to find x, y, and z of our intersection point
    double ptX[3];
    ptX[0] = obj->getPosX();
    ptX[1] = R0[1] + tX * Rd[1];
    ptX[2] = R0[2] + tX * Rd[2];

    double ptY[3];
    ptY[0] = R0[0] + tY * Rd[0];
    ptY[1] = obj->getPosY()+1;
    ptY[2] = R0[2] + tY * Rd[2];

    double ptZ[3];
    ptZ[0] = R0[0] + tZ * Rd[0];
    ptZ[1] = R0[1] + tZ * Rd[1];
    ptZ[2] = obj->getPosZ();

    //check each plane (XY, ZY, XZ)
    if(ptX[0] > obj->getPosX() - obj->getScale()/2 &&
            ptX[0] < obj->getPosX() + obj->getScale()/2 &&
            ptX[1] > obj->getPosY() - obj->getScale()/2 &&
            ptX[1] < obj->getPosY() + obj->getScale()/2 &&
            ptX[2] > obj->getPosZ() - obj->getScale()/2 &&
            ptX[2] < obj->getPosZ() + obj->getScale()/2)
    {
        return true;
    }

    if(ptY[0] > obj->getPosX() - obj->getScale()/2 &&
            ptY[0] < obj->getPosX() + obj->getScale()/2 &&
            ptY[1] > obj->getPosY() - obj->getScale()/2 &&
            ptY[1] < obj->getPosY() + obj->getScale()/2 &&
            ptY[2] > obj->getPosZ() - obj->getScale()/2 &&
            ptY[2] < obj->getPosZ() + obj->getScale()/2)
    {
        return true;
    }

    if(ptZ[0] > obj->getPosX() - obj->getScale()/2 &&
            ptZ[0] < obj->getPosX() + obj->getScale()/2 &&
            ptZ[1] > obj->getPosY() - obj->getScale()/2 &&
            ptZ[1] < obj->getPosY() + obj->getScale()/2 &&
            ptZ[2] > obj->getPosZ() - obj->getScale()/2 &&
            ptZ[2] < obj->getPosZ() + obj->getScale()/2)
    {
        return true;
    }
    return false;
}

/* Loads PPM file (for texture) */
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
        printf("%s\n",b);
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

/* handles special keyboard input */
void special(int key, int x, int y)
{
    /* OBJECT TRANSLATION */
    if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            //translate object in -x direction
            selectedObject->setPosition(selectedObject->getPosX()-0.3,
                                        selectedObject->getPosY(),
                                        selectedObject->getPosZ());
        }
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            //translate object in +x direction
            selectedObject->setPosition(selectedObject->getPosX()+0.3,
                                        selectedObject->getPosY(),
                                        selectedObject->getPosZ());
        }
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            //translate object in +z direction
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY(),
                                        selectedObject->getPosZ()+0.3);
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            //translate object in -z direction
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY(),
                                        selectedObject->getPosZ()-0.3);
        }
    }
    else if (key == GLUT_KEY_PAGE_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0)
        {
            //translate object in +y direction
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY()+0.3,
                                        selectedObject->getPosZ());
        }
    }
    else if (key == GLUT_KEY_PAGE_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        if (selectedObject != 0 && selectedObject->getPosY() > 0)
        {
            //translate object in -y direction
            selectedObject->setPosition(selectedObject->getPosX(),
                                        selectedObject->getPosY()-0.3,
                                        selectedObject->getPosZ());
        }
    }

    /* OBJECT SCALE */
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        if (selectedObject != 0 && selectedObject->getScale() < 50)
        {
            //upscale object
            selectedObject->setScale(selectedObject->getScale()+0.1);
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT)
    {
        if (selectedObject != 0 && selectedObject->getScale() > 0.01)
        {
            //downscale object
            selectedObject->setScale(selectedObject->getScale()-0.1);
        }
    }

    /* OBJECT ROTATION */
    else if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in -x axis
            selectedObject->setOrientation(selectedObject->getOrientationX()-2,
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ());
        }
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in +x axis
            selectedObject->setOrientation(selectedObject->getOrientationX()+2,
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ());
        }
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in +z axis
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ()+2);
        }
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in -z axis
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY(),
                                           selectedObject->getOrientationZ()-2);
        }
    }
    else if (key == GLUT_KEY_PAGE_UP && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in +y axis
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY()+2,
                                           selectedObject->getOrientationZ());
        }
    }
    else if (key == GLUT_KEY_PAGE_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            //rotate in -y axis
            selectedObject->setOrientation(selectedObject->getOrientationX(),
                                           selectedObject->getOrientationY()-2,
                                           selectedObject->getOrientationZ());
        }
    }

    /* CAMERA CONTROL */
    else if (key == GLUT_KEY_LEFT)
    {
        //rotate around y-axis in positive direction
        camPos[0] = camPos[0]*cos(0.03)-camPos[2]*sin(0.03);
        camPos[2] = camPos[0]*sin(0.03)+camPos[2]*cos(0.03);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        //rotate around y-axis in negative direction
        camPos[0] = camPos[0]*cos(-0.03)-camPos[2]*sin(-0.03);
        camPos[2] = camPos[0]*sin(-0.03)+camPos[2]*cos(-0.03);
    }
    else if (key == GLUT_KEY_UP && camPos[0]*sin(0.03)+camPos[1]*cos(0.03) > 0)
    {
        //rotate z-axis in positive direction
        camPos[0] = camPos[0]*cos(0.03)-camPos[1]*sin(0.03);
        camPos[1] = camPos[0]*sin(0.03)+camPos[1]*cos(0.03);
    }
    else if (key == GLUT_KEY_DOWN && camPos[0]*sin(-0.03)+camPos[1]*cos(-0.03) > 0)
    {
        //rotate z-axis in negative direction
        camPos[0] = camPos[0]*cos(-0.03)-camPos[1]*sin(-0.03);
        camPos[1] = camPos[0]*sin(-0.03)+camPos[1]*cos(-0.03);
    }
    glutPostRedisplay();
}

/* handles keyboard input */
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
        light1Pos[0]-=0.2;        //move light1 left
        break;
    case 'D':
        light1Pos[0]+=0.2;        //move light1 right
        break;
    case 'W':
        light1Pos[1]+=0.2;        //move light1 up
        break;
    case 'S':
        light1Pos[1]-=0.2;        //move light1 down
        break;
    case 'r':
        objectList.clear();     // clears the object list
    case 'a':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[0]-=0.2;    //move light0 left
        }
        break;
    case 'd':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[0]+=0.2;    //move light0 right
        }
        break;
    case 'w':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[1]+=0.2;    //move light0 up
        }
        break;
    case 's':
        if (glutGetModifiers() & GLUT_ACTIVE_ALT)
        {
            light0Pos[1]-=0.2;    //move light0 down
        }
        else
        {
            string fileNameSave = "";

            while (fileNameSave == "")
            {
                cout << "Enter file name for save file ";   //ask for save file name
                cin >> fileNameSave;

                if (fileNameSave.find('.') != string::npos) //checks that file name doesnt contain '.'
                {
                    fileNameSave = "";
                    cout << "Please enter a valid file name";
                }
                else
                {
                    //passes in save file name and object list into saveState
                    SaveLoadStates::saveState(fileNameSave, objectList);
                    cout << "Save Complete!\n";
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
            cout << "Please enter the name of the save file ";  //ask for save file name
            cin >> fileNameLoad;
            if (fileNameLoad.find('.') != string::npos)         //checks that file name doesnt contain '.'
            {
                fileNameLoad = "";
                cout << "invalid file name \n";
            }
            else
            {
                fileNameLoad = fileNameLoad + ".csv";
                if (FILE *file = fopen(fileNameLoad.c_str(), "r"))
                {
                    fclose(file);
                    objectList.clear();     //empty list before loading
                    selectedObject = 0;     //unselect all objects
                    objectList = SaveLoadStates::loadState(fileNameLoad);   //loads the objectList stored in save file

                    //sets texture for each object loaded
                    for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
                    {
                        Object* objP = *it;
                        objP->setTexture1(&myTex[0]);
                        objP->setTexture2(&myTex[1]);
                        objP->setTexture3(&myTex[2]);
                    }

                    cout << "File has been loaded!\n";
                    break;
                }
                else    //if no file is found with given name
                {
                    cout << "File not found \n";
                    break;
                }
            }
        }
        break;
    }
    //set current object to current material
    case 'm':
        if (selectedObject != 0)
        {
            selectedObject->storeMaterial(materialCounter);
        }
        break;
    //disable texture
    case 'u':
        if (selectedObject != 0)
        {
            selectedObject->setTexture(0);
        }
        break;
    //applies texture
    case 'i':
        if (selectedObject != 0)
        {
            selectedObject->setTexture(1);
        }
        break;
    //applies texture
    case 'o':
        if (selectedObject != 0)
        {
            selectedObject->setTexture(2);
        }
        break;
    //applies texture
    case 'p':
        if (selectedObject != 0)
        {
            selectedObject->setTexture(3);
        }
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
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
    }
    break;
    //create a sphere
    case '7':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Sphere);
        selectedObject->storeMaterial(materialCounter);
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
    }
    break;
    //create a octahedron
    case '8':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Octahedron);
        selectedObject->storeMaterial(materialCounter);
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
    }
    break;
    //create a cone
    case '9':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Cone);
        selectedObject->storeMaterial(materialCounter);
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
    }
    break;
    //create a torus
    case '0':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::Torus);
        selectedObject->storeMaterial(materialCounter);
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
        break;
    }
    //create a house shape
    case 'y':
    {
        selectedObject = new Object();
        selectedObject->setType(Object::House);
        selectedObject->storeMaterial(materialCounter);
        selectedObject->setTexture1(&myTex[0]);
        selectedObject->setTexture2(&myTex[1]);
        selectedObject->setTexture3(&myTex[2]);
        objectList.push_back(selectedObject);
        break;
    }
    break;
    }
    glutPostRedisplay();
}

/* Initialize */
void init(void)
{
    glClearColor(0, 0, 0, 0);       //set clear colour to black

    glEnable(GL_COLOR_MATERIAL);    //enable colour material

    //enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);

    glEnable(GL_DEPTH_TEST);    //enables z buffer

    //set material
    setMaterial(0);

    //enable backface culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    /* TEXTURE */
    glEnable(GL_TEXTURE_2D);
    glGenTextures(3, myTex);

    /* Set the image parameters*/
    glBindTexture(GL_TEXTURE_2D, myTex[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*Get and save image*/
    image = LoadPPM("interface.ppm", &width, &height, &k);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Set the image parameters*/
    glBindTexture(GL_TEXTURE_2D, myTex[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*Get and save image*/
    image2 = LoadPPM("marble.ppm", &width, &height, &k);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Set the image parameters*/
    glBindTexture(GL_TEXTURE_2D, myTex[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /*Get and save image*/
    image3 = LoadPPM("snail_a.ppm", &width, &height, &k);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glMatrixMode(GL_TEXTURE);
}

/* Adds lights to the scene */
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

/* display method to be recalled upon any change to the scene */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //set camera position and direction
    gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], camUp[0], camUp[1], camUp[2]);

    addLights();            //add lights

    glFrontFace(GL_CW);     //backface culling (clockwise)

    //draws plane (floor)
    setMaterial(0);
    glPushMatrix();
    glColor3f(0.7,0.7,0.7);
    glTranslatef(0,-1,0);
    glScalef(1,0.01,1);
    glutSolidCube(100);
    glPopMatrix();

    //draws all objects
    glColor3f(0.5,0.5,0.5);         //set all objects default colour to grey
    glFrontFace(GL_CCW);            //backface culling (counterclockwise)
    for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
    {
        Object* objP = *it;
        Object obj = *objP;
        setMaterial(obj.getMaterial());
        obj.drawObject(objP == selectedObject); //draws each object in object list
    }

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
    cout << "* KEYS 6 to 0 = creates a cube, sphere, octahedron, cone, torus respectively" << endl;
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
    cout << "* u = disables textures" << endl;
    cout << "* EXTRA FEATURE: y = creates custom object (house)" << endl;
    cout << "* EXTRA BONUS FEATURE: i, o, p = enables various textures; where i=interface, o=marble, p=snail" << endl << endl;
}

/* save our mouse coords when they change */
void mouse(int btn, int state, int x, int y)
{
    //left-click selects object
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseX = x;
        mouseY = WINDOW_SIZE_HEIGHT - y;
        for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
        {
            if (isClicked(*it))
            {
                selectedObject = *it;   //set selected object if object is clicked
            }
        }
    }
    //right click deletes object
    else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        bool deleteObject = false;      //indicates whether an object is clicked for deletion
        mouseX = x;
        mouseY = WINDOW_SIZE_HEIGHT - y;
        for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
        {
            if (isClicked(*it))
            {
                selectedObject = *it;
                deleteObject = true;
            }
        }
        if (deleteObject)
        {
            objectList.remove(selectedObject);  //deleted clicked object
        }
    }
    glutPostRedisplay();
}

/* main method */
int main(int argc, char** argv)
{
    printInstructions();

    glutInit(&argc, argv);                  //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("3GC3 Assignment 3");  //creates the window with game name
    glutDisplayFunc(display);               //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    init();
    glutMainLoop();                         //starts the event loop

    return(0);
}
