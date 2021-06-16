#include "pfsserver.h"
#include "ui_pfsserver.h"
#include <QMessageBox>
#include <QDateTime>
#include <QEventLoop>
#include <QTime>
#include <QElapsedTimer>

PFSServer::PFSServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PFSServer)
{
    ui->setupUi(this);

    // ---------------- mysql  init --------------
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    // 配置数据库
    db.setHostName("127.0.0.1");            // 主机IP
    db.setUserName("root");                 // mysql用户
    db.setPassword("root");                 // 用户密码
    db.setDatabaseName("ParkingFeeSystem"); // 数据库名

    // 打开数据库
    if(db.open()==false)
    {
        QMessageBox::warning(this,"waring", db.lastError().text());
    }


    // -------- tcp server init-----------
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
        ui->record_2->append("有新的连接...");

        countCar(car_have);
        conn->write(QString("欢迎光临博雅小区，剩余停车位：%1").arg(car_have).toUtf8().data());

        // 保证conn是一个有效对象，所以写在里面
        connect(conn,&QTcpSocket::readyRead,this,[=](){
            // 接收数据
            QByteArray array = conn->readAll();

            QString mes_data = QString(array);
            QString car_date = mes_data.section(',',0,0);
            QString state = mes_data.section(',',1,1);
            QString time = mes_data.section(',',2,2);

            QSqlQuery query;
            if(state == '0')
            {
                // 车辆进入
                  QString sql = QString("insert into car_in_tb(car_number,in_time,out_flog) values('%1','%2',%3)").arg(car_date).arg(time).arg(state);
                  query.exec(sql);

                  QString message = car_date + " 欢迎光临！";
                  conn->write(message.toUtf8().data());

            }
            if(state == '1')
            {
                // 车辆离开
                QString in_time_str ;
                QString sql = QString("select * from car_in_tb where car_number='%1'").arg(car_date);
                query.exec(sql);
                while(query.next())
                {
                    in_time_str = query.value("in_time").toString();
                }
                uint in_time_int = QDateTime::fromString(in_time_str ,"yyyy-MM-dd hh:mm:ss").toTime_t();
                uint out_time_int = QDateTime::fromString(time ,"yyyy-MM-dd hh:mm:ss").toTime_t();
                float stay_time = (out_time_int - in_time_int)/1800;
                float money = 0.0;
                // 计算费用
                if(stay_time > 1)
                {
                    // 大于1h,每小时5快
                    money = stay_time * 5;
                }

                // 更新车库表
                sql = QString("update car_in_tb set out_flog=1 where car_number='%1'").arg(car_date);
                query.exec(sql);

                // 更新收费表
                sql = QString("insert into car_money_tb(car_number,in_time,out_time,car_stay_time,money) "
                              "values('%1','%2','%3',%4,%5)").arg(car_date).arg(in_time_str).arg(time).arg(stay_time).arg(money);
                query.exec(sql);

                QString stay_time_str = QString("，共停留：%1 小时").arg(stay_time);
                QString money_str = QString("，收费：%1 元，一路顺风！").arg(money);
                QString message = car_date + stay_time_str + money_str;
                conn->write(message.toUtf8().data());


            }

            ui->record_2->append(array);
            countCar(car_have);

            conn->write(QString("剩余停车位：%1").arg(car_have).toUtf8().data());

        });

    });


    // 发送
//    connect(ui->send,&QPushButton::clicked,this,[=](){
//        // 发送数据 toPlainText 将文本框内容转换为纯字符串
//        conn->write(ui->message->toPlainText().toUtf8().data());
//        ui->record->append("Me Say:" + ui->message->toPlainText());
//        // 清除输入框数据
//        ui->message->clear();

//    });




}

PFSServer::~PFSServer()
{
    delete ui;
}


void PFSServer::countCar(int &car_have){

    int car_sum = 100; // 车库总量
    int car_count = 0; // 车库当前车辆
    QSqlQuery query;
    QString sql = "select count(*) from car_in_tb where out_flog=0";
    query.exec(sql);
    while(query.next())
    {
        car_count = query.value("count(*)").toInt();
    }
    car_have = car_sum - car_count;

}
