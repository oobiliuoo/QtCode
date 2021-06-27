#include "mainwindow.h"
#include "ui_mainwindow.h"
#define MY_DEBUG 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#if MY_DEBUG == 1

    // 读取图片
    this->originImg = imread("../pic/carnum6.jpg");
    imshow("originImg",this->originImg);
    if (this->originImg.cols != 640)
            cv::resize(this->originImg, this->originImg, Size(640, 640 * this->originImg.rows / this->originImg.cols));
    Mat srcImg = this->originImg.clone();
    test(srcImg);

#elif MY_DEBUG == 0
    test2();

#elif MY_DEBUG == 0
    readVideo();
#endif
}

void MainWindow::test2(){

    std::string path = "../pic/carnum";
    std::string type = ".jpg";
    std::string srcPath ;
    int l = 13;
    Mat img;
    for(int i=1;i<l;i++){
        char num[3];
        sprintf(num,"%d",i);
        std::string name(num);
        srcPath = path + name + type;
        std::cout<<"//--------**  "<<srcPath<<"  **-----------//"<<std::endl;
        img = imread(srcPath);
        if (img.cols != 640)
                cv::resize(img, img, Size(640, 640 * img.rows / img.cols));

        imshow("test111",img);
        test(img);
        cv::waitKey();
    }

}

void MainWindow::test(Mat srcImg){

    LPR mLPR(srcImg);
    mLPR.pictureProcessing();
    mLPR.borderDetection();
    mLPR.morphologicalProcessing();
    mLPR.licensePlateExtraction();
}


void MainWindow::readVideo(){
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
        flip(frame,frame,1);
        imshow("frame",frame);
        // TODO: do something


        this->originImg = frame.clone();
        if (this->originImg.cols != 640)
                cv::resize(this->originImg, this->originImg, Size(640, 640 * this->originImg.rows / this->originImg.cols));
        Mat srcImg = this->originImg.clone();
        test(srcImg);
        waitKey(10);
    }

    // 释放资源
    capture.release();
}


MainWindow::~MainWindow()
{
    delete ui;
}



