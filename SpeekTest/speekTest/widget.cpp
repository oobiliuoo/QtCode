#include "widget.h"
#include "ui_widget.h"
#include <QMediaPlayer>
#include "http.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QFile>
#include <QDataStream>

const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
const char *client_id = "OrLGGXeI6d1lbUIGECNPIpLL";
const char *secret_id = "UyLgK8fGkkDW8la14Ih7Gn2LoeZlaQ2w";
const QString baiduTextToSpeek = "http://tsn.baidu.com/text2audio";


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

  //QMediaPlayer *player = new QMediaPlayer;
  //播放进度的信号提示
  // connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
  //player->setMedia(QUrl::fromLocalFile("/home/biliu/Music/无问.mp3"));
  //player->setVolume(50); //0~100音量范围,默认是100
  //player->play();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{

    QFile mymp3;
    QMediaPlayer *media_player = new QMediaPlayer;
    QByteArray url="http://tsn.baidu.com/text2audio?";
    url.append(QString("&lan=zh&cuid=00:0c:29:6d:a7:94&ctp=1&tok=24.1c43e892cca323128fb3965633c33758.2592000.1611738451.282335-23404255&pit=8&per=3"));
    url.append("&tex=");
    url.append(QUrl::toPercentEncoding(ui->textEdit->toPlainText()));
    qDebug()<<url;
    //QByteArray url2 =  "http://tsn.baidu.com/text2audio?lan=zh&ctp=1&cuid=00:0c:29:6d:a7:94&tok=24.1c43e892cca323128fb3965633c33758.2592000.1611738451.282335-23404255&vol=9&per=0&spd=5&pit=5&aue=3&per=4&tex=%E6%88%91%E4%B8%8D%E5%A5%BD";
     media_player->setMedia(QUrl(url));
     media_player->play();


}
