#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <iostream>
#include <memory>
#include <vector>

#include "hittable.h"

class Hittable_list : public Hittable {
public:
    Hittable_list() {}
    Hittable_list(std::shared_ptr<Hittable> object) { add(object); }
    Hittable_list(std::vector<std::shared_ptr<Hittable>> objs) : objects(objs) {}

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;
    virtual Vector3d origin() const override {
        if (objects.empty()) {
            std::cout << "Empty Hittable_list, can't calculate origin!\n";
        }

        Vector3d temp;
        for (const auto& object : objects) {
            temp += object->origin();
        }
        return temp / objects.size();
    }

public:
    std::vector<std::shared_ptr<Hittable>> objects;
};

#endif  // HITTABLE_LIST_H
