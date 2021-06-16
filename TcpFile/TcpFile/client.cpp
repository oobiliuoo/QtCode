#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    // ui init
    QString ip_addr = "192.168.43.131";
    int port = 8080;

    // 实例化
    client = new QTcpSocket(this);

    // 连接服务器
    client->connectToHost(QHostAddress(ip_addr), port);

    // 接收数据
    connect(client,&QTcpSocket::readyRead,this,[=](){
        QByteArray array = client->readAll();
        QString flog = QString(array).section("##",0,0);
        QString recv_data = QString(array).section("##",1,1);
        if(flog == "0"){
            ui->content->setText(recv_data);
            ui->btnSend->setEnabled(true);
        }
        else if (flog == "1") {
             ui->record->append(recv_data);
        }

    });
    // /home/biliu/Desktop/QtCode/TcpFile/testFile/users.txt
    // 发送请求数据
    connect(ui->btnRequest,&QPushButton::clicked,this,[=](){
        QByteArray array = "0##";
        array.append(ui->fileName->text().toUtf8().data());
        client->write(array);
        ui->record->append("我正在请求:" + ui->fileName->text());
    });
    // 发生修改后数据
    connect(ui->btnSend,&QPushButton::clicked,this,[=](){
        QByteArray array = "1##";
        array.append(ui->content->toPlainText().toUtf8().data());
        client->write(array);
        ui->record->append("文件已修改，发送成功");
        ui->btnSend->setEnabled(false);
    });
}

Client::~Client()
{
    delete ui;
}
