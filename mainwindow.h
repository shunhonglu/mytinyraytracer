#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent* event) override;

private:
    Ui::MainWindow* ui;
    QImage* canvas;
    QTimer* timer;
    QMenuBar* menu;
    QStatusBar* status;
    QLabel* remaining_lines_label;
    QLabel* render_times_label;
    QMenu* begin_menu;
    QAction* save_as_image_action;

    QTime render_cur_time;
    QTime elapse_time;  // initialized in *create_status_bar()*

    void create_canvas();
    void create_menu();
    void create_status_bar();
    void create_connect();
    void create_timer();

private slots:
    void on_save_as_image_clicked();
};

#endif  // MAINWINDOW_H
