#include "mainwindow.h"

#include <QApplication>
#include <QTime>
#include "Render.h"
#include "Scene.h"
#include "ThreadPool.h"

// Screen
double aspect_ratio = 1.0;
int width = 800;
int height = static_cast<int>(width / aspect_ratio);
Screen canvas(width, height);

// Camera
Vector3d lookfrom{13, 2, 3};
Vector3d lookat{0, 0, 0};
Vector3d vup{0.0, 1.0, 0.0};
double aperture = 0.0;
double focus_dist = 1.0;
Camera cam(lookfrom, lookat, vup, 40.0, aspect_ratio, aperture, focus_dist, 0.0, 0.0);

// World
Hittable_list world = random_scene();

// Render
Render r{canvas, cam, world};

// Render Parameters
int samples_per_pixel = 1;
int raytraer_depth = 10;
Color3d background{0.0, 0.0, 0.0};

// Thread pool
ThreadPool pool;
std::atomic<bool> render_completion = false;
std::atomic<unsigned int> remaining_lines = 0;
QTime render_begin_time; // I don't set a mutex for it!

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
