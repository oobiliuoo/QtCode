#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("调用系统摄像头");
    camera=new QCamera;//摄像头
    viewfinder=new QCameraViewfinder(this);  //取景器
    imageCapture=new QCameraImageCapture(camera);//截图部件
    camera->setViewfinder(viewfinder);   //相机设置取景器

    ui->viewImage->addWidget(viewfinder);    //界面部件添加取景器

    QObject::connect(imageCapture, SIGNAL(imageCaptured(int,QImage)),
                     this, SLOT(displayImage(int,QImage)));    //图片显示
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);  //截图目的   转化成文件
    camera->setCaptureMode(QCamera::CaptureStillImage);   //摄像头截屏模式
    camera->setViewfinder(viewfinder);  //摄像头取景器
    camera->start(); //启动摄像头
}

MainWindow::~MainWindow()
{
    delete ui;
}
//显示图片
void MainWindow::displayImage(int ,QImage image)
{
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->setScaledContents(true);
    ui->statusBar->showMessage(tr("capture OK!"), 5000);
}

//拍照截屏
//void MainWindow::on_pushButton_capture_clicked()
//{
//    imageCapture->capture();    //截图后会自动触发  displayImage（int ,QImage image）信号
//    ui->statusBar->showMessage(tr("capturing..."), 1000);
//}

//保存
//void MainWindow::on_pushButton_save_clicked()
//{
    //自动保存
//    const QPixmap *pixmap = ui->label->pixmap();
//    if(pixmap)
//    {
//         pixmap->save("E:\\hallo.jpg");
//         qDebug()<<"保存上了"<<endl;
//    }

//    //选择路径
//    QString fileName=QFileDialog::getSaveFileName(this, tr("save file"), QDir::homePath(), tr("jpegfile(*.jpg)"));
//    if(fileName.isEmpty()) {
//    ui->statusBar->showMessage(tr("save cancel"), 5000);
//    return;
//    }
//    const QPixmap* pixmap=ui->label->pixmap();
//    if(pixmap) {
//    pixmap->save(fileName);
//    ui->statusBar->showMessage(tr("save OK"), 5000);
//    }
//}

////退出
//void MainWindow::on_pushButton_exit_clicked()
//{
//    QApplication *app;
//    app->exit();
//}

void MainWindow::on_pushButton_clicked()
{
    imageCapture->capture();    //截图后会自动触发  displayImage（int ,QImage image）信号
    ui->statusBar->showMessage(tr("capturing..."), 1000);
}

void MainWindow::on_pushButton_2_clicked()
{
    //自动保存
//    const QPixmap *pixmap = ui->label->pixmap();
//    if(pixmap)
//    {
//         pixmap->save("E:\\hallo.jpg");
//         qDebug()<<"保存上了"<<endl;
//    }

    //选择路径
    QString fileName=QFileDialog::getSaveFileName(this, tr("save file"), QDir::homePath(), tr("jpegfile(*.jpg)"));
    if(fileName.isEmpty()) {
    ui->statusBar->showMessage(tr("save cancel"), 5000);
    return;
    }
    const QPixmap* pixmap=ui->label->pixmap();
    if(pixmap) {
    pixmap->save(fileName);
    ui->statusBar->showMessage(tr("save OK"), 5000);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QApplication *app;
    app->exit();
}
