#ifndef UTILS_H
#define UTILS_H

#include <QTime>
#include <atomic>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>

#include "Hittable_list.h"
#include "Render.h"
#include "ThreadPool.h"

class RenderPara;

// Render Parameters
extern RenderPara render_para;

// Render
extern Render r;

// Scenes
extern std::function<Hittable_list()> get_scene_function(std::string function_name);
extern std::function<Hittable_list(const std::vector<std::string>&)> get_obj_scene_function(std::string function_name);

// Thread pool
extern ThreadPool pool;

// TODO: it needs improvement
// judge if *render* is over
extern std::atomic<bool> render_completion;
extern std::atomic<unsigned int> remaining_lines;
extern QTime render_begin_time;

// Format output
static std::string Vec3d_to_str(Vector3d v) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << v[0] << ", " << v[1] << ", " << v[2];
    return ss.str();
}

static std::string Vec2d_to_str(Vector2d v) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << v[0] << ", " << v[1];
    return ss.str();
}

#endif  // UTILS_H
