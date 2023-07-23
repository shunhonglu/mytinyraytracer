#ifndef XY_RECT_H
#define XY_RECT_H

#include "Hittable.h"

class XY_Rect : public Hittable {
public:
    XY_Rect() = default;
    XY_Rect(double x0, double x1, double y0, double y1, double z, std::shared_ptr<Material> XY_Rect_mat)
        : _x0(x0), _x1(x1), _y0(y0), _y1(y1), _z(z), _XY_Rect_mat(XY_Rect_mat) {}
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
        auto t = (_z - r.origin().z()) / r.direction().z();

        if (t < t_min || t > t_max) {
            return false;
        }

        auto x = r.origin().x() + t * r.direction().x();
        auto y = r.origin().y() + t * r.direction().y();

        if (x < _x0 || x > _x1 || y < _y0 || y > _y1) {
            return false;
        }

        rec.t = t;
        rec.p = r.at(t);
        Vector3d outward_normal = Vector3d{0.0, 0.0, 1.0};
        rec.set_face_normal(r, outward_normal);
        rec.u = (x - _x0) / (_x1 - _x0);
        rec.v = (y - _y0) / (_y1 - _y0);
        rec.mat_ptr = _XY_Rect_mat;

        return true;
    }
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const {
        bounding_box = AABB{Vector3d{_x0, _y0, _z - 0.001}, Vector3d{_x1, _y1, _z + 0.001}};
        return true;
    }

private:
    double _x0{0.0}, _x1{0.0}, _y0{0.0}, _y1{0.0}, _z{0.0};
    std::shared_ptr<Material> _XY_Rect_mat;
};

class XZ_Rect : public Hittable {
public:
    XZ_Rect() = default;
    XZ_Rect(double x0, double x1, double z0, double z1, double y, std::shared_ptr<Material> XZ_Rect_mat)
        : _x0(x0), _x1(x1), _z0(z0), _z1(z1), _y(y), _XZ_Rect_mat(XZ_Rect_mat) {}
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
        auto t = (_y - r.origin().y()) / r.direction().y();

        if (t < t_min || t > t_max) {
            return false;
        }

        auto x = r.origin().x() + t * r.direction().x();
        auto z = r.origin().z() + t * r.direction().z();

        if (x < _x0 || x > _x1 || z < _z0 || z > _z1) {
            return false;
        }

        rec.t = t;
        rec.p = r.at(t);
        Vector3d outward_normal = Vector3d{0.0, 1.0, 0.0};
        rec.set_face_normal(r, outward_normal);
        rec.u = (x - _x0) / (_x1 - _x0);
        rec.v = (z - _z0) / (_z1 - _z0);
        rec.mat_ptr = _XZ_Rect_mat;

        return true;
    }
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const {
        bounding_box = AABB{Vector3d{_x0, _y - 0.001, _z0}, Vector3d{_x1, _y + 0.001, _z1}};
        return true;
    }

private:
    double _x0{0.0}, _x1{0.0}, _z0{0.0}, _z1{0.0}, _y{0.0};
    std::shared_ptr<Material> _XZ_Rect_mat;
};

class YZ_Rect : public Hittable {
public:
    YZ_Rect() = default;
    YZ_Rect(double y0, double y1, double z0, double z1, double x, std::shared_ptr<Material> YZ_Rect_mat)
        : _y0(y0), _y1(y1), _z0(z0), _z1(z1), _x(x), _YZ_Rect_mat(YZ_Rect_mat) {}
    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
        auto t = (_x - r.origin().x()) / r.direction().x();

        if (t < t_min || t > t_max) {
            return false;
        }

        auto y = r.origin().y() + t * r.direction().y();
        auto z = r.origin().z() + t * r.direction().z();

        if (y < _y0 || y > _y1 || z < _z0 || z > _z1) {
            return false;
        }

        rec.t = t;
        rec.p = r.at(t);
        Vector3d outward_normal = Vector3d{1.0, 0.0, 0.0};
        rec.set_face_normal(r, outward_normal);
        rec.u = (y - _y0) / (_y1 - _y0);
        rec.v = (z - _z0) / (_z1 - _z0);
        rec.mat_ptr = _YZ_Rect_mat;

        return true;
    }
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const {
        bounding_box = AABB{Vector3d{_x - 0.001, _y0, _z0}, Vector3d{_x + 0.001, _y1, _z1}};
        return true;
    }

private:
    double _y0{0.0}, _y1{0.0}, _z0{0.0}, _z1{0.0}, _x{0.0};
    std::shared_ptr<Material> _YZ_Rect_mat;
};

#endif  // XY_RECT_H
