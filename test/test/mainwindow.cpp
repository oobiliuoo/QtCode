#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->label->text()=="helloQt"){
        ui->label->setText("helloUbnuntu");
    }else{
        ui->label->setText("helloQt");
    }

    float a = 1800 * 0.003;


    qDebug() << a;
}
