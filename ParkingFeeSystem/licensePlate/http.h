#ifndef HTTP_H
#define HTTP_H
#include <QString>
#include <QtNetwork/QNetworkAccessManager>  // 发送请求的动作
#include <QtNetwork/QNetworkReply>          // 回复的内容
#include <QtNetwork/QNetworkRequest>        // 请求的内容
#include <QEventLoop>                       // 程序悬停
#include <QObject>



class Http : public QObject
{
    Q_OBJECT
public:
    Http();

    static bool post_sync(QString Url,QMap<QString,QString> header,QByteArray &requestDate,QByteArray &replyDate);
};

#endif // HTTP_H
