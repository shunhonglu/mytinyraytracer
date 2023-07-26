#include "BVH_Node.h"

#include "spdlog/spdlog.h"

/*
 * we only have Triangle and Spere now, and their bounding_box function always return true.
 */
void BVH_Node::generate_BVH(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    AABB left_bounding_box, right_bounding_box;

    if (end - start <= 3) {
        left = right = std::make_shared<Hittable_list>(
            std::vector<std::shared_ptr<Hittable>>(objects.begin() + start, objects.begin() + end));
        if (!left->bounding_box(left_bounding_box) || !right->bounding_box(right_bounding_box)) {
            spdlog::error("Can't construct bounding box");
            exit(1);
        }
        _bounding_box = AABB::surround_bounding_box(left_bounding_box, right_bounding_box);
        return;
    }

    Axis axis = choose_axis(std::vector<std::shared_ptr<Hittable>>(objects.begin() + start, objects.begin() + end));

    std::sort(objects.begin() + start, objects.begin() + end,
              [&](const std::shared_ptr<Hittable>& left, const std::shared_ptr<Hittable>& right) {
                  AABB left_aabb, right_aabb;
                  left->bounding_box(left_aabb);
                  right->bounding_box(right_aabb);
                  return left_aabb.center()[axis] < right_aabb.center()[axis];
              });

    size_t middle = (start + end) / 2;
    left = std::make_shared<BVH_Node>(objects, start, middle);
    right = std::make_shared<BVH_Node>(objects, middle, end);
    if (!left->bounding_box(left_bounding_box) || !right->bounding_box(right_bounding_box)) {
        spdlog::error("Can't construct bounding box");
        exit(1);
    }
    _bounding_box = AABB::surround_bounding_box(left_bounding_box, right_bounding_box);
}

BVH_Node::BVH_Node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    generate_BVH(objects, start, end);
}

bool BVH_Node::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    if (!_bounding_box.hit(r, t_min, t_max)) {
        return false;
    }

    // choose the closest *Hittable*
    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left | hit_right;
}

/*
 * this function is of no use, we have calculated our bounding_box in the constructor.
 */
bool BVH_Node::bounding_box(AABB& bounding_box) const {
    bounding_box = _bounding_box;
    return true;
}
