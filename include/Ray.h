#ifndef RAY_H
#define RAY_H

#include "mymath.h"

class Ray {
public:
    Ray() {}
    Ray(const Vector3d& origin, const Vector3d& direction, double time)
        : _origin(origin), _direction(direction), _time(time) {}

    Vector3d origin() const { return _origin; }
    Vector3d direction() const { return _direction; }
    double time() const { return _time; }

    Vector3d at(double time) const { return _origin + time * _direction; }

private:
    Vector3d _origin;
    Vector3d _direction;
    double _time;
};

#endif  // RAY_H
