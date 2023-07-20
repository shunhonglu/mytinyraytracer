#include "BVH_Node.h"

void BVH_Node::generate_BVH(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
                       double time1) {
    AABB left_bounding_box, right_bounding_box;

    if (end - start <= 3) {
        left = right = std::make_shared<Hittable_list>(std::vector<std::shared_ptr<Hittable>>(objects.begin() + start, objects.begin() + end));
        if(!left->bounding_box(time0, time1, left_bounding_box) || !right->bounding_box(time0, time1, right_bounding_box)) {
            std::cout << "Can't construct BVH\n";
        }
        _bounding_box = AABB::surround_bounding_box(left_bounding_box, right_bounding_box);
        return;
    }

    int axis = choose_axis(std::vector<std::shared_ptr<Hittable>>(objects.begin() + start, objects.begin() + end));

    std::sort(objects.begin() + start, objects.begin() + end, [&](const std::shared_ptr<Hittable>& left, const std::shared_ptr<Hittable>& right) {
        return (left->origin())[axis] < (right->origin())[axis];
    });

    size_t middle = (start + end) / 2;
    left = std::make_shared<BVH_Node>(objects, start, middle, time0, time1);
    right = std::make_shared<BVH_Node>(objects, middle, end, time0, time1);
    if(!left->bounding_box(time0, time1, left_bounding_box) || !right->bounding_box(time0, time1, right_bounding_box)) {
        std::cout << "Error\n";
    }
    _bounding_box = AABB::surround_bounding_box(left_bounding_box, right_bounding_box);
}

BVH_Node::BVH_Node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
                   double time1) {
    generate_BVH(objects, start, end, time0, time1);
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

bool BVH_Node::bounding_box(double time0, double time1, AABB& bounding_box) const {
    bounding_box = _bounding_box;
    return true;
}
