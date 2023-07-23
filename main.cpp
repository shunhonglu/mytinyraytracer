#include <QApplication>
#include <QTime>

#include "Render.h"
#include "RenderPara.h"
#include "ThreadPool.h"
#include "mainwindow.h"
#include "utils.h"

// Render Parameters
RenderPara render_para("E:\\projects\\json_files\\bunny.json");

// Render
Render r;

// Thread pool
ThreadPool pool;
std::atomic<bool> render_completion = false;
std::atomic<unsigned int> remaining_lines = 0;
QTime render_begin_time;  // I don't set a mutex for it!

int main(int argc, char *argv[]) {
    // Screen
    double aspect_ratio = 1.0;
    int width = 800;
    int height = static_cast<int>(width / aspect_ratio);
    Screen canvas(width, height);

    // Camera
    Camera cam(render_para.lookfrom, render_para.lookat, render_para.vup, render_para.vfov, aspect_ratio,
               render_para.aperture, render_para.focus_dist, 0.0, 0.0);


    Hittable_list world;
    // World
    if(!render_para.scene_function.empty()){
        world = get_scene_function(render_para.scene_function)();
    }

    if(!render_para.scene_obj_paths.empty()) {
        world = obj_scene(render_para.scene_obj_paths);
    }

    // Render
    r = Render{canvas, cam, world};

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
