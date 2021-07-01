#ifndef LICENCEPLATERECOGNITION_H
#define LICENCEPLATERECOGNITION_H

#include "opencv2/opencv.hpp"
using namespace cv;

class LicencePlateRecognition
{
public:
    LicencePlateRecognition();
    LicencePlateRecognition(Mat img);

    // 图片预处理
    void pictureProcessing();
    void pictureProcessing(Mat &img);

    // 边缘检测
    void borderDetection(Mat &img );
    void borderDetection();

    // 形态学处理
    void morphologicalProcessing(Mat &img);
    void morphologicalProcessing();

    // 车牌提取
    Mat licensePlateExtraction(Mat &img);
    Mat licensePlateExtraction();

    // 车牌判断
    bool checkLicenseFromShape(float mLWR, float rectRate, double area);

    // 区域提取
    Mat regionalExtract(Mat &img, RotatedRect rRect);

    // 颜色判断
    bool checkLicenseFromColor(Mat img);


    void regionalExtract2(Mat &img);

    void characterExtraction(Mat img);
    void characterExtraction2(Mat img);

    int OTSU(cv::Mat &srcImage);
    cv::Mat rotate_demo(Mat &image,cv::RotatedRect rRect);


private:
    void characterPicProcessing( Mat &img);
    void characterDivision( Mat& img);
    void characterChineseDiscern(Mat &img);
    void characterOtherDiscern(Mat &img);
public:
    Mat srcImg ;
    Mat orginImg ;
};

#endif // LICENCEPLATERECOGNITION_H
