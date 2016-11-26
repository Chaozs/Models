/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "SaveLoadStates.h"


void SaveLoadStates::saveState(string fileName, list<Object*> objectList)
{
    fileName = fileName + ".csv"; //creates csv file with user inputted name
    const char *fileChar =  fileName.c_str(); // changes string to char

    ofstream saveState;
    saveState.open (fileChar);

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

list<Object*> SaveLoadStates::loadState(string fileName)
{
    /* changes string to char and opens file for loading*/
    const char *fileChar =  fileName.c_str();
    ifstream loadState;
    loadState.open (fileChar);
    string line;
    list<Object*> objectList;

    /*variable declaration*/
    int type, mat;
    float posX, posY, posZ, oriX, oriY, oriZ, scale;
    string strType, strMat, strPosX, strPosY, strPosZ, strOriX, strOriY, strOriZ, strScale;

    while(getline(loadState,line))
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

            Object* tempObj;
            tempObj = new Object();
            switch(type){
                case 0:
                    tempObj->setType(Object::Cube);
                    break;
                case 1:
                    tempObj->setType(Object::Sphere);
                    break;
                case 2:
                    tempObj->setType(Object::Teapot);
                    break;
                case 3:
                    tempObj->setType(Object::Cone);
                    break;
                case 4:
                    tempObj->setType(Object::Torus);
                    break;
            }
            
            tempObj->storeMaterial(mat);
            tempObj->setPosition(posX, posY, posZ);
            tempObj->setOrientation(oriX, oriY, oriZ);
            tempObj->setScale(scale);
            objectList.push_back(tempObj);

        }
    }
    loadState.close();
}