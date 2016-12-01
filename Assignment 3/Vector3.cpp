#include "Vector3.h"

Vector3::Vector3() { }

Vector3::Vector3(float x, float y, float z)
{
	d[0] = x; d[1] = y; d[2] = z;
}

Vector3::Vector3(const Vector3 &v)
{
	d[0] = v.d[0]; d[1] = v.d[1]; d[2] = v.d[2];
}

float Vector3::x() { return d[0]; }
float Vector3::y() { return d[1]; }
float Vector3::z() { return d[2]; }
float Vector3::length() { return sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]); }
void Vector3::normalize()
{
	float temp = length();
    if (temp == 0.0)
    	return; // 0 length vector
    // multiply by 1/magnitude
    temp = 1 / temp;
    d[0] *= temp;
    d[1] *= temp;
    d[2] *= temp;
}
