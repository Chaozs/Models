
#ifndef _RAY_H_
#define _RAY_H_

#include "Vector3.h"

class Ray {
  public:
    Ray();
    Ray(Vector3 o, Vector3 d);
    Ray(const Ray &r);

    Vector3 origin;
    Vector3 direction;
    Vector3 inv_direction;
    int sign[3];
};

#endif
