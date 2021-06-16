#include "server.h"
#include "ui_server.h"



Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    // ui init
    ui->severIp->setText("127.0.0.1");
    ui->serverPort->setText("8080");

    // 实例化 init
    server = new QTcpServer(this);
    // 监听 第一个为IP地址 第二个为端口号
    server->listen(QHostAddress(ui->severIp->text()),ui->serverPort->text().toInt());
    // 新的连接 收到一个newConnection信号 处理函数采用lamda表达式
    connect(server,&QTcpServer::newConnection,this,[=](){

        // 接收客户端的连接对象 相当与 accept
        // sock_addr 结构体 == 类 QTcpSocket
        conn = server->nextPendingConnection();
        ui->record->append("有新的连接...");

        // 保证conn是一个有效对象，所以写在里面
        connect(conn,&QTcpSocket::readyRead,this,[=](){
            // 接收数据
            QByteArray array = conn->readAll();
            ui->record->append(array);

        });

    });

    // 发送
    connect(ui->send,&QPushButton::clicked,this,[=](){
        // 发送数据 toPlainText 将文本框内容转换为纯字符串
        conn->write(ui->message->toPlainText().toUtf8().data());
        ui->record->append("Me Say:" + ui->message->toPlainText());
        // 清除输入框数据
        ui->message->clear();

    });


}

Server::~Server()
{
    delete ui;
}
