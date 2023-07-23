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
    Sphere(Vector3d cen, double r, std::shared_ptr<Material> m_p) : center(cen), radius(r), mat_ptr(m_p) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;

private:
    Vector3d center;
    double radius;
    std::shared_ptr<Material> mat_ptr;
};

class Moving_Sphere : public Hittable {
public:
    Moving_Sphere() {}
    Moving_Sphere(Vector3d cen0, double time0, Vector3d cen1, double time1, double r, std::shared_ptr<Material> m_p)
        : center0(cen0), t0(time0), center1(cen1), t1(time1), radius(r), mat_ptr(m_p) {}

    Vector3d get_center(double t) const { return center0 + (t - t0) / (t1 - t0) * (center1 - center0); }

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;

private:
    Vector3d center0, center1;
    double t0, t1;
    double radius;
    std::shared_ptr<Material> mat_ptr;
};

#endif  // SPHERE_H
