#ifndef PDF_H
#define PDF_H

#include "OBN.h"
#include "mymath.h"

class PDF {
public:
    virtual double value(Vector3d direction) const = 0;
    virtual Vector3d generate() const = 0;
};

class Cosine_PDF : public PDF {
public:
    static Vector3d random_cosine_direction() {
        double r1 = random_double();
        double r2 = random_double();

        double phi = 2 * pi * r1;
        double x = cos(phi) * sqrt(r2);
        double y = sin(phi) * sqrt(r2);
        double z = sqrt(1 - r2);

        return Vector3d{x, y, z};
    }
    Cosine_PDF(Vector3d normal) { uvw.build_from_w(normal); }

    virtual double value(Vector3d direction) const override {
        Vector3d unit_direction = direction.normalized();
        double cos_theta = unit_direction.dot(uvw.w());

        if (cos_theta > 0.0) {
            return cos_theta / pi;
        } else {
            return 0.0;
        }
    }

    virtual Vector3d generate() const override { return uvw.local(random_cosine_direction()); }

private:
    OBN uvw;
};

#endif  // PDF_H
