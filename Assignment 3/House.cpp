/*
3GC3 Assignment 3

Susan Yuen / yuens2 / 001416198
Thien Trandinh / trandit / 001420634
*/

#include "House.h"

//constructor
House::House()
{

}

void House::buildNormal(float x, float y, float z, int index){

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

void House::drawHouse(){
	glBegin(GL_TRIANGLES);
		glVertex3f(1,0,0);
		glVertex3f(1,0,1);
		glVertex3f(0,0,0);

		glVertex3f(1,0,1);
		glVertex3f(1,0,0);
		glVertex3f(0,0,0);

		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(0,0,0);

		glVertex3f(1,1,0);
		glVertex3f(0,1,0);
		glVertex3f(0,0,0);
	glEnd();
	
}