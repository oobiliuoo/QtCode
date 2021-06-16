#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>


namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    bool isFileExist(QString fullFilePath);

private:
    Ui::Server *ui;
    QTcpServer *server;     //监听套接字
    QTcpSocket *conn;       //通信套接字
    QString file_path;      //请求的文件名
    int file_size;          //请求文件的大小

};

#endif // SERVER_H
