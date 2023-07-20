#include "Hittable_list.h"

bool Hittable_list::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool Hittable_list::bounding_box(double time0, double time1, AABB& bounding_box) const {
    if (objects.empty()) {
        return false;
    }

    AABB temp;
    bool first_hit = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp)) {
            return false;
        }
        bounding_box = first_hit ? temp : AABB::surround_bounding_box(bounding_box, temp);
        first_hit = false;
    }

    return true;
}
