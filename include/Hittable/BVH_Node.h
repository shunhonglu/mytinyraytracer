#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "Hittable_list.h"

class BVH_Node : public Hittable {
    enum Axis{
        X_Axis, Y_Axis, Z_Axis
    };
public:
    static Axis choose_axis(const std::vector<std::shared_ptr<Hittable>>& objects) {
        Hittable_list temp{objects};
        AABB aabb;
        temp.bounding_box(0, infinity, aabb);

        Vector3d spacing = aabb._max_point - aabb._min_point;
        if(spacing.x() >= spacing.y() && spacing.x() > spacing.z()) {
            return X_Axis;
        } else if(spacing.y() >= spacing.x() && spacing.y() > spacing.z()) {
            return Y_Axis;
        } else {
            return Z_Axis;
        }
    }

    BVH_Node() = default;

    BVH_Node(Hittable_list& world, double time0, double time1)
        : BVH_Node{world.get_objects(), 0, world.get_objects().size(), time0, time1} {}

    BVH_Node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1);
    void generate_BVH(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
                      double time1);

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;
    // TODO: this function is of no use

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB _bounding_box;
};

#endif  // BVH_NODE_H
