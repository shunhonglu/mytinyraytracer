#ifndef RENDER_H
#define RENDER_H

#include "BVH_Node.h"
#include "Camera.h"
#include "Hittable_list.h"
#include "Screen.h"

class Render {
public:
    Render() = default;
    Render(const Screen& canvas, const Camera& cam, const Hittable_list& world)
        : _canvas(canvas), _cam(cam), _world(world) {
        bvh = BVH_Node{_world, 0.0, infinity};
    }

    const Screen& get_screen() const { return _canvas; }

    Color3d ray_color(const Ray& r, Color3d background, const Hittable& world, int depth);

    void render(int samples_per_pixel, int depth, Color3d background);

private:
    Screen _canvas;
    Camera _cam;
    Hittable_list _world;
    BVH_Node bvh;
};

#endif  // RENDER_H
