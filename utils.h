#ifndef UTILS_H
#define UTILS_H

#include <QTime>

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "ThreadPool.h"
#include "mymath.h"

class Render;

// Render
extern Render r;

// Render Parameters
extern int samples_per_pixel;
extern int raytraer_depth;
extern Color3d background;

// Thread pool
extern ThreadPool pool;

// TODO: it needs improvement
// judge if *render* is over
extern std::atomic<bool> render_completion;
extern std::atomic<unsigned int> remaining_lines;
extern QTime render_begin_time;

#endif  // UTILS_H
