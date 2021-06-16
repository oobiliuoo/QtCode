#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    // ui init
    ui->severIp->setText("127.0.0.1");
    ui->serverPort->setText("8080");

    // 实例化
    client = new QTcpSocket(this);

    // 连接服务器
    client->connectToHost(QHostAddress(ui->severIp->text()), ui->serverPort->text().toInt());

    // 接收数据
    connect(client,&QTcpSocket::readyRead,this,[=](){
        QByteArray array = client->readAll();
        ui->record->append(array);
    });

    // 发送数据
    connect(ui->send,&QPushButton::clicked,this,[=](){
        client->write(ui->message->toPlainText().toUtf8().data());
        ui->record->append("Me Say:" + ui->message->toPlainText());
        ui->message->clear();
    });
}

Client::~Client()
{
    delete ui;
}
