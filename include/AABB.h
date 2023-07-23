#ifndef AABB_H
#define AABB_H

#include "Ray.h"

struct AABB {
    static AABB surround_bounding_box(const AABB& bounding_box0, const AABB& bounding_box1) {
        Vector3d surround_min_point{std::min(bounding_box0._min_point.x(), bounding_box1._min_point.x()),
                                    std::min(bounding_box0._min_point.y(), bounding_box1._min_point.y()),
                                    std::min(bounding_box0._min_point.z(), bounding_box1._min_point.z())};

        Vector3d surround_max_point{std::max(bounding_box0._max_point.x(), bounding_box1._max_point.x()),
                                    std::max(bounding_box0._max_point.y(), bounding_box1._max_point.y()),
                                    std::max(bounding_box0._max_point.z(), bounding_box1._max_point.z())};

        return AABB{surround_min_point, surround_max_point};
    }

    AABB() {}
    AABB(Vector3d min_point, Vector3d max_point) : _min_point(min_point), _max_point(max_point) {}
    Vector3d center() const { return (_min_point + _max_point) / 2; }

    bool hit(const Ray& ray, double t_min, double t_max) const {
        for (size_t i = 0; i < 3; ++i) {
            /*
             * Lights that are close to an AABB are also considered to intersect the AABB. Shouldn't there be an elapse
             * here?
             */
            if (ray.direction()[i] == 0) {
                if (_min_point[i] <= ray.origin()[i] && ray.origin()[i] <= _max_point[i]) {
                    break;
                } else {
                    return false;
                }
            }

            double inv_dir = 1 / ray.direction()[i];
            double t0 = (_min_point[i] - ray.origin()[i]) * inv_dir;
            double t1 = (_max_point[i] - ray.origin()[i]) * inv_dir;

            /*
             * when *inv_dir* is negative, it means *ray* will first get *t_max* and next get *t_min*, thus *t0* will be
             * maximum so we should change *t0* with *t1*.
             */
            if (inv_dir < 0.0) {
                std::swap(t0, t1);
            }

            t_min = (t_min < t0) ? t0 : t_min;
            t_max = (t_max > t1) ? t1 : t_max;
        }

        /*
         * The first equal sign must be taken because there may be no thickness for the triangle.
         * The second equal sign means that contact is counted as long as ray touches.
         */
        if (t_min <= t_max && t_max >= 0) {
            return true;
        }

        return false;
    }

    Vector3d _min_point;
    Vector3d _max_point;
};

#endif  // AABB_H
