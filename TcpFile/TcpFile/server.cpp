#include "server.h"
#include "ui_server.h"
#include <QFile>
#include <QMessageBox>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);



    // 套接字实例化
    server = new QTcpServer();

    // 设置ip地址和端口号
    QString ip_addr = "192.168.43.131";
    int port = 8080;

    // 监听
    server->listen(QHostAddress(ip_addr),port);

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
            QString recv_data = QString(array);

            QString flog = recv_data.section("##",0,0);

            if(flog=="0"){
                // 判断是请求的文件是否存在
                QString filepath = recv_data.section("##",1,1);
                bool isFile = isFileExist(filepath);
                if(isFile){
                    file_path = filepath;
                    ui->record->append("正在请求：" + file_path);
                    // 读取指定文件
                    // 创建文件对象
                    // 默认读取文件格式 utf-8
                    QFile file(file_path);
                    // 指定打开方式
                    bool isok = file.open(QFile::ReadOnly);
                    if(isok == false)
                    {
                        QMessageBox::critical(this,"error","open file failed");
                        return;
                    }
                    file_size = file.size();
                    // 读文件
                    QByteArray fileArray = file.readAll();
                    // 显示到文本框
                    ui->record->append(fileArray);
                    QByteArray send_data = "0##";
                    send_data.append(fileArray);
                    //发送数据 toPlainText 将文本框内容转换为纯字符串
                    conn->write(send_data);
                    ui->record->append("发送成功！");

                    // 关闭文件
                    file.close();
                }
                else {
                    QByteArray array = "1##文件不存在";
                    conn->write(array);
                     ui->record->append("请求资源不存在！ 资源路径:" + filepath);
                }

            }
            else if(flog=="1"){
                QFile file(file_path);
                bool isok = file.open(QFile::WriteOnly);
                if(isok == false)
                {
                    QMessageBox::critical(this,"error","open file failed");
                    return;
                }
                QByteArray file_data = recv_data.section("##",1,1).toUtf8();
                //写文件
                file.write(file_data);

                ui->record->append("文件:"+file_path+"已被修改");

            }


        });

    });

    //    // 发送
    //    connect(ui->send,&QPushButton::clicked,this,[=](){

    //        // 发送数据 toPlainText 将文本框内容转换为纯字符串
    //        conn->write(ui->message->toPlainText().toUtf8().data());
    //        ui->record->append("Me Say:" + ui->message->toPlainText());
    //        // 清除输入框数据
    //        ui->message->clear();

    //    });



}

bool Server::isFileExist(QString fullFilePath)
{
    QFile file(fullFilePath);
    if(file.exists())
    {
        return true;
    }
    return false;
}

Server::~Server()
{
    delete ui;
}
