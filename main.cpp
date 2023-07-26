#include <QApplication>
#include <QTime>

#include "Render.h"
#include "RenderPara.h"
#include "ThreadPool.h"
#include "mainwindow.h"
#include "utils.h"

// Render Parameters
RenderPara render_para("E:\\projects\\json_files\\cornell_box.json");

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
    int width = 1200;
    int height = static_cast<int>(width / aspect_ratio);
    Screen canvas(width, height);

    // Camera
    Camera cam(render_para.lookfrom, render_para.lookat, render_para.vup, render_para.vfov, aspect_ratio,
               render_para.aperture, render_para.focus_dist, 0.0, 0.0);

    // Render
    r = Render{canvas, cam, render_para.world};

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
