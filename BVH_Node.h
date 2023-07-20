#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "Hittable_list.h"

class BVH_Node : public Hittable {
public:
    // TODO: choose the longest axis, this function should be simplified
    static int choose_axis(const std::vector<std::shared_ptr<Hittable>>& objects) {
        double x_min = std::numeric_limits<double>::min(), x_max = std::numeric_limits<double>::max();
        double y_min = std::numeric_limits<double>::min(), y_max = std::numeric_limits<double>::max();
        double z_min = std::numeric_limits<double>::min(), z_max = std::numeric_limits<double>::max();
        for (const auto& object : objects) {
            x_min = (x_min > object->origin().x()) ? object->origin().x() : x_min;
            x_max = (x_max < object->origin().x()) ? object->origin().x() : x_max;

            y_min = (y_min > object->origin().y()) ? object->origin().y() : y_min;
            y_max = (y_max < object->origin().y()) ? object->origin().y() : y_max;

            z_min = (z_min > object->origin().z()) ? object->origin().z() : z_min;
            z_max = (z_max < object->origin().z()) ? object->origin().z() : z_max;
        }

        double x_length = x_max - x_min;
        double y_length = y_max - y_min;
        double z_length = z_max - z_min;

        if (x_length >= y_length && x_length >= z_length) {
            return 0;
        } else if (y_length >= x_length && y_length >= x_length) {
            return 1;
        } else {
            return 2;
        }
    }

    BVH_Node() = default;

    BVH_Node(Hittable_list& world, double time0, double time1)
        : BVH_Node{world.objects, 0, world.objects.size(), time0, time1} {}

    BVH_Node(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
             double time1);
    void generate_BVH(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
                      double time1);

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;
    // TODO: this function is of no use
    virtual Vector3d origin() const override {
        return {0.0, 0.0, 0.0};
    }

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB _bounding_box;
};

#endif  // BVH_NODE_H
