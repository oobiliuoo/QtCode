#ifndef PFSSERVER_H
#define PFSSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlError>        // 数据库错误
#include <QSqlQuery>        // 数据库查询类


namespace Ui {
class PFSServer;
}

class PFSServer : public QWidget
{
    Q_OBJECT

public:
    explicit PFSServer(QWidget *parent = 0);
    ~PFSServer();

private slots:

private:
    void countCar(int &car_have);
private:
    Ui::PFSServer *ui;
    QTcpServer *server;     //监听套接字
    QTcpSocket *conn;       //通信套接字
    int car_have;
};

#endif // PFSSERVER_H
