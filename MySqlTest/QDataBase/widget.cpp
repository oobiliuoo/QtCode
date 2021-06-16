#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>        // 数据库错误
#include <QSqlQuery>        // 数据库查询类


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // qDebug()<< QSqlDatabase::drivers();
    // 添加一个MYSQL数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    // 配置数据库
    db.setHostName("127.0.0.1");            // 主机IP
    db.setUserName("root");                 // mysql用户
    db.setPassword("root");                 // 用户密码
    db.setDatabaseName("mypython"); // 数据库名

    // 打开数据库
    if(db.open()==false)
    {
        QMessageBox::warning(this,"waring", db.lastError().text());
    }

    // 增删改查
    // 添加记录
    QSqlQuery query;
//    QString sql = "insert into classes(name,num) values('QT',59)";
//    query.exec(sql);

    // 批处理
    // ? -- 通配符 ，odbc风格 顺序不能变
//    query.prepare("insert into classes(name,num) values(?,?)");
//    // 添加绑定数据
//    QVariantList namelist;
//    namelist <<"c#"<<"java"<<"shell";
//    query.addBindValue(namelist);
//    QVariantList numlist;
//    numlist <<88<<99<<100;
//    query.addBindValue(numlist);
//    // 执行批处理
//    query.execBatch();

    // oracle 风格通配符 顺序可变
//    query.prepare("insert into classes(name,num) values(:name,:num)");
//    // 添加绑定数据
//    QVariantList namelist;
//    namelist <<"c#"<<"java"<<"shell";
//    query.BindValue(":name",namelist);
//    QVariantList numlist;
//    numlist <<88<<99<<100;
//    query.BindValue(":num",numlist);
//    // 执行批处理
//    query.execBatch();


//    query.exec("select * from classes");
//    while(query.next())
//    {
//        qDebug() << query.value(1).toString().toUtf8().data();     // 表示表的第一个字段
//        qDebug() << query.value("num").toInt();

//    }

   //1.实例化model
   model = new QSqlTableModel(this);
   //2.将模型设置到视图中
   ui->tableView->setModel(model);
   //3.给model设置数据库表  -- 前提条件： 数据库已经打开
   model->setTable("classes");
   //4. 查询表
   model->select();
   //5. 设置表头
   model->setHeaderData(0,Qt::Horizontal,"编号");
   //6. 设置提交模式
   model->setEditStrategy(QSqlTableModel::OnManualSubmit);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_submitAll_clicked()
{
    model->submitAll();
}

void Widget::on_revertAll_clicked()
{
    model->revertAll();  // 撤销操作
    model->submitAll();  // 更新数据库表
}

void Widget::on_find_clicked()
{
    QString name = ui->name->text();

    // select * from classes where name=''
    // 设置过滤条件
    QString sql = QString("name='%1'").arg(name);
    model->setFilter(sql);
    // 重新查询
    model->select();

}
