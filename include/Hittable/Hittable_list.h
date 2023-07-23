#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <iostream>
#include <memory>
#include <vector>

#include "hittable.h"

class Hittable_list : public Hittable {
public:
    Hittable_list() = default;
    Hittable_list(const std::string& obj_path, std::shared_ptr<Material> mat_ptr);
    Hittable_list(std::shared_ptr<Hittable> object) { add(object); }
    Hittable_list(std::vector<std::shared_ptr<Hittable>> objects) : _objects(objects) {}

    void clear() { _objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { _objects.push_back(object); }
    std::vector<std::shared_ptr<Hittable>>& get_objects() { return _objects; }

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, AABB& bounding_box) const override;

private:
    std::vector<std::shared_ptr<Hittable>> _objects;
};

#endif  // HITTABLE_LIST_H
