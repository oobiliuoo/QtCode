#include "image.h"

Image::Image()
{

}

QByteArray Image::imageToBase64(QImage myimg)
{
    //QImage img(imgPath);

    QImage img = myimg;
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf,"JPG");                       //把img写入buf
    QByteArray base64 = ba.toBase64();          //对图片进行base64编码（不包括编码头）

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    // 对图片进行urlencode
    QByteArray imgData = codec->fromUnicode(base64).toPercentEncoding();

    return imgData;
}
