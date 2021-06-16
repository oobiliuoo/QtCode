#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "http.h"

#include <QDebug>
#include <QApplication>
#include <QFileDialog>
#include <QDateTime>


const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
const char *client_id = "G32Cl4xwcuhXzO5LDOmZXq6p";
const char *secret_id = "GGLxyLysdsTciqvvychCewp78O1w6b8u";
const QString baiduLicensePlateUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=%1";
const QString baiduTextToSpeekUrl = "http://tsn.baidu.com/text2audio?";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::openImageSlot);

    //-------摄像模块init-----
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

    // --------音频模块 init----------
    media_player = new QMediaPlayer;

    //--------Tcp client 模块 init-----

    QString serverIp = "127.0.0.1";
    int serverPort = 8080;
    // 实例化
    client = new QTcpSocket(this);
    // 连接服务器
    client->connectToHost(QHostAddress(serverIp), serverPort);
    // 发送数据
    // 进入
    connect(ui->btnEnter,&QPushButton::clicked,this,[=](){
        QString message = ui->lineEdit->text() + " 进入中...";
        ui->statusBar->showMessage(message, 5000);
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        message = ui->lineEdit->text() + ",0," + timeStr;
        client->write(message.toUtf8().data());
    });
    // 退出
    connect(ui->btnOut,&QPushButton::clicked,this,[=](){
        QString message = ui->lineEdit->text() + " 退出中...";
        ui->statusBar->showMessage(message, 5000);
        QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        message = ui->lineEdit->text() + ",1," + timeStr;
        client->write(message.toUtf8().data());
    });

    // 接收数据
    connect(client,&QTcpSocket::readyRead,this,[=](){
        QByteArray array = client->readAll();
        ui->showMessage->setText(array);
        // 音频播报
        QByteArray url=baiduTextToSpeekUrl.toUtf8().data();
        url.append(QString("&lan=zh&cuid=00:0c:29:6d:a7:94&ctp=1&tok=24.1c43e892cca323128fb3965633c33758.2592000.1611738451.282335-23404255&pit=8&per=3"));
        url.append("&tex=");
        url.append(QUrl::toPercentEncoding(ui->showMessage->text().toUtf8().data()));
        media_player->setMedia(QUrl(url));
        media_player->play();

        ui->btnEnter->setEnabled(false);
        ui->btnOut->setEnabled(false);
    });

    QImage image("/home/biliu/Desktop/QtCode/ParkingFeeSystem/pic/bg1.png");
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->setScaledContents(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImageSlot(){
    // 第二个参数是左上角的提示文本,第三个是默认路径，后面表示当前路径
    imgPath = QFileDialog::getOpenFileName(this,"选择图片",QCoreApplication::applicationFilePath());
    if(imgPath.isEmpty()){
        QMessageBox::warning(this,"提示","请插入一张图片");

    }
    QPixmap pix(imgPath);
    ui->label->setPixmap(pix);
}

//显示图片
void MainWindow::displayImage(int ,QImage image)
{
    myImage = image;
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->setScaledContents(true);
    ui->statusBar->showMessage(tr("capture OK!"), 5000);
}

// 开始识别
void MainWindow::on_pushButton_clicked()
{
    ui->statusBar->showMessage(tr("图片正在识别中..."));

    QByteArray img = Image::imageToBase64(myImage);
    QByteArray imgDate = "image="+img;      //body

    // 获取access_token
    QByteArray replyDate;  //保存回复信息

    QString url = QString(baiduTokenUrl).arg(client_id).arg(secret_id);
    //QString url = QString(baiduTokenUrl);
    QMap<QString,QString> header;       //封装头父信息
    header.insert(QString("Content-Type"),QString("application/x-www-form-urlencoded"));
    // accessToken有效期一个月,到期后可打开下面代码获取
    QString accessToken = "24.a6400d436350c59025954b5e8851c5f1.2592000.1611124736.282335-23188497";
    bool result;


    //    bool result = Http::post_sync(url,header,imgDate,replyDate);

    //    if(result){
    //       QJsonObject obj = QJsonDocument::fromJson(replyDate).object();
    //        accessToken = obj.value("access_token").toString();
    //     }

    QString licensePlateUrl = baiduLicensePlateUrl.arg(accessToken);
    replyDate.clear();
    result = Http::post_sync(licensePlateUrl,header,imgDate,replyDate);

    if(result)
    {
        QJsonObject obj = QJsonDocument::fromJson(replyDate).object();
        QJsonValue val = obj.value("words_result");

        if(val.isObject())
        {
            QJsonObject obj2 = val.toObject();
            QJsonValue words_result = obj2.value("number");

            QString car_license_plate = words_result.toString();
            ui->lineEdit->setText(car_license_plate);
            ui->statusBar->showMessage(tr("识别成功！"), 2000);
            ui->btnEnter->setEnabled(true);
            ui->btnOut->setEnabled(true);
            return ;

        }

    }
    ui->lineEdit->setText("识别失败");
}

// 拍照
void MainWindow::on_btn_photo_clicked()
{
    imageCapture->capture();    //截图后会自动触发  displayImage（int ,QImage image）信号
    ui->statusBar->showMessage(tr("capturing..."), 1000);
}

// 保存照片
void MainWindow::on_btn_save_clicked()
{
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


