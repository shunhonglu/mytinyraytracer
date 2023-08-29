#include <QApplication>
#include <QTime>

#include "Render.h"
#include "RenderPara.h"
#include "ThreadPool.h"
#include "mainwindow.h"
#include "utils.h"

// Render Parameters
RenderPara render_para("E:\\projects\\mytinyraytracer\\scenes\\cornell_box_scene\\cornell_box.json");

// Render
Render r;

// Thread pool
ThreadPool pool;
std::atomic<bool> render_completion = false;
std::atomic<unsigned int> remaining_lines = 0;
QTime render_begin_time;  // I don't set a mutex for it!

int main(int argc, char *argv[]) {
    // Render
    r = Render{render_para.canvas, render_para.cam, render_para.world};

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
