#include "Triangle.h"

#include "AABB.h"
#include "spdlog/spdlog.h"

bool Triangle::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Vector3d E1 = _v1 - _v0;
    Vector3d E2 = _v2 - _v0;
    Vector3d S = r.origin() - _v0;
    Vector3d S1 = r.direction().cross(E2);
    Vector3d S2 = S.cross(E1);
    double S1_dot_E = S1.dot(E1);
    double t = S2.dot(E2) / S1_dot_E;
    double b1 = S1.dot(S) / S1_dot_E;
    double b2 = S2.dot(r.direction()) / S1_dot_E;

    if(t < t_min || t > t_max) {
        return false;
    }

    if(b1 < 0 || b2 < 0 || (1 - b1 - b2) < 0) {
        return false;
    }
    rec.t = t;
    rec.p = r.at(rec.t);
    rec.normal = ((1 - b1 - b2) * _n0 + b1 * _n1 + b2 * _n2).normalized();
    Vector2d uv = (1 - b1 - b2) * _uv0 + b1 * _uv1 + b2 * _uv2;
    rec.u = uv[0];
    rec.v = uv[1];
    rec.mat_ptr = _mat_ptr;
    return true;
}

bool Triangle::bounding_box(double time0, double time1, AABB& bounding_box) const {
    if (near_zero(_v0 - _v1) || near_zero(_v1 - _v2) || near_zero(_v2 - _v0)) {
        spdlog::warn("there is a degenerated triangles!");
    }

    Vector3d min_point, max_point;
    for (size_t i = 0; i < 3; ++i) {
        min_point[i] = std::min(_v0[i], std::min(_v1[i], _v2[i]));
        max_point[i] = std::max(_v0[i], std::max(_v1[i], _v2[i]));
    }

    bounding_box = AABB{min_point, max_point};
    return true;
}
