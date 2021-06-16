#ifndef IMAGE_H
#define IMAGE_H
#include <QString>
#include <QImage>
#include <QBuffer>
#include <QTextCodec>
class Image
{
public:
    Image();
    static QByteArray imageToBase64(QImage myimg);

};

#endif // IMAGE_H
