#include "licenceplaterecognition.h"
#define SOBEL_TYPE 1
#define DEBUG_COUT 1
#define SHOW_ALL 0
#define SHOW_ONE 0

LicencePlateRecognition::LicencePlateRecognition()
{
}

LicencePlateRecognition::LicencePlateRecognition(Mat img){
    this->srcImg = img;
    this->orginImg = img.clone();
}

/**
* ---------------------------------------------------------------------------------------------------------
* 图片预处理
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
void LicencePlateRecognition::pictureProcessing(Mat &img){
    // 1 彩色图转灰度图
    cvtColor(img,img,COLOR_BGR2GRAY);
    // 2 高斯滤波，中值滤波
    // 2.1 高斯滤波
    GaussianBlur(img,img,Size(3,3),0,0,BORDER_DEFAULT);
    //imshow("Gaussian", img);
    // 2.2 中值滤波
    medianBlur(img,img,5);
    //imshow("median", img);
    this->srcImg = img;

#if SHOW_ALL == 1 || SHOW_ONE == 1
    imshow("pictureProcessing",img);
#endif
}

void LicencePlateRecognition::pictureProcessing(){
    if(this->srcImg.empty()){
        std::cout<<"请先赋值"<<std::endl;
        return ;
    }
    Mat img = this->srcImg;
    pictureProcessing(img);
}

/**
* ---------------------------------------------------------------------------------------------------------
* 边缘检测
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
void LicencePlateRecognition::borderDetection(Mat &img ){

#if SOBEL_TYPE == 1
    Mat sobelImg;
    // 边缘化检测
    Sobel(img,sobelImg,CV_16S,1,0,3);
    // imshow("img5", img);
    convertScaleAbs(sobelImg,img);
#elif SOBEL_TYPE == 2
    Mat sobelImg,sobelX,sobelY;
    Sobel(img,sobelImg,CV_16S,1,0,3);
    convertScaleAbs(sobelImg,sobelX);

    Sobel(img,sobelImg,CV_16S,0,1,3);
    convertScaleAbs(sobelImg,sobelY);

    addWeighted(sobelX,0.7,sobelY,0.3,0,img);
#endif
    this->srcImg = img;
    // SHOW_ONE 2
#if SHOW_ALL == 1 || SHOW_ONE == 2
    imshow("sobel",img);
#endif
}

void LicencePlateRecognition::borderDetection(){
    if(this->srcImg.empty()){
        std::cout<<"请先赋值"<<std::endl;
        return ;
    }
    Mat img = this->srcImg;
    borderDetection(img);
}


/**
* ---------------------------------------------------------------------------------------------------------
* 形态学处理
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
void LicencePlateRecognition::morphologicalProcessing(Mat &img){
    // 二值化操作
    threshold(img,img,125,255,THRESH_BINARY);
    //imshow("img7", img);
    // 形态学处理
    // 闭运算
    Mat element  = getStructuringElement(MORPH_RECT,Size(17,5));
    morphologyEx(img,img,MORPH_CLOSE,element);
#if SHOW_ALL == 1 || SHOW_ONE == 1
    imshow("mp", img);
#endif

}

void LicencePlateRecognition::morphologicalProcessing(){
    if(this->srcImg.empty()){
        std::cout<<"请先赋值"<<std::endl;
        return ;
    }
    Mat img = this->srcImg;
    morphologicalProcessing(img);
}

/**
* ---------------------------------------------------------------------------------------------------------
* 根据形状特征判断是否为车牌
* 汽车车牌区域的每个字符宽度为45 mm，字符高度为90 mm，间隔符宽10 mm，字符间隔为12 mm
* 整个车牌区域的宽高比为44/14
* mLWR:长宽比
* recrRate: 矩形率，越接近1越像矩形
* area: 面积
* ---------------------------------------------------------------------------------------------------------
*/
bool LicencePlateRecognition::checkLicenseFromShape(float mLWR, float rectRate, double area){

    const float LENGTH_WIDTH_RATIO = 3.142857; // 标准长宽比
    const float ERROR_RATE = 1;       // 浮动范围
    float errEara = 0.5;
    float minLWR = LENGTH_WIDTH_RATIO - ERROR_RATE;  // 最小长宽比
    float maxLWR = LENGTH_WIDTH_RATIO + ERROR_RATE;  // 最大长宽比


    if(mLWR>minLWR && mLWR < maxLWR && rectRate > 1 - errEara && rectRate < 1 + errEara
            && area > 1500 && area<8000){
        return true;
    }

    return false;
}

/**
* ---------------------------------------------------------------------------------------------------------
* 区域获取
* 获取指定图片的指定区域
* img: 指定图像
* rRect: 要获取的区域，为旋转矩形
* ---------------------------------------------------------------------------------------------------------
*/
Mat LicencePlateRecognition::regionalExtract(Mat &img,  RotatedRect rRect){
    Mat licenceImg;
    // 掩码
    Mat mask = Mat::zeros(img.size(),CV_8UC3);
    // 标记轮廓区域
    Point2f mPoints[4];
    rRect.points(mPoints);
    Point points[4];
    for(int i=0;i<4;i++){
        points[i].x = (int) mPoints[i].x;
        points[i].y = (int) mPoints[i].y;
    }
    fillConvexPoly(mask,points,4,Scalar(255,255,255),LINE_AA);
    //imshow("mask",mask);
    Mat t ;
    img.copyTo(t,mask);
    //imshow("t",t);
    float ff = 90;
    std::cout<<"angle"<<rRect.angle<<std::endl;
    // 判断是否需要旋转
    if(abs(abs(rRect.angle) - ff) < 5 ){
        // 始终保持宽 > 高
        Size rect_size = rRect.size;
        if (rect_size.width < rect_size.height) {
            swap(rect_size.width, rect_size.height);
        }
        std::cout<<"sub"<<std::endl;
        getRectSubPix(t,rect_size,rRect.center,licenceImg);
    }else{
        std::cout<<"rotate"<<std::endl;
        licenceImg =  rotate_demo(t,rRect);
    }
    //imshow("licenceImg",licenceImg);


    return licenceImg;
}

/**
* ---------------------------------------------------------------------------------------------------------
* 车牌颜色判断
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
bool LicencePlateRecognition::checkLicenseFromColor(Mat img){

    const int BLUE_WHITE_B = 138;
    const int BLUE_WHITE_G = 63;
    const int BLUE_WHITE_R = 23;
    const int ERROR_RANGE = 85;

  //  Mat dst;
    //dst = imread("../pic/carnum1.jpg");
   // Mat m[3];
   // split(dst,m);

    //cvtColor(img,img,COLOR_BGR2GRAY);
    //equalizeHist(img,img);
    //cvtColor(dst,dst,COLOR_GRAY2BGR);

   // merge(m,dst);
    //imshow("qweq",img);

    Mat mean,stddev; // 平均值，方差
    meanStdDev(img,mean,stddev);

    double pixelB = mean.at<double>(0,0);
    double pixelG = mean.at<double>(1,0);
    double pixelR = mean.at<double>(2,0);

    int mB = abs(pixelB - BLUE_WHITE_B);
    int mG = abs(pixelG - BLUE_WHITE_G);
    int mR = abs(pixelR - BLUE_WHITE_R);

#if DEBUG_COUT == 1
    std::cout<<"mean: "<<std::endl;
    std::cout<<mean<<std::endl;
    std::cout<<"mb: "<<mB<<std::endl;
    std::cout<<"mg: "<<mG<<std::endl;
    std::cout<<"mr: "<<mR<<std::endl;
    imshow("color check",img);
#endif

    if(mB < ERROR_RANGE && mG < ERROR_RANGE && mR < ERROR_RANGE){
          return true;
    }
    return false;
}

/**
* ---------------------------------------------------------------------------------------------------------
* 车牌提取
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
Mat LicencePlateRecognition::licensePlateExtraction(Mat &img){

    Mat temp2 = this->orginImg.clone();
    Mat licenceImg = this->orginImg.clone();
    double area = 0;    //轮廓面积
    int height = 0;     //外接矩高
    int weight = 0;     //外接矩长
    float mLWR = 0;     //当前长宽比
    int mArea = 0;      //外接距面积
    float rectRate = 1; //矩形率

    // 获取所有轮廓
    std::vector<std::vector<Point>> contours;
    findContours(img,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

    int i = 0;  // 记录当前轮廓下标
    // 遍历所有轮廓
    std::vector<std::vector<Point>>::iterator it;
    for(it=contours.begin();it!=contours.end();it++)
    {
        // 计算轮廓面积
        area = contourArea(*it);
        // 筛选出可能轮廓
        if(area>800&&area<10000){
            // 获取最小外接矩
            RotatedRect mRect = minAreaRect(*it);
            // 获取四个角点坐标
            Point2f mPoints[4];// 左下，左上，右上，右下
            mRect.points(mPoints);
            Point points[4];
            for(int i=0;i<4;i++){
                points[i].x = (int) mPoints[i].x;
                points[i].y = (int) mPoints[i].y;
            }
            // 获取长宽
            Size2f s = mRect.size;
            height = s.height;
            weight = s.width;

            // 获取长宽比
            if(height > weight)
                mLWR = (float) height / weight;
            else
                mLWR = (float) weight / height;
            // 矩形面积
            mArea = height * weight;
            rectRate = area/mArea;

            if(checkLicenseFromShape(mLWR,rectRate,area)){
                std::cout<<"**** pass shape check ****"<<std::endl;
                // 区域提取
                licenceImg = regionalExtract(licenceImg,mRect);
                // 根据颜色再次判断
                if(checkLicenseFromColor(licenceImg)){
                    std::cout<<"--------draw -------------:"<<i<<std::endl;
                    for (int i = 0; i < 4; i++)
                          line(temp2, mPoints[i], mPoints[(i + 1) % 4], Scalar(0, 0,255), 2);

                    imshow("lpe",temp2);
                    imshow("licenceImg1",licenceImg);
                }

            }

#if DEBUG_COUT == 1

            std::cout<<"size: "<<contours.size()<<std::endl;
            std::cout<<"current : "<<i<<" area"<<area<<std::endl;
            std::cout<<"H: "<<height<<" W: "<<weight<<std::endl;
            std::cout<<"mLWR: "<<mLWR<<std::endl;
            std::cout<<"rectRate: "<<rectRate<<std::endl;
            std::cout<<"---------------------------------"<<std::endl;
#elif DEBUG_COUT == 0
            int c = waitKey();
            if(c == 32){
                Mat temp = this->orginImg.clone();
                drawContours(temp,contours,i,Scalar(0,0,255),5);
                std::cout<<"temp in : "<<i<<std::endl;
                imshow("temp",temp);
            }
#endif
        }
        i++;
    }
    return licenceImg;
}

Mat LicencePlateRecognition::licensePlateExtraction(){
    if(this->srcImg.empty()){
        std::cout<<"请先赋值"<<std::endl;
        return Mat();
    }
    Mat img = this->srcImg;
    return licensePlateExtraction(img);
}




int LicencePlateRecognition::OTSU(Mat &srcImage)
{
    int nRows = srcImage.rows;
    int nCols = srcImage.cols;

    int threshold = 0;
    double max = 0.0;
    double AvePix[256];
    int nSumPix[256];
    double nProDis[256];
    double nSumProDis[256];

    for (int i = 0; i < 256; i++)
    {
        AvePix[i] = 0.0;
        nSumPix[i] = 0;
        nProDis[i] = 0.0;
        nSumProDis[i] = 0.0;
    }

    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            nSumPix[(int)srcImage.at<uchar>(i, j)]++;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        nProDis[i] = (double)nSumPix[i] / (nRows*nCols);

    }


    AvePix[0] = 0;
    nSumProDis[0] = nProDis[0];


    for (int i = 1; i < 256; i++)
    {
        nSumProDis[i] = nSumProDis[i - 1] + nProDis[i];
        AvePix[i] = AvePix[i - 1] + i*nProDis[i];
    }

    double mean = AvePix[255];


    for (int k = 1; k < 256; k++)
    {
        double PA = nSumProDis[k];
        double PB = 1 - nSumProDis[k];
        double value = 0.0;
        if (fabs(PA) > 0.001 && fabs(PB) > 0.001)
        {
            double MA = AvePix[k];
            double MB = (mean - PA*MA)/PB;
            value = (double)(PA * PB * pow((MA - MB), 2));
            //或者这样value = (double)(PA * PB * pow((MA-MB),2));//类间方差
            //pow(PA,1)* pow((MA - mean),2) + pow(PB,1)* pow((MB - mean),2)
            if (value > max)
            {
                max = value;
                threshold = k;
            }
        }
    }
    std::cout<<threshold;
    return threshold;
}

Mat LicencePlateRecognition::rotate_demo(Mat &image,RotatedRect rRect){

    Mat dst,M;
    int w = image.cols;
    int h = image.rows;
    // 获取旋转2*3矩阵 arg1:中心坐标 arg2:旋转角度 arg3:
    M = getRotationMatrix2D(Point2f(w/2,h/2),rRect.angle,1.0);

    // 计算旋转后大小
    double cos = abs(M.at<double>(0,0));
    double sin = abs(M.at<double>(0,1));
    int nw = cos*w + sin*h;
    int nh = sin*w + cos*h;
    // 修改矩阵中心
    M.at<double>(0,2) += (nw/2 - w/2);
    M.at<double>(1,2) += (nh/2 - h/2);


    // 旋转函数 原图，目标图，旋转矩阵，大小，插值方式，边缘检测，背景色
    warpAffine(image,dst,M,Size(nw,nh),INTER_LINEAR,0,Scalar(0,0,0));
    // imshow("warp",dst);

    // 图像切割
    Mat licenceImg;
    // 始终保持宽 > 高
    Size rect_size = rRect.size;
    if (rect_size.width < rect_size.height) {
        swap(rect_size.width, rect_size.height);
    }

    Point2f center ;
    center.x =h * sin + (rRect.center.x - rRect.center.y * sin / cos) * cos;
    center.y = rRect.center.y / cos + (rRect.center.x - rRect.center.y * sin / cos) * sin;

    getRectSubPix(dst, rect_size ,center, licenceImg);
    //imshow("crop",licenceImg);

    return licenceImg;

}
