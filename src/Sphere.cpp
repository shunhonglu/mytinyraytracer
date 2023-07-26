#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Vector3d oc = r.origin() - _center;
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - _radius * _radius;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vector3d outward_normal = (rec.p - _center) / _radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = _mat_ptr;

    return true;
}

bool Sphere::bounding_box(AABB& bounding_box) const {
    bounding_box = AABB{_center - Vector3d{_radius, _radius, _radius}, _center + Vector3d{_radius, _radius, _radius}};
    return true;
}
