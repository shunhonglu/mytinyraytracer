#include "Sphere.h"

bool Sphere::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Vector3d oc = r.origin() - center;
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - radius * radius;

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
    Vector3d outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Sphere::bounding_box(double time0, double time1, AABB& bounding_box) const {
    AABB bounding_box0{center - Vector3d{radius, radius, radius}, center + Vector3d{radius, radius, radius}};
    bounding_box = bounding_box0;
    return true;
}

bool Moving_Sphere::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Vector3d oc = r.origin() - get_center(r.time());
    auto a = r.direction().dot(r.direction());
    auto half_b = oc.dot(r.direction());
    auto c = oc.dot(oc) - radius * radius;

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
    Vector3d outward_normal = (rec.p - get_center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Moving_Sphere::bounding_box(double time0, double time1, AABB& bounding_box) const {
    AABB bounding_box0{center0 - Vector3d{radius, radius, radius}, center0 + Vector3d{radius, radius, radius}};
    AABB bounding_box1{center1 - Vector3d{radius, radius, radius}, center1 + Vector3d{radius, radius, radius}};
    bounding_box = AABB::surround_bounding_box(bounding_box0, bounding_box1);
    return true;
}
