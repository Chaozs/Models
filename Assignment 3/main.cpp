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

float eye[] = {2, 2, 2};                 //initial camera location
float lookAt[] {0,0,0};                     //point camera is looking at
float light0Pos[] = {-70, 38, -70, 1};      //initial light0 position
float light1Pos[] = {40, 38, 70, 1};        //initial light1 positon
float xAxisRotation = 0;                    //rotation around x axis
float yAxisRotation = 0;                    //rotation around y axis
int sceneSize = 100;                        //total size of the scene
float scaleFactor = 0.5/(sceneSize/50);     //scales scene to fit the screen
int materialCounter = 0;
list<Object*> objectList;                   //list of object addresses
Object* selectedObject;                     //pointer to currently selected object
void saveState(string fileName);
void fileLoad(string fileName);

//Material structure
struct Material
{
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
};
//list 5 different materials
Material redPlastic =
{
    {0.3, 0.0, 0.0, 1.0},
    {0.6, 0.0, 0.0, 1.0},
    {0.8, 0.6, 0.6, 1.0},
    0.25
};
Material brass =
{
    {0.33f, 0.22f, 0.27f, 1.f},
    {0.78f, 0.57f, 0.11f, 1.f},
    {0.99f, 0.94f, 0.8f, 1.f},
    0.22f
};
Material chrome =
{
    {0.25f, 0.25f, 0.25f, 1.f},
    {0.4f, 0.4f, 0.4f, 1.f},
    {0.75f, 0.75f, 0.75f, 1.f},
    0.6f
};
Material silver =
{
    {0.2f, 0.2f, 0.2f, 1.f},
    {0.51f, 0.51f, 0.51f, 1.f},
    {0.51f, 0.51f, 0.51f, 1.f},
    0.4f
};
Material greenRubber =
{
    {0.0f, 0.05f, 0.f, 1.f},
    {0.4f, 0.5f, 0.4f, 1.f},
    {0.04f, 0.7f, 0.04f, 1.f},
    0.078f
};

void setMaterial(int i)
{
    Material current;
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

void special(int key, int x, int y)
{
    //translation of objects
    if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0 && selectedObject->getPosX() > -50)
    {
        selectedObject->setPosition(selectedObject->getPosX()-0.3,
                                    selectedObject->getPosY(), selectedObject->getPosZ()); //translate object in -x direction
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0 && selectedObject->getPosX() < 50)
    {
        selectedObject->setPosition(selectedObject->getPosX()+0.3,
                                    selectedObject->getPosY(), selectedObject->getPosZ()); //translate object in +x direction
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0 && selectedObject->getPosZ() < 50)
    {
        selectedObject->setPosition(selectedObject->getPosX(),
                                    selectedObject->getPosY(), selectedObject->getPosZ()+0.3); //translate object in +z direction
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0 && selectedObject->getPosZ() > -50)
    {
        selectedObject->setPosition(selectedObject->getPosX(),
                                    selectedObject->getPosY(), selectedObject->getPosZ()-0.3); //translate object in -z direction
    }
    else if (key == GLUT_KEY_PAGE_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0)
    {
        selectedObject->setPosition(selectedObject->getPosX(),
                                    selectedObject->getPosY()+0.3, selectedObject->getPosZ()); //translate object in +y direction
    }
    else if (key == GLUT_KEY_PAGE_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL && selectedObject != 0 && selectedObject->getPosY() > 0)
    {
        selectedObject->setPosition(selectedObject->getPosX(),
                                    selectedObject->getPosY()-0.3, selectedObject->getPosZ()); //translate object in -y direction
    }


    //scaling of objects
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_SHIFT && selectedObject != 0 && selectedObject->getScale() < 50)
    {
        selectedObject->setScale(selectedObject->getScale()+0.1);   //upscale object
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_SHIFT && selectedObject != 0 && selectedObject->getScale() > 0.01)
    {
        selectedObject->setScale(selectedObject->getScale()-0.1);   //downscale object
    }

    //rotation of objects
    else if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_ALT && selectedObject != 0)
    {
        selectedObject->setOrientation(selectedObject->getOrientationX()-1,
                                       selectedObject->getOrientationY(), selectedObject->getOrientationZ());  //rotate in -x axis
    }
    else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        if (selectedObject != 0)
        {
            selectedObject->setOrientation(selectedObject->getOrientationX()+1,
                                           selectedObject->getOrientationY(), selectedObject->getOrientationZ());  //rotate in +x axis
        }
    }
    else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_ALT && selectedObject != 0)
    {
        selectedObject->setOrientation(selectedObject->getOrientationX(),
                                       selectedObject->getOrientationY(), selectedObject->getOrientationZ()+1);  //rotate in +z axis
    }
    else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_ALT && selectedObject != 0)
    {
        selectedObject->setOrientation(selectedObject->getOrientationX(),
                                       selectedObject->getOrientationY(), selectedObject->getOrientationZ()-1);  //rotate in -z axis
    }
    else if (key == GLUT_KEY_PAGE_UP && selectedObject != 0 && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        selectedObject->setOrientation(selectedObject->getOrientationX(),
                                       selectedObject->getOrientationY()+1, selectedObject->getOrientationZ());  //rotate in +y axis
    }
    else if (key == GLUT_KEY_PAGE_DOWN && selectedObject != 0 && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        selectedObject->setOrientation(selectedObject->getOrientationX(),
                                       selectedObject->getOrientationY()-1, selectedObject->getOrientationZ());  //rotate in -y axis
    }

    //camera control
    else if (key == GLUT_KEY_LEFT)
    {
        yAxisRotation--;        //rotate y-axis in negative direction
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        yAxisRotation++;        //rotate y-axis in positive direction
    }
    else if (key == GLUT_KEY_UP && xAxisRotation < 45)
    {
        xAxisRotation++;        //rotate x-axis in positive direction
    }
    else if (key == GLUT_KEY_DOWN && xAxisRotation > -45)
    {
        xAxisRotation--;        //rotate x-axis in negative direction
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
                    saveState(fileNameSave);
                    cout << "File has been saved!\n";
                    break;
                }
            }

        }
        break;
    //set current object to current material
    case 'm':
        selectedObject->storeMaterial(materialCounter);
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
    case '6':
    {
        selectedObject = new Object(Object::Cube);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    case '7':
    {
        selectedObject = new Object(Object::Sphere);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    case '8':
    {
        selectedObject = new Object(Object::Teapot);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    case '9':
    {
        selectedObject = new Object(Object::Cone);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
    }
    break;
    case '0':
    {
        selectedObject = new Object(Object::Torus);
        selectedObject->storeMaterial(materialCounter);
        objectList.push_back(selectedObject);
        break;
    }
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
                    fileLoad(fileNameLoad);
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
    }
    break;


    break;
    }
    glutPostRedisplay();
}

void saveState(string fileName)
{
    fileName = fileName + ".csv"; //creates csv file with user inputted name
    const char *fileNam =  fileName.c_str(); // changes string to char

    ofstream saveState;
    saveState.open (fileNam);

    for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
    {
        Object* objP = *it;
        Object obj = *objP;
        saveState << obj.getType() << "," << obj.getMaterial();
        saveState << "," << obj.getPosX() << "," <<  obj.getPosY() << "," <<  obj.getPosZ();
        saveState << "," << obj.getOrientationX() << "," <<  obj.getOrientationY() << "," <<  obj.getOrientationZ();
        saveState << "," << obj.getScale() << "\n";
    }

    saveState.close();
}

void fileLoad(string fileName)
{
    /* changes string to char and opens file for loading*/
    const char *fileNam =  fileName.c_str();
    ifstream fileLoad;
    fileLoad.open (fileNam);
    string line;

    /*variable declaration*/
    int type, mat;
    float posX, posY, posZ, oriX, oriY, oriZ, scale;
    string strType, strMat, strPosX, strPosY, strPosZ, strOriX, strOriY, strOriZ, strScale;

    while(getline(fileLoad,line))
    {
        stringstream linestr(line);
        if (getline(linestr, strType, ',') &&
                getline(linestr, strMat, ',') &&
                getline(linestr, strPosX, ',') &&
                getline(linestr, strPosY, ',') &&
                getline(linestr, strPosZ, ',') &&
                getline(linestr, strOriX, ',') &&
                getline(linestr, strOriY, ',') &&
                getline(linestr, strOriZ, ',') &&
                getline(linestr, strScale, ','))
        {

            /*changes string type to the desired types (int, and floats for point3D) */
            type = atoi(strType.c_str());
            mat = atoi(strMat.c_str());
            posX = atof(strPosX.c_str());
            posY = atof(strPosY.c_str());
            posZ = atof(strPosZ.c_str());
            oriX = atof(strOriX.c_str());
            oriY = atof(strOriX.c_str());
            oriZ = atof(strOriZ.c_str());
            scale = atof(strScale.c_str());


            switch(type)
            {
            case '0':
            {
                selectedObject = new Object(Object::Cube);
                selectedObject->storeMaterial(mat);
                selectedObject->setPosition(posX, posY, posZ);
                selectedObject->setOrientation(oriX, oriY, oriZ);
                selectedObject->setScale(scale);
                objectList.push_back(selectedObject);
            }
            break;
            case '1':
            {
                selectedObject = new Object(Object::Sphere);
                selectedObject->storeMaterial(mat);
                selectedObject->setPosition(posX, posY, posZ);
                selectedObject->setOrientation(oriX, oriY, oriZ);
                selectedObject->setScale(scale);
                objectList.push_back(selectedObject);
            }
            break;
            case '2':
            {
                selectedObject = new Object(Object::Teapot);
                selectedObject->storeMaterial(mat);
                selectedObject->setPosition(posX, posY, posZ);
                selectedObject->setOrientation(oriX, oriY, oriZ);
                selectedObject->setScale(scale);
                objectList.push_back(selectedObject);
            }
            break;
            case '3':
            {
                selectedObject = new Object(Object::Cone);
                selectedObject->storeMaterial(mat);
                selectedObject->setPosition(posX, posY, posZ);
                selectedObject->setOrientation(oriX, oriY, oriZ);
                selectedObject->setScale(scale);
                objectList.push_back(selectedObject);
            }
            break;
            case '4':
            {
                selectedObject = new Object(Object::Torus);
                selectedObject->storeMaterial(mat);
                selectedObject->setPosition(posX, posY, posZ);
                selectedObject->setOrientation(oriX, oriY, oriZ);
                selectedObject->setScale(scale);
                objectList.push_back(selectedObject);
            }
            break;
            }


        }
    }


    fileLoad.close();

}

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
    setMaterial(0);

    //enable backface culling
    glFrontFace(GL_CW);
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
    glutSolidCube(100); //draws plane
    glPopMatrix();

    //draws all objects
    for(list<Object*>::iterator it=objectList.begin(); it != objectList.end(); ++it)
    {
        Object* objP = *it;
        Object obj = *objP;
        setMaterial(obj.getMaterial());
        obj.drawObject();
    }
    if (selectedObject != 0)
    {
        selectedObject->drawWireframe();  //draw box around selected object
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
    cout << "*ARROW KEYS = control camera movement" << endl;
    cout << "*CTRL + ARROW KEYS/PAGE UP/PAGE DOWN = control translation of currently selected object" << endl;
    cout << "*SHIFT + UP ARROW/DOWN ARROW = manipulate scale of currently selected object" << endl;
    cout << "*ALT + ARROW KEYS/PAGE UP/PAGE DOWN = control rotation of currently selected object" << endl;
    cout << "q or ESC = exits the program" << endl;
    cout << "ALT + wasd = control movement of light source 1" << endl;
    cout << "SHIFT + wasd = control movement of light source 2" << endl;
    cout << "s = saves the current object scene as a .txt file" << endl;
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
