#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>

#include "AABB.h"
#include "Ray.h"

class Material;

struct Hit_record {
    Vector3d p;
    Vector3d normal;
    double u, v;
    std::shared_ptr<Material> mat_ptr;

    double t;
    bool front_face;

    // Ensure *normal* always point against the ray
    // and use *front_face* to determine which side of the surface the ray is on
    void set_face_normal(const Ray& r, Vector3d outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const = 0;
    virtual bool bounding_box(AABB& bounding_box) const = 0;
    virtual bool get_mat(std::shared_ptr<Material>& mat_ptr) const { return false; }
    virtual bool sample(Hit_record& rec, double& pdf) const { return false; }
};

#endif  // HITTABLE_H
