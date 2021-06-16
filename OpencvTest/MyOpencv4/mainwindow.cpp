#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // colorSpace_Demo();

    //mat_creation_demo();

    // pixel_visit_demo();

    //channels_demo();

    //inrange_demo();

    //pixel_statistic_demo();

    //drawing_demo();

    //random_drawing_demo();

    //norm_demo();

    //resize_demo();

    //flip_demo();

    //rotate_demo();

   // video_demo();

    histogram_demo();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::histogram_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));

    // 三通道分离
    std::vector<Mat> bgr_plane;
    split(image,bgr_plane);
    // 定义参数变量
    const int channels[1] = {0};
    const int bins[1] = { 256}; // 总灰度级别
    float hranges[2] = {0,255}; // 通道的取值范围
    const float* ranges[1] = { hranges };
    Mat b_hist;
    Mat g_hist;
    Mat r_hist;
    // 计算Blue,Green,Red的通道直方图
    calcHist(&bgr_plane[0],1,0,Mat(),b_hist,1,bins,ranges);
    calcHist(&bgr_plane[1],1,0,Mat(),g_hist,1,bins,ranges);
    calcHist(&bgr_plane[2],1,0,Mat(),r_hist,1,bins,ranges);
    // 显示直方图
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double)hist_w / bins[0]);
    Mat histImage = Mat::zeros(hist_h,hist_w,CV_8UC3);
    // 归一化直方图数据
    normalize(b_hist,b_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());
    normalize(g_hist,g_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());
    normalize(r_hist,r_hist,0,histImage.rows,NORM_MINMAX,-1,Mat());
    // 绘制直方图曲线
    for(int i = 1;i<bins[0];i++){
        line(histImage,Point(bin_w*(i-1),hist_h-cvRound(b_hist.at<float>(i-1))),
             Point(bin_w*(i),hist_h-cvRound(b_hist.at<float>(i))),Scalar(255,0,0),2,8,0);
        line(histImage,Point(bin_w*(i-1),hist_h-cvRound(g_hist.at<float>(i-1))),
             Point(bin_w*(i),hist_h-cvRound(g_hist.at<float>(i))),Scalar(0,255,0),2,8,0);
        line(histImage,Point(bin_w*(i-1),hist_h-cvRound(r_hist.at<float>(i-1))),
             Point(bin_w*(i),hist_h-cvRound(r_hist.at<float>(i))),Scalar(0,0,255),2,8,0);
    }

    // 显示直方图
    namedWindow("Histogram Demo",WINDOW_AUTOSIZE);
    imshow("Histogram Demo",histImage);
}

// 读取视频
void MainWindow::video_demo(){

    // VideoCapture capture(video_path); // 读取某个视频
    VideoCapture capture;
    capture.open(0);
    if(capture.isOpened()){
        std::cout<<"vedio is open";
    }
    Mat frame;
    while (true) {
        // 读一幀
        capture.read(frame);

        flip(frame,frame,1);

        if(frame.empty()){
            break;
        }
        imshow("frame",frame);
        // TODO: do something

        int c = waitKey(10);
        if(c == 27){// 退出
            break;
        }

    }

    // 释放资源
    capture.release();

}

// 图片旋转
void MainWindow::rotate_demo(){
    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));

    Mat dst,M;
    int w = image.cols;
    int h = image.rows;
    // 获取旋转2*3矩阵 arg1:中心坐标 arg2:旋转角度 arg3:
    M = getRotationMatrix2D(Point2f(w/2,h/2),45,1.0);
    // 计算旋转后大小
    double cos = abs(M.at<double>(0,0));
    double sin = abs(M.at<double>(0,1));
    int nw = cos*w + sin*h;
    int nh = sin*w + cos*h;
    // 修改矩阵中心
    M.at<double>(0,2) += (nw/2 - w/2);
    M.at<double>(1,2) += (nh/2 - h/2);
    // 旋转函数 原图，目标图，旋转矩阵，大小，插值方式，边缘检测，背景色
    warpAffine(image,dst,M,Size(nw,nh),INTER_LINEAR,0,Scalar(255,0,0));
    imshow("warp",dst);
}

// 图片翻转
void MainWindow::flip_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));
    Mat dst;
    //flip(image,dst,0);//上下翻转
    //flip(image,dst,1);//左右翻转
    flip(image,dst,-1);//180度翻转
    imshow("flip",dst);

}

// 放大，缩小
void MainWindow::resize_demo(){
    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));
    Mat zoomin,zoomout;
    int h = image.rows;
    int w = image.cols;
    // 缩小 线性插方
    cv::resize(image,zoomin,Size(w/2,h/2),0,0,INTER_LINEAR);
    imshow("zoomin",zoomin);
    // 放大 线性插方
    cv::resize(image,zoomout,Size(w*1.5,h*1.5),0,0,INTER_LINEAR);
    imshow("zoomout",zoomout);
}

// 归一化处理
void MainWindow::norm_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));
    Mat dst;
    std::cout<<image.type()<<std::endl;
    // 将字符型转换为浮点行
    image.convertTo(image,CV_32F);
    std::cout<<image.type()<<std::endl;
    // 归一化处理
    normalize(image,dst,1.0,0,NORM_MINMAX);
    std::cout<<dst.type()<<std::endl;
    imshow("a",dst);

}

// 随机绘制
void MainWindow::random_drawing_demo(){
    Mat canvas = Mat::zeros(Size(512,512),CV_8UC3);
    int w = canvas.cols;
    int h = canvas.rows;
    // 随机数
    RNG rng(12345);
    while(true){
        int c = waitKey(300);
        if(c == 27){ //退出
            break;
        }
        int x1 = rng.uniform(0,w);
        int y1 = rng.uniform(0,h);
        int x2 = rng.uniform(0,w);
        int y2 = rng.uniform(0,h);
        int b = rng.uniform(0,255);
        int g = rng.uniform(0,255);
        int r = rng.uniform(0,255);
        //canvas = Scalar(0,0,0);
        line(canvas,Point(x1,y1),Point(x2,y2),Scalar(b,g,r),1,LINE_AA,0);
        imshow("randow draw",canvas);

    }
}

// 绘制图像
void MainWindow::drawing_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));
    // 绘制矩形
    Rect rect;
    rect.x = 600;
    rect.y = 100;
    rect.width = 250;
    rect.height = 350;

    Mat bg = Mat::zeros(image.size(),image.type());
    // arg1: 在哪绘制 arg2:绘制什么 arg3:颜色 arg4: >0表示线宽，<0表示填充 arg5: LINA_AA 可以抗锯齿
    rectangle(bg,rect,Scalar(0,0,255),-1,8,0);

    // 绘制圆
    circle(bg,Point(850,450),15,Scalar(255,0,0),1,8,0);

    // 绘制线
    line(bg,Point(600,100),Point(850,450),Scalar(0,255,0),2,LINE_AA,0);

    Mat dst;
    // 两图相加
    addWeighted(image,0.7,bg,0.3,0,dst);

    imshow("h",dst);
}

// 获取最大最小值，平均值方差
void MainWindow::pixel_statistic_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));

    // 像素最小最大值
    double minv,maxv;
    // 对应位置
    Point minLoc,maxLoc;
    std::vector<Mat> mv;
    split(image,mv);
    for(int i=0;i<mv.size();i++){
        // 获取最大值最小值及其坐标
        minMaxLoc(mv[i],&minv,&maxv,&minLoc,&maxLoc,Mat());
        std::cout<<"channels: "<<i<<" min: "<<minv<<" max: "<<maxv<<std::endl;
    }
    Mat mean,stddev;
    // 获取三个通道平均值和方差 ，可选参数mask, 获取目标区域的数据
    meanStdDev(image,mean,stddev);
    std::cout<<"means: "<<mean<<std::endl<<" stddev:"<<stddev<<std::endl;

}

// 像素色彩空间转换
void MainWindow::inrange_demo(){

    Mat image = readImage(cv::String("/home/biliu/Pictures/greenperson.png"));
    Mat hsv;
    cvtColor(image,hsv,COLOR_BGR2HSV);
    Mat mask;
    inRange(hsv,Scalar(35,43,46),Scalar(77,255,255),mask);

    //cvtColor(image,image,COLOR_BGR2RGB);
    showImage(ui->label,MatToQImage(image));
    showImage(ui->label_3,MatToQImage(mask));

    Mat redback = Mat::zeros(image.size(),image.type());
    // 红色
    redback = Scalar(40,40,200);
    // 像素取反 获取目标区域
    bitwise_not(mask,mask);
    image.copyTo(redback,mask);
    cvtColor(redback,redback,COLOR_BGR2RGB);
    showImage(ui->label_2,MatToQImage(redback));
}

// 图像读取与转换
void MainWindow::colorSpace_Demo(){

    cv::Mat image = cv::imread("/home/biliu/Downloads/pictures/img2.png"); // IMREAD_GRAYSCALE 显示为灰度图
    if(image.empty()){
        printf("con not open the image ");
        return ;
    }

    cv::Mat img,hsv,gray;

    // 显示原画
    cv::cvtColor(image,img,cv::COLOR_BGR2RGB);
    showImage(ui->label,MatToQImage(img));

    // 显示灰度图像
    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);
    showImage(ui->label_3,MatToQImage(gray));

    // 保存图片
    //imwrite("/home/biliu/Pictures/gray.png",gray)

}

// Mat图像的构建
void MainWindow::mat_creation_demo(){
    // cv::Mat m1.m2;
    // m1 = image.clone();
    // image.copyTo(m2);

    // 创建空白图像
    // zeros(Size(8,8,CV_8UC3)) 初始化为8*8的全0,3通道图片
    Mat m3 = Mat::zeros(Size(8,8),CV_8UC3);
    m3 = Scalar(0,0,255); // 赋值
    std::cout<<"width: "<<m3.cols<<" height: "<<m3.rows<<" channels: "<<m3.channels()<<std::endl;
    std::cout<<m3<<std::endl;
    showImage(ui->label_2,MatToQImage(m3));

}

// 像素获取与赋值
void MainWindow::pixel_visit_demo(){
    Mat image = imread("/home/biliu/Downloads/pictures/img2.png"); // IMREAD_GRAYSCALE 显示为灰度图
    if(image.empty()){
        std::cout<<"con not open the image ";
        return ;
    }
    Mat img ;

    cvtColor(image,img,COLOR_BGR2RGB);
    showImage(ui->label,MatToQImage(img));

    // 获取宽度
    int width = image.cols;
    // 获取高度
    int height = image.rows;
    // 获取通道数
    int dims = image.channels();

    for(int row=0;row<height;row++){
        uchar* current_row = image.ptr<uchar>(row);

        for(int col=0;col<width;col++){
            // 灰度图像
            if(dims == 1){
                int px = *current_row;
                *current_row++ = 255-px;
            }
            // 彩色图像
            if(dims == 3){
                *current_row++ = 255 - *current_row;
                *current_row++ = 255 - *current_row;
                *current_row++ = 255 - *current_row;
            }
        }
    }
    showImage(ui->label_2,MatToQImage(image));


}

// 通道分离与合并
void MainWindow::channels_demo(){
    Mat image = imread("/home/biliu/Downloads/pictures/img2.png"); // IMREAD_GRAYSCALE 显示为灰度图
    if(image.empty()){
        std::cout<<"con not open the image ";
        return ;
    }

    std::vector<Mat> mv;
    // 将图片进行通道分离
    split(image,mv);
    showImage(ui->label,MatToQImage(mv[0]));
    showImage(ui->label_2,MatToQImage(mv[1]));
    showImage(ui->label_3,MatToQImage(mv[2]));

    Mat dst;
    mv[0] = 0;
    // 合并通道
    merge(mv,dst);
    showImage(ui->label,MatToQImage(dst));

}

// 读取目标路径图片
Mat MainWindow::readImage(const cv::String& file_path){
    Mat image = imread(file_path); // IMREAD_GRAYSCALE 显示为灰度图
    if(image.empty()){
        printf("con not open the image ");
        exit(-1);
    }
    Mat temp;
    cvtColor(image,temp,COLOR_BGR2RGB);
    showImage(ui->label_0,MatToQImage(temp));

    return image;
}

// 将Mat图片转换为对应的QImage图片
QImage MainWindow::MatToQImage(const cv::Mat &mat){
    switch (mat.type())
        {
            // 8-bit, 4 channel
            case CV_8UC4:
                {
                    QImage image(mat.data,
                                 mat.cols, mat.rows,
                                 static_cast<int>(mat.step),
                                 QImage::Format_ARGB32);

                    return image;
                 }

             // 8-bit, 3 channel
             case CV_8UC3:
                {
                    QImage image(mat.data,
                                 mat.cols, mat.rows,
                                 static_cast<int>(mat.step),
                                 QImage::Format_RGB888);

                    //return image.rgbSwapped();
                    return image;
                }

             // 8-bit, 1 channel
            case CV_8UC1:
                {
                    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                    QImage image(mat.data,
                                 mat.cols, mat.rows,
                                 static_cast<int>(mat.step),
                                 QImage::Format_Grayscale8);
                    #else
                    static QVector<QRgb>  sColorTable;

                    // only create our color table the first time
                    if (sColorTable.isEmpty())
                    {
                           sColorTable.resize( 256 );

                           for ( int i = 0; i < 256; ++i )
                           {
                              sColorTable[i] = qRgb( i, i, i );
                           }
                    }

                    QImage image(mat.data,
                                 mat.cols, mat.rows,
                                 static_cast<int>(mat.step),
                                 QImage::Format_Indexed8 );

                    image.setColorTable(sColorTable);
                    #endif

                    return image;
                }

            // wrong
            default:
                qDebug() << "ERROR: Mat could not be converted to QImage.";
                break;
        }
        return QImage();
}

// 显示图片到某个标签
void MainWindow::showImage(QLabel* label,QImage img){
    label->setPixmap(QPixmap::fromImage(img));
    label->setScaledContents(true);
}