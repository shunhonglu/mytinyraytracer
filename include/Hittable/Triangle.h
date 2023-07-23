#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "Hittable.h"

class Triangle : public Hittable {
public:
    Triangle() = default;
    Triangle(Vector3d v0, Vector3d v1, Vector3d v2, Vector3d n0, Vector3d n1, Vector3d n2, Vector2d uv0, Vector2d uv1,
             Vector2d uv2, std::shared_ptr<Material> mat_ptr)
        : _v0(v0), _v1(v1), _v2(v2), _n0(n0), _n1(n1), _n2(n2), _uv0(uv0), _uv1(uv1), _mat_ptr(mat_ptr) {}
    Triangle(std::vector<Vector3d> v, std::vector<Vector3d> n, std::vector<Vector2d> uv,
             std::shared_ptr<Material> mat_ptr)
        : Triangle(v[0], v[1], v[2], n[0], n[1], n[2], uv[0], uv[1], uv[2], mat_ptr) {}

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;

private:
    Vector3d _v0, _v1, _v2;
    Vector3d _n0, _n1, _n2;
    Vector2d _uv0, _uv1, _uv2;
    std::shared_ptr<Material> _mat_ptr;
};

#endif  // TRIANGLES_H
