#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 读取图片
    this->originImg = imread("../pic/cartest.jpg");
    imshow("originImg",this->originImg);

    if (this->originImg.cols != 640)
            cv::resize(this->originImg, this->originImg, Size(640, 640 * this->originImg.rows / this->originImg.cols));
    /*
    if (this->originImg.cols < 640)
            cv::resize(this->originImg, this->originImg, Size(640, 640 * this->originImg.rows / this->originImg.cols));
    */

    Mat srcImg = this->originImg.clone();

    test(srcImg);

   // test2();
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

MainWindow::~MainWindow()
{
    delete ui;
}



