#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
// 文件流
// 文本流  数据流（二进制格式）
#include <QTextStream>      // 操作基础数据类型： int,string...
#include <QDataStream>      // QImage、QPoint、QRect
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

#if 0
    connect(ui->btnOpenFile,&QPushButton::clicked,this,[=](){
        QString filename = QFileDialog::getOpenFileName(this,"open file","/home/biliu");
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,"warning","select file failed");
            return;
        }
        ui->filepath->setText(filename);

        // 创建文件对象
        // 默认读取文件格式 utf-8
        QFile file(filename);
        // 指定打开方式
        bool isok = file.open(QFile::ReadOnly);
        if(isok == false)
        {
            QMessageBox::critical(this,"error","open file failed");
            return;
        }
        // 读文件
        QByteArray array = file.readAll();
        // 显示到文本框
        ui->textEdit->setText(array);

        // 写文件
        // file.write(QString("aaa").toUtf8());
        // char buf[128];
        // file.wirte(buf,strlen(128));
        // file.write(buf);
        // 关闭文件
        file.close();

    });

#endif
    connect(ui->btnOpenFile,&QPushButton::clicked,this,[=](){
        QString filename = QFileDialog::getOpenFileName(this,"open file","/home/biliu");
        if(filename.isEmpty())
        {
            QMessageBox::warning(this,"warning","select file failed");
            return;
        }
        ui->filepath->setText(filename);

        // 创建文件对象
        // 默认读取文件格式 utf-8
        QFile file(filename);
        // 指定打开方式
        bool isok = file.open(QFile::ReadOnly);
        if(isok == false)
        {
            QMessageBox::critical(this,"error","open file failed");
            return;
        }
        // 创建流对象
        QTextStream stream(&file);       // 设置IO设备给流对象
        // stream.setCodec("utf8");
        // 读文件
        QString array = stream.readAll();
        // 显示到文本框
        ui->textEdit->setText(array);

        // 写文件
        // file.write(QString("aaa").toUtf8());
        // char buf[128];
        // file.wirte(buf,strlen(128));
        // file.write(buf);
        // 关闭文件
        file.close();
#if 0
        QFile f("text.txt");
        f.open(QFile::WriteOnly);
        QTextStream qts(&f);
        // 写文件
        qts<<QString("你好啊") << 123456;
        f.close();

        QString text;
        f.open(QFile::ReadOnly);
        qts.setDevice(&f);
        qts>>text;
        qDebug() << text.toUtf8().data();
#endif
        QFile f("text.txt");
        f.open(QFile::WriteOnly);
        QDataStream ds(&f);
        // 写文件
        ds<<QString("你好啊") << 123456;
        f.close();
        // 注意：读取顺序要一致
        QString text;
        int number;
        f.open(QFile::ReadOnly);
        ds.setDevice(&f);
        ds>>text>>number;
        qDebug() << text.toUtf8().data()<<number;
#if 0
        // 特殊用法
        QImage img("xxx.jpg");
        QByteArray aaa;
        QDataStream sss(&aaa,QIODevice::ReadWrite);
        sss<<img;
#endif

    });



}

Widget::~Widget()
{
    delete ui;
}
