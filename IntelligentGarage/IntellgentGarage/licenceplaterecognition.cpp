#include "licenceplaterecognition.h"
#include "QString"

#define SOBEL_TYPE 1
#define DEBUG_COUT 1
#define MY_DEBUG 1
#define SHOW_ALL 0
#define SHOW_ONE 0

using namespace std;

typedef pair<int, double> PAIR;
struct CmpByValue {
  bool operator()(const PAIR& lhs, const PAIR& rhs) {
    return lhs.second > rhs.second;
  }
};



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


/*
    imshow("t1",img);

    Mat imageRGB[3];
    split(img, imageRGB);
    for (int i = 0; i < 3; i++)
    {
        equalizeHist(imageRGB[i], imageRGB[i]);
    }
    Mat equalizeHistDist;
    merge(imageRGB, 3, img);


    imshow("t2",img);
*/
    // 1 彩色图转灰度图
    cvtColor(img,img,COLOR_BGR2GRAY);

    // 2 高斯滤波，中值滤波
    // 2.1 高斯滤波
    GaussianBlur(img,img,Size(3,3),0,0,BORDER_DEFAULT);
   // imshow("Gaussian", img);
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
#elif SOBEL_TYPE == 0
    Mat sobelImg,sobelX,sobelY;
    Sobel(img,sobelImg,CV_16S,1,0,3);
    convertScaleAbs(sobelImg,sobelX);

    Sobel(img,sobelImg,CV_16S,0,1,3);
    convertScaleAbs(sobelImg,sobelY);

    addWeighted(sobelX,0.7,sobelY,0.3,0,img);
#endif
    this->srcImg = img;
    // SHOW_ONE 2
#if SHOW_ALL == 1 || SHOW_ONE == 1
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
    threshold(img,img,OTSU(img),255,THRESH_BINARY);
    //imshow("img7", img);
    // 形态学处理
    // 闭运算:

    Mat element  = getStructuringElement(MORPH_RECT,Size(17,5));
    morphologyEx(img,img,MORPH_CLOSE,element);
    /*
    Mat element1  = getStructuringElement(MORPH_RECT,Size(3,3));
    erode(img,img,element1);
  //  imshow("a0",img);

    Mat element  = getStructuringElement(MORPH_RECT,Size(17,5));
    morphologyEx(img,img,MORPH_CLOSE,element);
   // imshow("a1",img);
   // morphologyEx(img,img,MORPH_OPEN,element);
    //imshow("a2",img);


    dilate(img,img,element);
   // imshow("a2",img);
    Mat element2  = getStructuringElement(MORPH_RECT,Size(17,7));
    morphologyEx(img,img,MORPH_CLOSE,element2);

    Mat element3  = getStructuringElement(MORPH_RECT,Size(17,9));
    dilate(img,img,element3);
*/
#if SHOW_ALL == 1 || SHOW_ONE == 0
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
    const float ERROR_RATE = 1.5;       // 浮动范围
    float errEara = 0.5;
    float minLWR = LENGTH_WIDTH_RATIO - ERROR_RATE + 0.8;  // 最小长宽比
    float maxLWR = LENGTH_WIDTH_RATIO + ERROR_RATE;  // 最大长宽比


    if(mLWR>minLWR && mLWR < maxLWR && rectRate > 1 - errEara && rectRate < 1 + errEara
            && area > 1500 && area<80000){
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
   // imshow("mask",mask);
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
   // imshow("licenceImg",licenceImg);


    return licenceImg;
}


void LicencePlateRecognition::regionalExtract2(Mat &img){
    Mat OriginalImg;

    OriginalImg = img.clone();
    if (OriginalImg.empty())  //判断图像对否读取成功
    {
        std::cout << "错误!读取图像失败\n";
        return ;
    }
    //imshow("originalimg", OriginalImg); //显示原始图像
    std::cout << "Width:" << OriginalImg.rows << "\tHeight:" << OriginalImg.cols << std::endl;//打印长宽

    Mat ResizeImg = OriginalImg.clone();
    /*
    Mat ResizeImg;
    if (OriginalImg.cols != 640)
        cv::resize(OriginalImg, ResizeImg, Size(640, 640 * OriginalImg.rows / OriginalImg.cols));
    imshow("resize", ResizeImg);
*/

    unsigned char pixelB, pixelG, pixelR;  //记录各通道值
    unsigned char DifMax = 80;             //基于颜色区分的阈值设置
    unsigned char B = 138, G = 63, R = 23; //各通道的阈值设定，针对与蓝色车牌
    Mat BinRGBImg = ResizeImg.clone();  //二值化之后的图像
    int i = 0, j = 0;
    for (i = 0; i < ResizeImg.rows; i++)   //通过颜色分量将图片进行二值化处理
    {
        for (j = 0; j < ResizeImg.cols; j++)
        {
            pixelB = ResizeImg.at<Vec3b>(i, j)[0]; //获取图片各个通道的值
            pixelG = ResizeImg.at<Vec3b>(i, j)[1];
            pixelR = ResizeImg.at<Vec3b>(i, j)[2];

            if (abs(pixelB - B) < DifMax && abs(pixelG - G) < DifMax && abs(pixelR - R) < DifMax)
            {                                           //将各个通道的值和各个通道阈值进行比较
                BinRGBImg.at<Vec3b>(i, j)[0] = 255;     //符合颜色阈值范围内的设置成白色
                BinRGBImg.at<Vec3b>(i, j)[1] = 255;
                BinRGBImg.at<Vec3b>(i, j)[2] = 255;
            }
            else
            {
                BinRGBImg.at<Vec3b>(i, j)[0] = 0;        //不符合颜色阈值范围内的设置为黑色
                BinRGBImg.at<Vec3b>(i, j)[1] = 0;
                BinRGBImg.at<Vec3b>(i, j)[2] = 0;
            }
        }
    }
    imshow("bin", BinRGBImg);        //显示二值化处理之后的图像

    Mat BinOriImg;     //形态学处理结果图像
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); //设置形态学处理窗的大小
    dilate(BinRGBImg, BinOriImg, element);     //进行多次膨胀操作
    dilate(BinOriImg, BinOriImg, element);
    dilate(BinOriImg, BinOriImg, element);

    erode(BinOriImg, BinOriImg, element);      //进行多次腐蚀操作
    erode(BinOriImg, BinOriImg, element);
    erode(BinOriImg, BinOriImg, element);
    imshow("bin2", BinOriImg);        //显示形态学处理之后的图像

}

/**
* ---------------------------------------------------------------------------------------------------------
* 车牌颜色判断
* img: 需要处理的图像
* ---------------------------------------------------------------------------------------------------------
*/
bool LicencePlateRecognition::checkLicenseFromColor(Mat img){

#if SOBEL_TYPE == 1
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

    if(MY_DEBUG == 1){
        std::cout<<"mean: "<<std::endl;
        std::cout<<mean<<std::endl;
        std::cout<<"mb: "<<mB<<std::endl;
        std::cout<<"mg: "<<mG<<std::endl;
        std::cout<<"mr: "<<mR<<std::endl;
        imshow("color check",img);
    }

    if(mB < ERROR_RANGE && mG < ERROR_RANGE && mR < ERROR_RANGE){
          return true;
    }

#elif SOBEL_TYPE == 0

    const int ERROR_RANGE = 10;
    const int BLUE_WHITE_GRAY = 128;

    cvtColor(img,img,COLOR_BGR2GRAY);
    equalizeHist(img,img);

    Mat mean,stddev; // 平均值，方差
    meanStdDev(img,mean,stddev);

    double pixelGray = mean.at<double>(0,0);

    if(abs(pixelGray - BLUE_WHITE_GRAY) < ERROR_RANGE){
        return true;
    }

#endif
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
    Mat licenceImg;

    double area = 0;    //轮廓面积
    int height = 0;     //外接矩高
    int weight = 0;     //外接矩长
    float mLWR = 0;     //当前长宽比
    int mArea = 0;      //外接距面积
    float rectRate = 1; //矩形率

    std::vector<std::vector<Point>> contours;
    findContours(img,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);
    std::map<int,double> contours_areas;
    for(int i = 0; i< contours.size(); i++){
        double area = contourArea(contours[i]);
        if(area > 800 && area < 50000)
            contours_areas.insert(make_pair(i,area));
    }
    vector<PAIR> index_area_vec(contours_areas.begin(), contours_areas.end());
    sort(index_area_vec.begin(), index_area_vec.end(), CmpByValue());

    double last_area = 0;
    double current_area = 0;
    int final_index = 0;
    for (int i = 0; i != index_area_vec.size(); ++i) {
        //cout<<index_area_vec[i].first<<" "<<index_area_vec[i].second<<endl;
        // 获取最小外接矩
        RotatedRect mRect = minAreaRect(contours[index_area_vec[i].first]);
        // 获取四个角点坐标
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
        area = index_area_vec[i].second;
        rectRate = area/mArea;

        std::cout<<"current : "<<index_area_vec[i].first<<" area"<<index_area_vec[i].second<<std::endl;
        std::cout<<"H: "<<height<<" W: "<<weight<<std::endl;
        std::cout<<"mLWR: "<<mLWR<<std::endl;
        std::cout<<"rectRate: "<<rectRate<<std::endl;
        std::cout<<"---------------------------------"<<std::endl;

        if(checkLicenseFromShape(mLWR,rectRate,area) ){
            std::cout<<"**** pass shape check ****"<<index_area_vec[i].first<<std::endl;
            current_area = index_area_vec[i].second;
            // 优先选择面积接近6000的
            if(abs(current_area - 6000) > abs(last_area - 6000)){
                cout<<"out becease:---last_area---- "<<last_area<<" current_area"<<current_area<<endl;
                continue ;
            }

            last_area = current_area;
            final_index = index_area_vec[i].first;

            cout<<"---aaa---- "<<index_area_vec[i].first<<" pass"<<endl;
            Point2f mPoints[4];// 左下，左上，右上，右下
            mRect.points(mPoints);
            Point points[4];
            for(int i=0;i<4;i++){
                points[i].x = (int) mPoints[i].x;
                points[i].y = (int) mPoints[i].y;
            }
            for (int i = 0; i < 4; i++)
                  line(temp2, mPoints[i], mPoints[(i + 1) % 4], Scalar(0, 0,255), 2);

            imshow("lpe",temp2);


        }


    }

    if(final_index != 0){
        // 区域提取
        RotatedRect fRect = minAreaRect(contours[final_index]);
        this->orginImg.copyTo(licenceImg);
        licenceImg = regionalExtract(licenceImg,fRect);
        imshow("licenceImg",licenceImg);
        characterExtraction(licenceImg);
    }


    /*


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
#if MY_DEBUG == 0
        int c = waitKey();
        if(c == 32){
            Mat temp = this->orginImg.clone();
            drawContours(temp,contours,i,Scalar(0,0,255),5);
            std::cout<<"temp in : "<<i<<std::endl;
            imshow("temp",temp);
        }
#endif
        i++;
    }


    */



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
   // std::cout<<"otu : "<<threshold<<endl;
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

void LicencePlateRecognition::characterExtraction(Mat img){

    Size s(144,33);
    resize(img,img,s,0,0,INTER_CUBIC);

    Mat dst ;
   // GaussianBlur(img,dst,Size(3,3),0,0,BORDER_DEFAULT);
    bilateralFilter(img,dst,0,100,10);
   // imwrite("../pic/licence5.jpg",dst);

    Mat srcImg = dst.clone();
    imshow("srcIg",srcImg);
   // imshow("ce0",dst);
    cvtColor(dst,dst,COLOR_BGR2GRAY);
   // blur(dst,dst,Size(3,3));
    imshow("ce1",dst);
    //equalizeHist(dst,dst);
    //imshow("ce2",dst);

    Mat gray = dst.clone();
    int sold = OTSU(dst);
    threshold(dst,dst,OTSU(dst),255,THRESH_BINARY);
    //threshold(dst,dst,0,255,THRESH_OTSU);

    imshow("ce3",dst);

    Mat element = getStructuringElement(MORPH_RECT,Size(2,1));
    erode(dst,dst,element);

    imshow("ce",dst);

    // 获取宽度
    int width = dst.cols;
    // 获取高度
    int height = dst.rows;

    int black_num = 0;

    int x1[10];
    int x2[10];
    int pos = 0;

    for(int col=0;col<width;col++){

        int white_num = 0;

        for(int row=4;row<30;row++){
            int px = dst.at<uchar>(row,col);
            if(px == 255)
                white_num ++ ;
        }

        if(white_num > 5){
            white_num = 0;
            int flog_num = 0;
            int ncol = col + 12;
            int col2 = col;
            for(col2;col2<=ncol;col2++){
                for(int row=4;row<30;row++){
                    int px = dst.at<uchar>(row,col2);
                    if(px == 255){
                        white_num++;
                        if(col2 == ncol)
                            flog_num++;
                    }

                }
            }
            if(white_num > 50 && flog_num < 6){

                x1[pos] = col - 3;
                x2[pos] = col2;
                pos++;
                col = ncol ;
            }

        }

    }


  Mat num[10];
  for(int x=0;x<pos;x++){
    cout<<"x: "<<x<<" x1: "<<x1[x]<<" x2:"<<x2[x]<<endl;

    float width2 = x2[x] - x1[x];
    imshow("gray123",gray);
    getRectSubPix(dst,Size(width2,25),Point2f(x1[x]+width2/2,16),num[x],-1);
    //GaussianBlur(num[x],num[x],Size(3,3),0,0,BORDER_DEFAULT);
    threshold(num[x],num[x],0,255,THRESH_OTSU);

   // imshow("dstasdad",);

    char str[3];

    sprintf(str,"%d",x);
    std::string name(str);
    imshow(name,num[x]);

  }


  // char(['0':'9' 'A':'H' 'J':'N' 'P':'Z' 34:z 35 '藏川鄂甘赣桂贵黑沪吉冀津晋京辽鲁蒙闽宁青琼陕苏皖湘新渝豫粤云浙'])

  /*
  char cnum[200] = {'0','1','2','3','4','5','6','7','8','9'
                   ,'A','B','C','D','E','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'
                   ,'藏','川','鄂','甘','赣','桂','贵','黑','沪','吉','冀','津','晋','京','辽','鲁','蒙','闽','宁','青'
                   ,'琼','陕','苏','皖','湘','新','渝','豫','粤','云','浙'};
  */

 // char cnum1[2] = {'藏','a'};

  string cnum[200] = {"0","1","2","3","4","5","6","7","8","9"
                   ,"A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z"
                   ,"藏","川","鄂","甘","赣","桂","贵","黑","沪","吉","冀","津","晋","京","辽","鲁","蒙","闽","宁","青"
                   ,"琼","陕","苏","皖","湘","新","渝","豫","粤","云","浙"};
 // string cnum1[100] = {"藏","川鄂甘赣桂贵黑沪吉冀津晋京辽鲁蒙闽宁青琼陕苏皖湘新渝豫粤云浙"};
  //QString carnum1 = "藏川鄂甘赣桂贵黑沪吉冀津晋京辽鲁蒙闽宁青琼陕苏皖湘新渝豫粤云浙";
  cout<<"asd  "<<cnum[34]<<endl;
 // string tt = "藏川鄂甘赣桂贵黑沪吉冀津晋京辽鲁蒙闽宁青琼陕苏皖湘新渝豫粤云浙";

  //char tt = '浙';

  std::string path = "../train1/licence_plate_";
  char pathNum[3];
  char fileNum[3];
  std::string type = ".jpg";
  std::string srcPath ;
  double mlikeRate[65];

  float theBest[65][2] = {0};

  for(int p = 34;p<65;p++){

      string strPathName = cnum[p];

      double bestLike = 0;
      float avgLike = 0;
      float sumLike = 0;


      for(int n=1;n<10;n++){

          string strFileName = cnum[p] + "_ ";
          sprintf(fileNum,"%d",n);
          std::string FileName(fileNum);
          strFileName += FileName;
          srcPath = path + strPathName +"/"+ strFileName + type;
          cout<<"path: "<<srcPath<<endl;
          Mat train8 = imread(srcPath);
          cvtColor(train8,train8,COLOR_BGR2GRAY);
         // cv::resize(train8,train8,Size(16,25),0,0,INTER_CUBIC);
          threshold(train8,train8,0,255,THRESH_OTSU);
         // imshow("train8",train8);

          double likeRate = 0;
          for(int x = 0;x<20;x++){
              for(int y=0;y<20;y++){
                  int px1 = num[0].at<uchar>(x,y);
                  int px2 = train8.at<uchar>(x,y);
                  if(px1 == px2 && px1 == 255){
                      likeRate++;
                  }

              }
          }

          if(bestLike < likeRate){
              bestLike = likeRate;
              mlikeRate[p] = bestLike;
          }


          Mat result;
          Mat rNum;
          cv::resize(num[0],rNum,train8.size());
          matchTemplate(rNum,train8,result,TM_CCOEFF);
        //  cout<<"result"<<result<<endl;
          if(theBest[p][0]<result.at<float>(0,0)){
              theBest[p][0] = result.at<float>(0,0);
              //cout<<"asd"<<result.at<float>(0,0)<<endl;
          }
      sumLike += result.at<float>(0,0);
      }

      avgLike = sumLike / 10;
      cout<<"p: "<<p<<"bestlike: "<<mlikeRate[p] / (20 * 20)<<endl;
      cout<<"p: "<<p<<"---------- avglike: "<<avgLike<<endl;

  }

  int bestNum = 0;
  float temp2=0;
  for(int i=34;i<65;i++){
      cout<<"p: "<<i<<"--newLike--："<<theBest[i][0]<<endl;
      if(temp2 < theBest[i][0]){
          bestNum = i;
          temp2 = theBest[i][0];
      }
  }

  cout<<"the best num: "<<bestNum<<endl;
  cout<<"the province: "<<cnum[bestNum]<<endl;


}
