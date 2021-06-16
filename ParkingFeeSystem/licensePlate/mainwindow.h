#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <image.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QImage>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMediaPlayer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openImageSlot();
    void on_pushButton_clicked();
    void on_btn_photo_clicked();
    void on_btn_save_clicked();
    void displayImage(int,QImage image);  //播放图片


private:
    Ui::MainWindow *ui;
    QString imgPath;
    QCamera *camera;                            //摄像头
    QCameraViewfinder * viewfinder;             //摄像头取景器部件
    QCameraImageCapture * imageCapture;			//截图部件
    QImage myImage;                             // 拍下来的照片
    QTcpSocket *client;                         // 通信套接字
    QMediaPlayer *media_player;                 // 音频播放对象
};

#endif // MAINWINDOW_H
