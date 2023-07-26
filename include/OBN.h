#ifndef OBN_H
#define OBN_H

#include "mymath.h"

// TODO: there seem to be some problems remaining.
class OBN {
public:
    OBN() {}

    Vector3d operator[](int i) const { return axis[i]; }
    Vector3d u() const { return axis[0]; }
    Vector3d v() const { return axis[1]; }
    Vector3d w() const { return axis[2]; }

    Vector3d local(double a, double b, double c) const { return a * u() + b * v() + c * w(); }

    Vector3d local(Vector3d a) const { return a.x() * u() + a.y() * v() + a.z() * w(); }

    // n doesn't have to be a unit vector.
    void build_from_w(Vector3d n) {
        axis[2] = n.normalized();
        Vector3d a = (std::fabs(w().x()) > 0.9) ? Vector3d{0.0, 1.0, 0.0} : Vector3d{1.0, 0.0, 0.0};
        axis[1] = (w().cross(a)).normalized();
        axis[0] = w().cross(v());
    }

public:
    Vector3d axis[3];
};

#endif  // OBN_H
