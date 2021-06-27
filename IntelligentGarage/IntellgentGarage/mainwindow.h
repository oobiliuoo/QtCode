#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "licenceplaterecognition.h"
typedef LicencePlateRecognition LPR;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void test(cv::Mat);
    void test2();

    // 摄像头
    void readVideo();


private:
    Ui::MainWindow *ui;
    // 原图
    Mat originImg;
};

#endif // MAINWINDOW_H
