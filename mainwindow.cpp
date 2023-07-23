#include "mainwindow.h"

#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPainter>
#include <QStatusBar>
#include <QTimer>
#include <iostream>
#include <thread>
#include <vector>

#include "./ui_mainwindow.h"
#include "Render.h"
#include "utils.h"
#include "RenderPara.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    std::thread t1([&] { r.render(render_para.samples_per_pixel, render_para.raytraer_depth, render_para.background); });

    ui->setupUi(this);

    create_canvas();

    create_menu();

    create_status_bar();

    create_connect();

    create_timer();

    this->setWindowTitle("mytinyraytracer");

    this->setFixedSize(r.get_screen().get_width(), r.get_screen().get_height() + status->height());

    pool.push_back(std::move(t1));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::paintEvent(QPaintEvent *event) {
    if (render_completion) {
        timer->stop();
    }

    // TODO: it needs improvement
    // construct a new QImage and swap it with canvas
    QImage image = QImage(r.get_screen().get_data().data(), r.get_screen().get_width(), r.get_screen().get_height(),
                          QImage::Format_RGB888);
    canvas->swap(image);
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.drawImage(0, 0, *canvas);

    remaining_lines_label->setText("Remaining Lines: " + QString::number(remaining_lines) + " ");

    render_cur_time = QTime::currentTime();
    elapse_time.setHMS(0, 0, 0);
    elapse_time = elapse_time.addSecs(render_begin_time.secsTo(render_cur_time));
    render_times_label->setText(" Total Times: " + elapse_time.toString("hh:mm:ss") + " ");
}

void MainWindow::create_canvas() {
    canvas = new QImage(r.get_screen().get_data().data(), r.get_screen().get_width(), r.get_screen().get_height(),
                        QImage::Format_RGB888);
}

void MainWindow::create_menu() {
    menu = new QMenuBar(this);
    // Attention: set this *QMenuBar* as *Mainwindow Menubar*
    this->setMenuBar(menu);
    begin_menu = menu->addMenu("开始");
    save_as_image_action = begin_menu->addAction("存储为图像");
}

void MainWindow::create_status_bar() {
    status = statusBar();
    setStatusBar(status);
    remaining_lines_label = new QLabel("Remaining Lines: " + QString::number(remaining_lines) + " ");
    status->addWidget(remaining_lines_label);

    elapse_time.setHMS(0, 0, 0);
    render_times_label = new QLabel(" Total Times: " + elapse_time.toString("hh:mm:ss") + " ");
    status->addWidget(render_times_label);
}

void MainWindow::create_connect() {
    connect(save_as_image_action, &QAction::triggered, this, &MainWindow::on_save_as_image_clicked);
}

void MainWindow::create_timer() {
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));

    // every 500 milisecond refresh screen
    timer->start(100);
}

void MainWindow::on_save_as_image_clicked() {
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("png");
    dialog.setNameFilter("*.png");
    dialog.exec();

    // get the newly created file name
    QString selectedFile = dialog.selectedFiles().value(0);
    canvas->save(selectedFile);
}
