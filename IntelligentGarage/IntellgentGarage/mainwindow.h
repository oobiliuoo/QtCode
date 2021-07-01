#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "licenceplaterecognition.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QFileDialog>


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

    //Tcp server--------
    void initNet();
    void mConnect();
    void readMsg();
    void sendMsg(QString msg);

    //摄像头
    void readVideo();

    QStringList getFileNames(const QString &path);


private slots:
    void on_btn_door_clicked();

private:
    Ui::MainWindow *ui;
    // 原图
    Mat originImg;
    QTcpServer *server;     //监听套接字
    QTcpSocket *conn;       //通信套接字
};

#endif // MAINWINDOW_H

