#include "http.h"

Http::Http()
{

}

bool Http::post_sync(QString Url, QMap<QString, QString> header, QByteArray &requestDate, QByteArray &replyDate)
{

    QNetworkAccessManager manager;      // 发送请求的动作
    QNetworkRequest request;            // 请求的内容(包含url和头)
    request.setUrl(Url);

    QMapIterator<QString,QString> it(header);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toLatin1(),it.value().toLatin1());
    }

    QNetworkReply *reply =  manager.post(request,requestDate);
    QEventLoop l;
    // 请求完成产生一个信号,l收到信号调用退出函数，结束死循环
    connect(reply,&QNetworkReply::finished,&l,&QEventLoop::quit);
    l.exec();   //死循环

    if(reply!=nullptr && reply->error()== QNetworkReply::NoError)
    {
        replyDate = reply->readAll();      //读出回复的信息
        return true;
    }
    else
    {
        return false;
    }

}
