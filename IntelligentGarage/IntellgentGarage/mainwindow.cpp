#include "mainwindow.h"
#include "ui_mainwindow.h"
#define MY_DEBUG 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initNet();


#if MY_DEBUG == 0

    // 读取图片
    this->originImg = imread("../pic/carnum3.jpg");
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


    QString mFolderPath ="../pic";
    QStringList mImgNames = getFileNames(mFolderPath);
    for (int i = 0; i < mImgNames.size(); ++i)
    {
        qDebug() << "entryList: " << i << "-" << mImgNames[i];
        QString temp = mImgNames[i];
        std::string srcPath = mFolderPath.toStdString() +"/" + temp.toStdString();
        Mat img = imread(srcPath);
        if (img.cols != 640)
                cv::resize(img, img, Size(680, 480));

        imshow("test111",img);
        test(img);
        cv::waitKey();

    }

    /*
    std::string path = "../pic/carnum";
    std::string type = ".jpg";
    std::string srcPath ;
    int l = 16;
    Mat img;
    for(int i=1;i<l;i++){
        char num[3];
        sprintf(num,"%d",i);
        std::string name(num);
        srcPath = path + name + type;
        std::cout<<"//--------**  "<<srcPath<<"  **-----------//"<<std::endl;
        img = imread(srcPath);
        if (img.cols != 640)
                cv::resize(img, img, Size(680, 480));

        imshow("test111",img);
        test(img);
        cv::waitKey();
    }

    */
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

void MainWindow::initNet(){
    // 实例化 init
    server = new QTcpServer(this);
    // 监听 第一个为IP地址 第二个为端口号
    server->listen(QHostAddress("192.168.43.131"),8080);
    // 新的连接 收到一个newConnection信号 处理函数采用lamda表达式
    connect(server,&QTcpServer::newConnection,this,&MainWindow::mConnect);
}

void MainWindow::mConnect(){
    // 接收客户端的连接对象 相当与 accept
    // sock_addr 结构体 == 类 QTcpSocket
    conn = server->nextPendingConnection();
    std::cout<<"有新的连接...："<<std::endl;

    // 保证conn是一个有效对象，所以写在里面
    connect(conn,&QTcpSocket::readyRead,this,&MainWindow::readMsg);
}

void MainWindow::readMsg(){
    // 接收数据
    QByteArray array = conn->readAll();
  //  ui->record->append(array);
    std::string msg = array.toStdString();
    std::cout<<"收到数据："<<msg<<std::endl;

    handMsg(msg);


}

void MainWindow::sendMsg(QString msg){
    // 发送数据 toPlainText 将文本框内容转换为纯字符串
    if(!msg.isEmpty() && !msg.isNull()){
        conn->write(msg.toUtf8().data());
        std::cout<<"发送："<<msg.toStdString()<<std::endl;
    }
}


void MainWindow::on_btn_door_clicked()
{
    QString msg = "btnDoor onclick";
    sendMsg(msg);
}


void MainWindow::handMsg(std::string msg){

    if(msg==LED_0_ON){
       ui->label_led_0_states->setText("ON");
       ui->btn_led_0->setText("关灯");
    }
    if(msg==LED_0_OFF){
       ui->label_led_0_states->setText("OFF");
       ui->btn_led_0->setText("开灯");
    }

    if(msg==LED_1_ON){
       ui->label_led_1_states->setText("ON");
       ui->btn_led_1->setText("关灯");
    }
    if(msg==LED_1_OFF){
       ui->label_led_1_states->setText("OFF");
       ui->btn_led_1->setText("开灯");
    }

    if(msg==DOOR_OPEN){
       ui->label_door_states->setText("开");
       ui->btn_door->setText("关门");
    }
    if(msg==DOOR_CLOSE){
       ui->label_door_states->setText("关");
       ui->btn_door->setText("开门");
    }

    if(msg==WARING_OPEN){
       ui->btn_waring->setText("关闭警告");
    }
    if(msg==WARING_CLOSE){
       ui->btn_waring->setText("警告");
    }


}

QStringList MainWindow::getFileNames(const QString &path)
{
    QStringList mImgNames = {""};
    QString mFolderPath =path;
    if (mFolderPath.isEmpty()) return mImgNames;
    // 获取所有文件名
    QDir dir(mFolderPath);
    if (!dir.exists()) mImgNames = QStringList("");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    dir.setNameFilters(QString("*.jpg").split(";"));
    mImgNames = dir.entryList();
    return mImgNames;
}
