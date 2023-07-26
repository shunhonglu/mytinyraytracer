#ifndef SPHERE_H
#define SPHERE_H

#include <memory>

#include "Hittable.h"
#include "Material.h"
#include "mymath.h"

class Sphere : public Hittable {
public:
    static void get_sphere_uv(Vector3d point, double& u, double& v) {
        /*
         * Attention: *point* is a unit vector
         */
        double phi = std::atan2(-point.z(), point.x()) + pi;
        double theta = std::acos(-point.y());

        u = phi / (2 * pi);
        v = theta / pi;
    }

    Sphere() {}
    Sphere(Vector3d center, double radius, std::shared_ptr<Material> mat_ptr)
        : _center(center), _radius(radius), _mat_ptr(mat_ptr) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(AABB& bounding_box) const override;
    virtual bool get_mat(std::shared_ptr<Material>& mat_ptr) const override {
        mat_ptr = _mat_ptr;
        return true;
    }

private:
    Vector3d _center;
    double _radius;
    std::shared_ptr<Material> _mat_ptr;
};

#endif  // SPHERE_H
