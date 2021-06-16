#include <QMainWindow>
#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDebug>
#include <QImage>

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void displayImage(int,QImage image);  //播放图片

private slots:
//    void on_pushButton_capture_clicked();  //拍照
//    void on_pushButton_save_clicked();   //保存
//    void on_pushButton_exit_clicked();    //退出

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QCamera *camera;				//摄像头
    QCameraViewfinder * viewfinder;			//摄像头取景器部件
    QCameraImageCapture * imageCapture;			//截图部件
};
