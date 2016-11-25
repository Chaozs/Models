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

float eye[] = {10, 10, 10};             //initial camera location
float lookAt[] {0,0,0};                 //point camera is looking at
float lightPos1[] = {-70, 38, -70, 1};  //initial light0 position
float lightPos2[] = {70, 38, 70, 1};    //initial light1 positon
float xAxisRotation = 0;                //rotation around x axis
float yAxisRotation = 0;                //rotation around y axis
int sceneSize = 100;                    //total size of the scene
float scaleFactor = 0.5/(sceneSize/50); //scales scene to fit the screen
list<Object> objectList;
Object *selectedObject;

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        yAxisRotation--;        //rotate y-axis in negative direction
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
        yAxisRotation++;        //rotate y-axis in positive direction
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL && xAxisRotation < 45)
    {
        xAxisRotation++;        //rotate x-axis in positive direction
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL && xAxisRotation > -45)
    {
        xAxisRotation--;        //rotate x-axis in negative direction
    }
    else if (key == GLUT_KEY_LEFT)
    {
        if (selectedObject != 0)
        {
            cout << "Object is selected. Type = " << selectedObject->getType() << endl;
            selectedObject->setPosition(selectedObject->getPosX()-1, selectedObject->getPosY(), selectedObject->getPosZ());
        }
    }
    else if (key == GLUT_KEY_RIGHT)
    {
    }
    else if (key == GLUT_KEY_UP)
    {
    }
    else if (key == GLUT_KEY_DOWN)
    {
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
    //move light0 left
    case 'a':
        ;
        lightPos1[0]-=8;
        break;
    //move light0 right
    case 'd':
        lightPos1[0]+=8;
        break;
    //move light0 up
    case 'w':
        lightPos1[1]+=8;
        break;
    //move light0 down
    case 's':
        lightPos1[1]-=8;
        break;
    case '6':
    {
        selectedObject = new Object(Object::Cube);
        objectList.push_back(*selectedObject);
    }
    break;
    case '7':
    {
        selectedObject = new Object(Object::Sphere);
        objectList.push_back(*selectedObject);
    }
    break;
    case '8':
    {
        selectedObject = new Object(Object::Teapot);
        objectList.push_back(*selectedObject);
    }
    break;
    case '9':
    {
        selectedObject = new Object(Object::Cone);
        objectList.push_back(*selectedObject);
    }
    break;
    case '0':
    {
        selectedObject = new Object(Object::Torus);
        objectList.push_back(*selectedObject);
    }
    break;
    }
    glutPostRedisplay();
}


//initialize
void init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.3, 0.3, 0.3, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    gluPerspective(45, 1, 1, 1000);

    //enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}

//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //set light colours
    float diff0[4] = {1.0, 0.1,0.1, 1};
    float diff1[4] = {0.6,0.3,0.1,1};

    float amb0[4] = {0.8,0.3, 0.1, 1};
    float amb1[4] = {0.8,0.5, 0.1, 1};

    float spec0[4] = {0.5,0.3, 0.8, 1};
    float spec1[4] = {0.5,0.3, 0.8, 1};

    //set light position and properties
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

    //set initial camera position and direction
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);

    //handle camera movement
    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glRotatef(xAxisRotation, 1, 0, 0);
    glRotatef(yAxisRotation, 0, 1, 0);
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(0,-1,0);
    glScalef(1,0.01,1);
    glutSolidCube(100); // draws scene
    glPopMatrix();

    for(list<Object>::iterator it=objectList.begin(); it != objectList.end(); ++it)
    {
        Object obj = *it;
        obj.drawObject();
    }
    glPopMatrix();

    glutSwapBuffers();
}

/* prints control instructions to console */
void printInstructions()
{
    cout << "MODELLER" << endl;
    cout << "Susan Yuen / yuens2 / 001416198" << endl;
    cout << "Thien Trandinh / trandit / 001420634" << endl << endl;
    cout << "*CTRL + ARROW KEYS = control camera movement" << endl;
    cout << "*ARROW KEYS = control translation of currently selected object" << endl;
    cout << "q or ESC = exits the program" << endl;
    cout << "WASD = control movement of light source" << endl;
    cout << "KEYS 6 to 0 = creates a cube, sphere, teapot, cone, torus respectively" << endl;

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

    glEnable(GL_DEPTH_TEST);
    init();
    glutMainLoop();                     //starts the event loop

    return(0);
}
