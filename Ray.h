#ifndef RAY_H
#define RAY_H

#include "mymath.h"

class Ray {
public:
    Ray() {}
    Ray(const Vector3d& origin, const Vector3d& direction, double time) : orig(origin), dir(direction), t(time) {}

    Vector3d origin() const { return orig; }
    Vector3d direction() const { return dir; }
    double time() const { return t; }

    Vector3d at(double t) const { return orig + t * dir; }

private:
    Vector3d orig;
    Vector3d dir;
    double t;
};

#endif  // RAY_H
