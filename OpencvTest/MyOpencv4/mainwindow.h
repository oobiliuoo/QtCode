#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/opencv.hpp"
#include "QLabel"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void colorSpace_Demo();
    void mat_creation_demo();
    void pixel_visit_demo();
    void channels_demo();
    void inrange_demo();
    void pixel_statistic_demo();
    void drawing_demo();
    void random_drawing_demo();
    void norm_demo();
    void resize_demo();
    void flip_demo();
    void rotate_demo();
    void video_demo();
    void histogram_demo();
    void histogram_eq_demo();
    void blur_demo();
    void gaussian_blur_demo();
    void bifilter_demo();
    void dilate_erode_demo();

    void test();
    void test2();
    void test3();

    int OTSU(cv::Mat &srcImage);
    cv::Mat readImage(const cv::String& file_path);
    QImage MatToQImage(const cv::Mat& mat);
    void showImage(QLabel* label, QImage img);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
