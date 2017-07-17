#include "mainwindow.h"
#include <QWidget>

#include <QApplication>
#include <QDesktopWidget>

#include <QIcon>
#include <QPixmap>
#include <QFont>

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QSpacerItem>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>

#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend.h>

#include <QTime>

#include <databaseconnection.h>

#include <QSqlError>

#include <QtNetwork/QTcpSocket>
#include <QTcpServer>

#include <QHeaderView>
#include <QScrollBar>

#include <QDateTime>
#include <QMessageBox>

#include <QFileDialog>
#include <QDir>

#include "kinematicsolution.h"

#include "mythread.h"

#include "communication.h"

// 编译时未报错，但生成的程序中文乱码
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

QString MainWindow::portName = "COM1";
int MainWindow::buadRate = 9600;
int MainWindow::dataBitsIndex = 3;
int MainWindow::parityIndex = 2;
int MainWindow::stopBitsIndex = 0;

bool MainWindow::communicationState = false;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    /*      背景图片自定义
    QPixmap pixmap1(":/image/backImage.jpg");
    QPalette bgpal = palette();
    bgpal.setBrush(backgroundRole(), QBrush(pixmap1));
    bgpal.setColor (QPalette::Foreground, QColor (0,0,0,255));
    setPalette(bgpal);
    */

    //重定义界面布局
    QWidget *widget = new QWidget();
    this->setCentralWidget(widget);
    this->resize(WIN_W, WIN_H);
    this->setWindowState(Qt::WindowMaximized);

    //kuka六自由度机械臂通信
    tcpSocket = new QTcpSocket();

    //记录保存相关初始化
    QString runPath = QCoreApplication::applicationDirPath();
    sendFileName.clear();
    sendFileName.append(tr("%1/%2/SendLogFile_%3.txt").arg(runPath).arg("SendRecordFold").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss")));
    qDebug()<<sendFileName;
    sendFileInstance = new QFile(sendFileName);
    if(!sendFileInstance->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Truncate))
        QMessageBox::warning(this, tr("提示信息"), tr("记录文件创建失败，将无法保存程序收发数据记录！"), QMessageBox::Ok);
    sendOut = new QTextStream(sendFileInstance);

    receiveFileName.clear();
    receiveFileName.append(tr("%1/%2/ReceiveLogFile_%3.txt").arg(runPath).arg("ReceiveRecordFold").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss")));
    receiveFileInstance = new QFile(receiveFileName);
    if(!receiveFileInstance->open(QIODevice::Text|QIODevice::WriteOnly|QIODevice::Truncate))
        QMessageBox::warning(this, tr("提示信息"), tr("记录文件创建失败，将无法保存程序收发数据记录！"), QMessageBox::Ok);
    receiveOut = new QTextStream(receiveFileInstance);

    buttonInitialize();
    MainWindowInit();
    signalSlotConnection();

    // 车臂系统轨迹规划相关成员变量初始化
    velocityValue = 0;
    MyThread::getInstance()->start();

    // 定时器初始化，定期对刷新界面
    updateInterfaceTimer.start(10);
}

MainWindow::~MainWindow()
{
    sendFileInstance->close();
    receiveFileInstance->close();

    // 终止线程
    MyThread::getInstance()->stop();
    MyThread::getInstance()->wait();
    MyThread::destory();
}


void MainWindow::signalSlotConnection()
{
    //菜单栏
    QObject::connect(communicationBtn, SIGNAL(clicked(bool)), this, SLOT(communicationBtn_clicked()), Qt::UniqueConnection);
    QObject::connect(systemParaBtn, SIGNAL(clicked(bool)), this, SLOT(systemParaBtn_clicked()), Qt::UniqueConnection);
    QObject::connect(userInterfaceBtn, SIGNAL(clicked(bool)), this, SLOT(userInterfaceBtn_clicked()), Qt::UniqueConnection);
    QObject::connect(historyRecordBtn, SIGNAL(clicked(bool)), this, SLOT(historyRecordBtn_clicked()), Qt::UniqueConnection);
    QObject::connect(managerBtn, SIGNAL(clicked(bool)), this, SLOT(managerBtn_clicked()), Qt::UniqueConnection);
    QObject::connect(aboutBtn, SIGNAL(clicked(bool)), this, SLOT(aboutBtn_clicked()), Qt::UniqueConnection);
    //密码管理页面
    QObject::connect(passWordConfigBtn, SIGNAL(clicked(bool)), this, SLOT(passWordConfigBtn_Clicked()), Qt::UniqueConnection);
    QObject::connect(passWordCancelBtn, SIGNAL(clicked(bool)), this, SLOT(passWordCancelBtn_Clicked()), Qt::UniqueConnection);   
    //通信相关
    QObject::connect(connectBtn, SIGNAL(clicked(bool)), this, SLOT(connectBtn_Clicked()), Qt::UniqueConnection);
    QObject::connect(resetBtn, SIGNAL(clicked(bool)), this, SLOT(resetBtn_Clicked()), Qt::UniqueConnection);
    QObject::connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(kukaReceiveMessage()), Qt::UniqueConnection);
    QObject::connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError()), Qt::UniqueConnection);
    //读写记录文件
    QObject::connect(openSendBtn, SIGNAL(clicked(bool)), this, SLOT(readSendRecordFun()),  Qt::UniqueConnection);
    QObject::connect(openReceiveBtn, SIGNAL(clicked(bool)), this, SLOT(readReceiveRecordFun()), Qt::UniqueConnection);

    //速度值更新
    QObject::connect(velocitySlider, SIGNAL(valueChanged(int)), this, SLOT(velocityUpdate(int)), Qt::UniqueConnection);
    //执行动作槽函数
    QObject::connect(operationBtn, SIGNAL(clicked(bool)), this, SLOT(executeRoutine()), Qt::UniqueConnection);
    //急停槽函数
    QObject::connect(stopBtn, SIGNAL(clicked(bool)), this, SLOT(stopRoutine()), Qt::UniqueConnection);
    //模式更新槽函数
    QObject::connect(straightLineRadioBtn, SIGNAL(clicked(bool)), this, SLOT(modelStraightPath(bool)));
    QObject::connect(circularPathRadioBtn, SIGNAL(clicked(bool)), this, SLOT(modelCircularPath(bool)));
    QObject::connect(pointToPointRadioBtn, SIGNAL(clicked(bool)), this, SLOT(modelPointToPoint(bool)));
    QObject::connect(demo1RadioBtn, SIGNAL(clicked(bool)), this, SLOT(modelDemo1(bool)));
    QObject::connect(demo2RadioBtn, SIGNAL(clicked(bool)), this, SLOT(modelDemo2(bool)));
    //单关节微调槽函数
    QObject::connect(jointRowL1, SIGNAL(clicked(bool)), this, SLOT(jointL1Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowL2, SIGNAL(clicked(bool)), this, SLOT(jointL2Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowL3, SIGNAL(clicked(bool)), this, SLOT(jointL3Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowL4, SIGNAL(clicked(bool)), this, SLOT(jointL4Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowL5, SIGNAL(clicked(bool)), this, SLOT(jointL5Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowL6, SIGNAL(clicked(bool)), this, SLOT(jointL6Clicked()), Qt::UniqueConnection);

    QObject::connect(jointRowR1, SIGNAL(clicked(bool)), this, SLOT(jointR1Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowR2, SIGNAL(clicked(bool)), this, SLOT(jointR2Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowR3, SIGNAL(clicked(bool)), this, SLOT(jointR3Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowR4, SIGNAL(clicked(bool)), this, SLOT(jointR4Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowR5, SIGNAL(clicked(bool)), this, SLOT(jointR5Clicked()), Qt::UniqueConnection);
    QObject::connect(jointRowR6, SIGNAL(clicked(bool)), this, SLOT(jointR6Clicked()), Qt::UniqueConnection);
    // 关节复位槽函数
    QObject::connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButtonClicked()), Qt::UniqueConnection);
    QObject::connect(&updateInterfaceTimer, SIGNAL(timeout()), this, SLOT(updateInterface()));
}

//按钮初始化
void MainWindow::buttonInitialize()
{
    maxBtn = new QPushButton();
    minBtn = new QPushButton();
    closeBtn = new QPushButton();

    minBtn->setFixedSize(40, 40);
    minBtn->setFlat(true);
    minBtn->setIconSize(QSize(40, 40));
    minBtn->setIcon(QIcon(QPixmap(":/image/minmize.png")));

    maxBtn->setFixedSize(40, 40);
    maxBtn->setFlat(true);
    maxBtn->setIconSize(QSize(40, 40));
    maxBtn->setIcon(QIcon(QPixmap(":/image/maxmize.png")));

    closeBtn->setFixedSize(40, 40);
    closeBtn->setFlat(true);
    closeBtn->setIconSize(QSize(40, 40));
    closeBtn->setIcon(QIcon(QPixmap(":/image/close.png")));

    operationBtn = new QPushButton();
    stopBtn = new QPushButton();

    operationBtn->setFixedSize(100,50);
    operationBtn->setIconSize(QSize(30, 30));
    operationBtn->setIcon(QIcon(QPixmap(":/image/start.png")));
    operationBtn->setText(tr("执 行"));
    operationBtn->setFont(QFont("幼圆",12));
    operationBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    operationBtn->setDefault(1);

    stopBtn->setFixedSize(100,50);
    stopBtn->setIconSize(QSize(30, 30));
    stopBtn->setIcon(QIcon(QPixmap(":/image/stop.png")));
    stopBtn->setText(tr("停 止"));
    stopBtn->setFont(QFont("幼圆",12));
    stopBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    stopBtn->setDefault(1);


    managerBtn = new QPushButton();
    communicationBtn = new QPushButton();
    userInterfaceBtn = new QPushButton();
    systemParaBtn = new QPushButton();
    historyRecordBtn = new QPushButton();
    aboutBtn = new QPushButton();

    managerBtn->setFixedHeight(M_B_H);
    communicationBtn->setFixedHeight(M_B_H);
    userInterfaceBtn->setFixedHeight(M_B_H);
    systemParaBtn->setFixedHeight(M_B_H);
    historyRecordBtn->setFixedHeight(M_B_H);
    aboutBtn->setFixedHeight(M_B_H);

    managerBtn->setText(tr("密码管理"));
    communicationBtn->setText(tr("通信设置"));
    userInterfaceBtn->setText(tr("工作界面"));
    systemParaBtn->setText(tr("系统参数"));
    historyRecordBtn->setText(tr("历史记录"));
    aboutBtn->setText(tr("操作流程"));

    QFont buttonFont("幼圆", 16);
    managerBtn->setFont(buttonFont);
    communicationBtn->setFont(buttonFont);
    userInterfaceBtn->setFont(buttonFont);
    systemParaBtn->setFont(buttonFont);
    historyRecordBtn->setFont(buttonFont);
    aboutBtn->setFont(buttonFont);

    /*
    managerBtn->setDefault(1);
    communicationBtn->setDefault(1);
    userInterfaceBtn->setDefault(1);
    systemParaBtn->setDefault(1);
    historyRecordBtn->setDefault(1);
    aboutBtn->setDefault(1);
    */

    //设置菜单栏按键样式
    managerBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");
    communicationBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");
    userInterfaceBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");
    systemParaBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");
    historyRecordBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");
    aboutBtn->setStyleSheet("QPushButton{background-color: #79a4ff; border-radius:6px; color:#000000; border:2px solid grey;} QPushButton:pressed{background-color:#5ea7ff; border-style:inset;}");

    //设置菜单栏按键鼠标样式
    managerBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    communicationBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    userInterfaceBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    systemParaBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    historyRecordBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    aboutBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

    resetButton = new QPushButton();
    QFont resetFont("幼圆", 12);
    resetButton->setFont(resetFont);
    resetButton->setIconSize(QSize(25, 25));
    resetButton->setIcon(QIcon(QPixmap(":/image/reset.png")));
    resetButton->setFixedSize(110,40);
    resetButton->setText(tr(" 复 位"));
    resetButton->setDefault(1);
    resetButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
}

//原形进度条控件初始化
void MainWindow::roundProgressBarInit()
{
    wheelBar1 = new QRoundProgressBar();
    wheelBar2 = new QRoundProgressBar();
    wheelBar3 = new QRoundProgressBar();
    wheelBar4 = new QRoundProgressBar();

    //wheelBar1->setFixedSize(80,80);
    wheelBar1->setFormat("%v");
    wheelBar1->setDecimals(0);
    wheelBar1->setRange(0, 100);
    wheelBar1->setValue(25);

    wheelBar2->setFormat("%v");
    wheelBar2->setDecimals(0);
    wheelBar2->setRange(0, 100);
    wheelBar2->setValue(25);

    wheelBar3->setFormat("%v");
    wheelBar3->setDecimals(0);
    wheelBar3->setRange(0, 100);
    wheelBar3->setValue(25);

    wheelBar4->setFormat("%v");
    wheelBar4->setDecimals(0);
    wheelBar4->setRange(0, 100);
    wheelBar4->setValue(25);

    wheelBar4->setStyleSheet("QRoundProgressBar{Shadow: #22b9ff;}");


/*
    QPalette p;
    p.setBrush(QPalette::Background,Qt::NoBrush);
    //p.setBrush(QPalette::Base,Qt::NoBrush);
    //p.setColor(QPalette::Text, Qt::black);
    //p.setColor(QPalette::Shadow, Qt::blue);

    wheelBar1->setPalette(p);
    wheelBar2->setPalette(p);
    wheelBar3->setPalette(p);
    wheelBar4->setPalette(p);
*/

}

//主界面初始化
void MainWindow::MainWindowInit()
{
   menuStackedWidget = new QStackedWidget(this);
   menuStackedWidget->setFixedSize(0.15*WIN_W, WIN_H);
   //menuStackedWidget->setFrameShape(QFrame::Shape::StyledPanel);
   menuPageWidget = new QWidget(this);
   menuPageWidget->setFixedSize(0.15*WIN_W, WIN_H);
   menuPageInitialize();
   menuStackedWidget->insertWidget(0, menuPageWidget);

   stackedWidget = new QStackedWidget(this);
   //stackedWidget->setFrameShape(QFrame::Shape::StyledPanel);
   stackedWidget->setFixedSize(0.85*WIN_W, WIN_H);
   firstPageWidget = new QWidget(this);
   firstPageWidget->setFixedSize(0.85*WIN_W, WIN_H);
   secondPageWidget = new QWidget(this);
   secondPageWidget->setFixedSize(0.85*WIN_W, WIN_H);
   thirdPageWidget = new QWidget(this);
   thirdPageWidget->setFixedSize(0.85*WIN_W, WIN_H);
   fourthPageWidget = new QWidget(this);
   fourthPageWidget->setFixedSize(0.85*WIN_W, WIN_H);
   fifthPageWidget = new QWidget(this);
   fifthPageWidget->setFixedSize(0.85*WIN_W, WIN_H);
   sixthPageWidget = new QWidget(this);
   sixthPageWidget->setFixedSize(0.85*WIN_W, WIN_H);

   firstPageInitialize();
   stackedWidget->insertWidget(0, firstPageWidget);
   secondPageInitialize();
   stackedWidget->insertWidget(1, secondPageWidget);
   thirdPageInitialize();
   stackedWidget->insertWidget(2, thirdPageWidget);
   fourthPageInitialize();
   stackedWidget->insertWidget(3, fourthPageWidget);
   fifthPageInitialize();
   stackedWidget->insertWidget(4, fifthPageWidget);
   sixthPageInitialize();
   stackedWidget->insertWidget(5, sixthPageWidget);

   QHBoxLayout *mainWindowLayout = new QHBoxLayout();
   mainWindowLayout->addWidget(menuStackedWidget);
   mainWindowLayout->addWidget(stackedWidget);
   centralWidget()->setLayout(mainWindowLayout);

   stackedWidget->setCurrentIndex(0);
}

//菜单栏界面初始化
void MainWindow::menuPageInitialize()
{
    QVBoxLayout *menuLayout = new QVBoxLayout();
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    menuGroupBox = new QGroupBox();
    menuGroupBox ->setTitle(tr("菜单栏:"));
    stateMessageBox = new QGroupBox();
    stateMessageBox ->setTitle(tr("状态信息:"));


    buttonLayout->addWidget(communicationBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(systemParaBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(userInterfaceBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(historyRecordBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(managerBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(aboutBtn);
    menuGroupBox->setLayout(buttonLayout);

    menuLayout->addWidget(menuGroupBox);
    menuLayout->addSpacing(10);
    menuLayout->addWidget(stateMessageBox);

    menuPageWidget->setLayout(menuLayout);
}

//分界面1初始化
 void MainWindow::firstPageInitialize()
 {
     QVBoxLayout *firstPageLayout = new QVBoxLayout();

     //标题
     communicationTitleLabel = new QLabel();
     QFont titleFont("华文新魏", 33);
     communicationTitleLabel->setStyleSheet("QLabel{color: #3a96d9;}");
     communicationTitleLabel->setAlignment(Qt::AlignHCenter);
     communicationTitleLabel->setFont(titleFont);
     communicationTitleLabel->setText(tr("通 信 参 数"));


     //边框
     CommunicationGroupBox = new QGroupBox();
     CommunicationGroupBox->setStyleSheet("QGroupBox{border: 3.5px solid grey; border-radius:6px; background-color: rgba(255, 255, 255, 0);}");
     CommunicationGroupBox->setFixedSize(800, 480);
     QVBoxLayout *CommunicationGroupBoxLayout= new QVBoxLayout();
     CommunicationGroupBoxLayout->setAlignment(Qt::AlignHCenter);

     QFont lineEditFont("幼圆", 10);
     QFont labelFont("幼圆", 10);
     QFont buttonFont("幼圆", 13);
     //kuka六自由度机械臂通信
     ipLabel = new QLabel();
     ipLabel->setText(tr("IP地址:"));
     ipLabel->setFont(labelFont);
     ipLineEdit = new QLineEdit();
     ipLineEdit->setText("192.168.1.24");
     ipLineEdit->setFont(lineEditFont);
     QHBoxLayout *ipLayout = new QHBoxLayout();
     ipLayout->addSpacing(150);
     ipLayout->addWidget(ipLabel);
     ipLayout->addSpacing(5);
     ipLayout->addWidget(ipLineEdit);
     ipLayout->addSpacing(150);


     portLabel = new QLabel();
     portLabel->setText(tr("端口号:"));
     portLabel->setFont(labelFont);
     portLineEdit = new QLineEdit();
     portLineEdit->setText("6666");
     portLineEdit->setFont(lineEditFont);

     QHBoxLayout *portLayout = new QHBoxLayout();
     portLayout->addSpacing(150);
     portLayout->addWidget(portLabel);
     portLayout->addSpacing(5);
     portLayout->addWidget(portLineEdit);
     portLayout->addSpacing(150);

     connectBtn = new QPushButton();
     connectBtn->setText(tr("连 接"));
     connectBtn->setFont(buttonFont);
     connectBtn->setFixedSize(120, 35);

     resetBtn = new QPushButton();
     resetBtn->setText(tr("重 置"));
     resetBtn->setFont(buttonFont);
     resetBtn->setFixedSize(120, 35);

     kukaGroupBox = new QGroupBox();
     kukaGroupBox->setTitle(tr("六自由度机械臂"));
     kukaGroupBox->setFixedSize(700, 130);
     kukaGroupBox->setStyleSheet("QGroupBox{border: 1px solid #ffffff; border-radius:3px; background-color: rgba(255, 255, 255, 0);}");
     QVBoxLayout *kukaGroupBoxLayout = new QVBoxLayout();
     kukaGroupBoxLayout->addLayout(ipLayout);
     kukaGroupBoxLayout->addLayout(portLayout);
     kukaGroupBoxLayout->addSpacing(8);
     kukaGroupBox->setLayout(kukaGroupBoxLayout);


     //AGV通信参数
     agvGroupBox = new QGroupBox();
     agvGroupBox->setTitle(tr("全向车"));
     agvGroupBox->setFixedSize(700, 160);
     agvGroupBox->setStyleSheet("QGroupBox{border: 1px solid #ffffff; border-radius:3px; background-color: rgba(255, 255, 255, 0);}");

     serialPortLabel = new QLabel(tr("端口号:"));
     buadRateLabel = new QLabel(tr("波特率:"));
     dataBitsLabel = new QLabel(tr("数据位:"));
     stopBitsLabel = new QLabel(tr("停止位:"));
     parityLabel = new QLabel(tr("校验位:"));

     serialPortLabel->setFont(labelFont);
     buadRateLabel->setFont(labelFont);
     dataBitsLabel->setFont(labelFont);
     stopBitsLabel->setFont(labelFont);
     parityLabel->setFont(labelFont);

     serialPortComboBox = new QComboBox();
     buadRateComboBox = new QComboBox();
     dataBitsComboBox = new QComboBox();
     stopBitsComboBox = new QComboBox();
     parityComboBox = new QComboBox();

     serialPortComboBox->setFont(lineEditFont);
     buadRateComboBox->setFont(lineEditFont);
     dataBitsComboBox->setFont(lineEditFont);
     stopBitsComboBox->setFont(lineEditFont);
     parityComboBox->setFont(lineEditFont);

     portList = QSerialPortInfo::availablePorts();
     for(int i=0; i<portList.size(); i++)
     {
        serialPortComboBox->addItem(portList.at(i).portName());
     }

     QStringList buadRateList;
     buadRateList << QString(tr("1200"))
                  << QString(tr("2400"))
                  << QString(tr("4800"))
                  << QString(tr("9600"))
                  << QString(tr("19200"))
                  << QString(tr("38400"))
                  << QString(tr("56000"))
                  << QString(tr("57600"))
                  << QString(tr("115200"))
                  << QString(tr("128000"))
                  << QString(tr("256000"));
     buadRateComboBox->addItems(buadRateList);

     QStringList dataBitsList;
     dataBitsList << QString(tr("5"))
                  << QString(tr("6"))
                  << QString(tr("7"))
                  << QString(tr("8"));
     dataBitsComboBox->addItems(dataBitsList);

     QStringList stopBitsList;
     stopBitsList << QString(tr("NONE"))
                  << QString(tr("ODD"))
                  << QString(tr("EVEN"))
                  << QString(tr("MARK"))
                  << QString(tr("SPACE"));
     stopBitsComboBox->addItems(stopBitsList);

     QStringList parityList;
     parityList << QString(tr("1"))
                  << QString(tr("1.5"))
                  << QString(tr("2"));
     parityComboBox->addItems(parityList);

     serialPortComboBox->setFixedWidth(130);
     buadRateComboBox->setFixedWidth(130);
     dataBitsComboBox->setFixedWidth(130);
     stopBitsComboBox->setFixedWidth(130);
     parityComboBox->setFixedWidth(130);

     serialPortComboBox->setCurrentIndex(0);
     buadRateComboBox->setCurrentIndex(3);
     dataBitsComboBox->setCurrentIndex(3);
     stopBitsComboBox->setCurrentIndex(2);
     parityComboBox->setCurrentIndex(0);

     QHBoxLayout *serialPortLayout = new QHBoxLayout();
     serialPortLayout->addSpacing(150);
     serialPortLayout->addWidget(serialPortLabel);
     serialPortLayout->addSpacing(5);
     serialPortLayout->addWidget(serialPortComboBox);
     serialPortLayout->addSpacing(150);
     QHBoxLayout *buadRateLayout = new QHBoxLayout();
     buadRateLayout->addSpacing(150);
     buadRateLayout->addWidget(buadRateLabel);
     buadRateLayout->addSpacing(5);
     buadRateLayout->addWidget(buadRateComboBox);
     buadRateLayout->addSpacing(150);
     QHBoxLayout *dataBitsLayout = new QHBoxLayout();
     dataBitsLayout->addSpacing(150);
     dataBitsLayout->addWidget(dataBitsLabel);
     dataBitsLayout->addSpacing(5);
     dataBitsLayout->addWidget(dataBitsComboBox);
     dataBitsLayout->addSpacing(150);
     QHBoxLayout *stopBitsLayout = new QHBoxLayout();
     stopBitsLayout->addSpacing(150);
     stopBitsLayout->addWidget(stopBitsLabel);
     stopBitsLayout->addSpacing(5);
     stopBitsLayout->addWidget(stopBitsComboBox);
     stopBitsLayout->addSpacing(150);
     QHBoxLayout *parityLayout = new QHBoxLayout();
     parityLayout->addSpacing(150);
     parityLayout->addWidget(parityLabel);
     parityLayout->addSpacing(5);
     parityLayout->addWidget(parityComboBox);
     parityLayout->addSpacing(150);

     QVBoxLayout *agvLayout_V1 = new QVBoxLayout();
     agvLayout_V1->addLayout(serialPortLayout);
     agvLayout_V1->addLayout(buadRateLayout);
     agvLayout_V1->addLayout(dataBitsLayout);
     QVBoxLayout *agvLayout_V2 = new QVBoxLayout();
     agvLayout_V2->addLayout(stopBitsLayout);
     agvLayout_V2->addLayout(parityLayout);

     QHBoxLayout *agvLayout = new QHBoxLayout();
     agvLayout->addSpacing(20);
     agvLayout->addLayout(agvLayout_V1);
     agvLayout->addLayout(agvLayout_V2);
     agvLayout->addSpacing(80);

     agvGroupBox->setLayout(agvLayout);

     QHBoxLayout *kukaBtnLayout = new QHBoxLayout();
     kukaBtnLayout->addSpacing(140);
     kukaBtnLayout->addWidget(connectBtn);
     kukaBtnLayout->addWidget(resetBtn);
     kukaBtnLayout->addSpacing(140);

     CommunicationGroupBoxLayout->addWidget(kukaGroupBox);
     CommunicationGroupBoxLayout->addSpacing(30);
     CommunicationGroupBoxLayout->addWidget(agvGroupBox);
     CommunicationGroupBoxLayout->addSpacing(30);
     CommunicationGroupBoxLayout->addLayout(kukaBtnLayout);
     CommunicationGroupBox->setLayout(CommunicationGroupBoxLayout);


     firstPageLayout->setAlignment(Qt::AlignHCenter);
     firstPageLayout->addSpacing(65);
     firstPageLayout->addWidget(communicationTitleLabel);
     firstPageLayout->addSpacing(5);
     firstPageLayout->addWidget(CommunicationGroupBox);
     firstPageLayout->addSpacing(100);
     firstPageWidget->setLayout(firstPageLayout);;
 }
 
//分界面2初始化
 void MainWindow::secondPageInitialize()
 {
     QGridLayout *secondPageLayout = new QGridLayout();
     secondPageWidget->setLayout(secondPageLayout);
 }
 
//分界面3初始化
 void MainWindow::thirdPageInitialize()
 {
     QHBoxLayout *thirdPageLayout = new QHBoxLayout(); 
     QVBoxLayout *robotLayout = new QVBoxLayout();
     QVBoxLayout *jointLayout = new QVBoxLayout();
     QHBoxLayout *toolPosLayout = new QHBoxLayout();

     QVBoxLayout *agvLayout = new QVBoxLayout();
     QHBoxLayout *roundBarLayout = new QHBoxLayout();
     QHBoxLayout *labelLayout = new QHBoxLayout();

     QVBoxLayout *operationLayout = new QVBoxLayout();



     jointGroupBox = new QGroupBox();
     jointGroupBox->setTitle("移动关节:");
     jointGroupBox->setFixedHeight(0.54*WIN_H);
     toolPosGroupBox = new QGroupBox();
     toolPosGroupBox->setTitle(tr("工具位置:"));
     agvPosGroupBox = new QGroupBox();
     agvPosGroupBox->setTitle("移动平台:");
     agvPosGroupBox->setFixedWidth(0.37*WIN_W);


     //移动平台界面布局
     //速度曲线图初始化
     qwtPlot = new QwtPlot();
     qwtPlot->setTitle(tr("移动平台速度波形图"));
     qwtPlot->setFixedHeight(0.48*WIN_H);
     QFont plotFont("幼圆", 15);
     qwtPlot->setFont(plotFont);
     QFont axisFont("幼圆", 10);
     qwtPlot->setAxisFont(0, axisFont);
     qwtPlot->setAxisFont(2, axisFont);
     qwtPlot->setAxisAutoScale(0, true);
     qwtPlot->setAxisAutoScale(2, true);
     qwtPlot->setAxisScale(0, -2, 2);
     qwtPlot->setCanvasBackground(QColor(209, 211, 211));

     //qwtPlot->setAxisTitle(0, "velcity/rpm");

     qwtPlot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
     QwtPlotGrid *grid = new QwtPlotGrid();
     grid->setMajorPen(QPen(Qt::gray, 0, Qt::DashDotLine));
     grid->setMinorPen(QPen(Qt::gray, 1, Qt::DotLine));
     grid->attach(qwtPlot);

     //曲线图表数据显示部分

     for(int i=1;i<5001;i++)
     {
         xdata.append(double(i)/1000-5);
         ydata.append(0);
     }

     curve = new QwtPlotCurve();
     curve->setTitle("m1");
     curve->setLegendIconSize(QSize(12,10));
     curve->setPen(Qt::black,1);

     //速度表盘初始化
     QFont labelFont("幼圆", 11);
     wheelLabel1.setText(tr("轮1速度"));
     wheelLabel2.setText(tr("轮2速度"));
     wheelLabel3.setText(tr("轮3速度"));
     wheelLabel4.setText(tr("轮4速度"));
     wheelLabel1.setAlignment(Qt::AlignHCenter);
     wheelLabel2.setAlignment(Qt::AlignHCenter);
     wheelLabel3.setAlignment(Qt::AlignHCenter);
     wheelLabel4.setAlignment(Qt::AlignHCenter);
     wheelLabel1.setFont(labelFont);
     wheelLabel2.setFont(labelFont);
     wheelLabel3.setFont(labelFont);
     wheelLabel4.setFont(labelFont);

     roundProgressBarInit();
     roundBarLayout->addSpacing(20);
     roundBarLayout->addWidget(wheelBar1);
     roundBarLayout->addSpacing(20);
     roundBarLayout->addWidget(wheelBar2);
     roundBarLayout->addSpacing(20);
     roundBarLayout->addWidget(wheelBar3);
     roundBarLayout->addSpacing(20);
     roundBarLayout->addWidget(wheelBar4);
     roundBarLayout->addSpacing(20);

     labelLayout->addWidget(&wheelLabel1);
     labelLayout->addWidget(&wheelLabel2);
     labelLayout->addWidget(&wheelLabel3);
     labelLayout->addWidget(&wheelLabel4);

     agvLayout->addWidget(qwtPlot);
     agvLayout->addSpacing(10);
     agvLayout->addLayout(roundBarLayout);
     agvLayout->addLayout(labelLayout);
     agvPosGroupBox->setLayout(agvLayout);

//模式选择面板布局
    modelGroupBox = new QGroupBox();
    modelGroupBox->setTitle(tr("模式选择"));

    QHBoxLayout *modelLayout = new QHBoxLayout();
    QFont radioFont("幼圆", 10);
    pointToPointRadioBtn = new QRadioButton();
    straightLineRadioBtn = new QRadioButton();
    circularPathRadioBtn = new QRadioButton();
    demo1RadioBtn = new QRadioButton();
    demo2RadioBtn = new QRadioButton();

    pointToPointRadioBtn->setText(tr("点动"));
    straightLineRadioBtn->setText(tr("直线插补"));
    circularPathRadioBtn->setText(tr("圆弧插补"));
    demo1RadioBtn->setText(tr("演示1"));
    demo2RadioBtn->setText(tr("演示2"));

    // 模式选择初始化
    pointToPointRadioBtn->setChecked(true);
    ms = (Model)2;

    pointToPointRadioBtn->setFont(radioFont);
    straightLineRadioBtn->setFont(radioFont);
    circularPathRadioBtn->setFont(radioFont);
    demo1RadioBtn->setFont(radioFont);
    demo2RadioBtn->setFont(radioFont);

    pointToPointRadioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    straightLineRadioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    circularPathRadioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    demo1RadioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    demo2RadioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

    modelLayout->addSpacing(30);
    modelLayout->addWidget(straightLineRadioBtn);
    modelLayout->addWidget(circularPathRadioBtn);
    modelLayout->addWidget(pointToPointRadioBtn);
    modelLayout->addWidget(demo1RadioBtn);
    modelLayout->addWidget(demo2RadioBtn);

    modelGroupBox->setLayout(modelLayout);

//速度设定面板布局
    speedSetGroupBox = new QGroupBox();
    speedSetGroupBox->setTitle(tr("速度设定"));

    velocityLineEdit = new QLineEdit();
    velocityLineEdit->setFixedWidth(0.07*WIN_W);
    velocityLineEdit->setReadOnly(true);
    QFont velcityFont("幼圆", 10);
    velocityLineEdit->setFont(velcityFont);
    velocityLineEdit->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
    velocityLineEdit->setAlignment(Qt::AlignRight);
    velocityLineEdit->setText("0.00");

    veloctyUnitLabel = new QLabel();
    veloctyUnitLabel->setText("mm/s");
    veloctyUnitLabel->setFont(velcityFont);
    velocitySlider = new QSlider();
    velocitySlider->setOrientation(Qt::Horizontal);
    velocitySlider->setFixedWidth(0.19*WIN_W);
    velocitySlider->setTracking(true);

    QVBoxLayout *speedLayout = new QVBoxLayout();
    QHBoxLayout *speedDisLayout = new QHBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *speedGroupLayout = new QHBoxLayout();

    speedDisLayout->setAlignment(Qt::AlignHCenter);
    speedDisLayout->addWidget(velocityLineEdit);
    speedDisLayout->addWidget(veloctyUnitLabel);

    speedLayout->addSpacing(10);
    speedLayout->addLayout(speedDisLayout);
    speedLayout->addSpacing(10);
    speedLayout->addWidget(velocitySlider);
    speedLayout->addSpacing(10);

    buttonLayout->addWidget(operationBtn);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(stopBtn);

    speedGroupLayout->addLayout(speedLayout);
    speedGroupLayout->addLayout(buttonLayout);

    speedSetGroupBox->setLayout(speedGroupLayout);

    operationLayout->addWidget(modelGroupBox);
    operationLayout->addSpacing(10);
    operationLayout->addWidget(speedSetGroupBox);

//关节位置界面布局
     jointBar1 = new QProgressBar();
     jointBar1->setFixedSize(323,21);
     jointBar2 = new QProgressBar();
     jointBar2->setFixedSize(323,21);
     jointBar3 = new QProgressBar();
     jointBar3->setFixedSize(323,21);
     jointBar4 = new QProgressBar();
     jointBar4->setFixedSize(323,21);
     jointBar5 = new QProgressBar();
     jointBar5->setFixedSize(323,21);
     jointBar6 = new QProgressBar();
     jointBar6->setFixedSize(323,21);

     jointBar1->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");
     jointBar2->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");
     jointBar3->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");
     jointBar4->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");
     jointBar5->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");
     jointBar6->setStyleSheet("QProgressBar::chunk{background-color: #05B8CC;} QProgressBar{border:2px solid grey; border-radius:5px; background-color: #FFFFFF;}");


     jointBar1->setRange(-100, 100);
     jointBar2->setRange(-100, 100);
     jointBar3->setRange(-100, 100);
     jointBar4->setRange(-100, 100);
     jointBar5->setRange(-100, 100);
     jointBar6->setRange(-100, 100);

     jointBar1->setValue(0);
     jointBar2->setValue(0);
     jointBar3->setValue(0);
     jointBar4->setValue(0);
     jointBar5->setValue(0);
     jointBar6->setValue(0);

     jointBar1->setTextVisible(false);
     jointBar2->setTextVisible(false);
     jointBar3->setTextVisible(false);
     jointBar4->setTextVisible(false);
     jointBar5->setTextVisible(false);
     jointBar6->setTextVisible(false);

     jointLabel1 = new QLabel();
     jointLabel2 = new QLabel();
     jointLabel3 = new QLabel();
     jointLabel4 = new QLabel();
     jointLabel5 = new QLabel();
     jointLabel6 = new QLabel();
     jointLabel1->setText(tr("基 座"));
     jointLabel2->setText(tr("肩 部"));
     jointLabel3->setText(tr("肘 部"));
     jointLabel4->setText(tr("手腕1"));
     jointLabel5->setText(tr("手腕2"));
     jointLabel6->setText(tr("手腕3"));
     QFont jointLabelFont("幼圆", 11);
     jointLabel1->setFont(jointLabelFont);
     jointLabel2->setFont(jointLabelFont);
     jointLabel3->setFont(jointLabelFont);
     jointLabel4->setFont(jointLabelFont);
     jointLabel5->setFont(jointLabelFont);
     jointLabel6->setFont(jointLabelFont);

     jointRowL1 = new QPushButton();
     jointRowL2 = new QPushButton();
     jointRowL3 = new QPushButton();
     jointRowL4 = new QPushButton();
     jointRowL5 = new QPushButton();
     jointRowL6 = new QPushButton();
     jointRowR1 = new QPushButton();
     jointRowR2 = new QPushButton();
     jointRowR3 = new QPushButton();
     jointRowR4 = new QPushButton();
     jointRowR5 = new QPushButton();
     jointRowR6 = new QPushButton();

     jointRowL1->setFixedSize(60, 40);
     jointRowL1->setFlat(true);
     jointRowL1->setIconSize(QSize(50, 50));
     jointRowL1->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL1->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowL2->setFixedSize(60, 40);
     jointRowL2->setFlat(true);
     jointRowL2->setIconSize(QSize(50, 50));
     jointRowL2->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowL3->setFixedSize(60, 40);
     jointRowL3->setFlat(true);
     jointRowL3->setIconSize(QSize(50, 50));
     jointRowL3->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowL4->setFixedSize(60, 40);
     jointRowL4->setFlat(true);
     jointRowL4->setIconSize(QSize(50, 50));
     jointRowL4->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowL5->setFixedSize(60, 40);
     jointRowL5->setFlat(true);
     jointRowL5->setIconSize(QSize(50, 50));
     jointRowL5->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowL6->setFixedSize(60, 40);
     jointRowL6->setFlat(true);
     jointRowL6->setIconSize(QSize(50, 50));
     jointRowL6->setIcon(QIcon(QPixmap(":/image/arrow-left.png")));
     jointRowL6->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR1->setFixedSize(60, 40);
     jointRowR1->setFlat(true);
     jointRowR1->setIconSize(QSize(50, 50));
     jointRowR1->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR1->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR2->setFixedSize(60, 40);
     jointRowR2->setFlat(true);
     jointRowR2->setIconSize(QSize(50, 50));
     jointRowR2->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR3->setFixedSize(60, 40);
     jointRowR3->setFlat(true);
     jointRowR3->setIconSize(QSize(50, 50));
     jointRowR3->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR4->setFixedSize(60, 40);
     jointRowR4->setFlat(true);
     jointRowR4->setIconSize(QSize(50, 50));
     jointRowR4->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR5->setFixedSize(60, 40);
     jointRowR5->setFlat(true);
     jointRowR5->setIconSize(QSize(50, 50));
     jointRowR5->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointRowR6->setFixedSize(60, 40);
     jointRowR6->setFlat(true);
     jointRowR6->setIconSize(QSize(50, 50));
     jointRowR6->setIcon(QIcon(QPixmap(":/image/arrow-right.png")));
     jointRowR6->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

     jointLine1 = new QLineEdit();
     jointLine2 = new QLineEdit();
     jointLine3 = new QLineEdit();
     jointLine4 = new QLineEdit();
     jointLine5 = new QLineEdit();
     jointLine6 = new QLineEdit();

     jointLine1->setFixedWidth(68);
     jointLine2->setFixedWidth(68);
     jointLine3->setFixedWidth(68);
     jointLine4->setFixedWidth(68);
     jointLine5->setFixedWidth(68);
     jointLine6->setFixedWidth(68);

     /*
     jointLine1->setReadOnly(true);
     jointLine2->setReadOnly(true);
     jointLine3->setReadOnly(true);
     jointLine4->setReadOnly(true);
     jointLine5->setReadOnly(true);
     jointLine6->setReadOnly(true);
     */

     jointLine1->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");
     jointLine2->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");
     jointLine3->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");
     jointLine4->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");
     jointLine5->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");
     jointLine6->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px;}");

     jointLine1->setAlignment(Qt::AlignRight);
     jointLine2->setAlignment(Qt::AlignRight);
     jointLine3->setAlignment(Qt::AlignRight);
     jointLine4->setAlignment(Qt::AlignRight);
     jointLine5->setAlignment(Qt::AlignRight);
     jointLine6->setAlignment(Qt::AlignRight);

     QFont lineEditFont("幼圆", 10);

     jointLine1->setFont(lineEditFont);
     jointLine2->setFont(lineEditFont);
     jointLine3->setFont(lineEditFont);
     jointLine4->setFont(lineEditFont);
     jointLine5->setFont(lineEditFont);
     jointLine6->setFont(lineEditFont);

     jointLine1->setText("0.00");
     jointLine2->setText("0.00");
     jointLine3->setText("0.00");
     jointLine4->setText("0.00");
     jointLine5->setText("0.00");
     jointLine6->setText("0.00");

     unitLabel1 = new QLabel();
     unitLabel2 = new QLabel();
     unitLabel3 = new QLabel();
     unitLabel4 = new QLabel();
     unitLabel5 = new QLabel();
     unitLabel6 = new QLabel();
     unitLabel1->setText(tr("。"));
     unitLabel2->setText(tr("。"));
     unitLabel3->setText(tr("。"));
     unitLabel4->setText(tr("。"));
     unitLabel5->setText(tr("。"));
     unitLabel6->setText(tr("。"));
     QFont unitLabelFont("幼圆", 11);
     unitLabel1->setFont(unitLabelFont);
     unitLabel2->setFont(unitLabelFont);
     unitLabel3->setFont(unitLabelFont);
     unitLabel4->setFont(unitLabelFont);
     unitLabel5->setFont(unitLabelFont);
     unitLabel6->setFont(unitLabelFont);

     QHBoxLayout *jointTempLayout1 = new QHBoxLayout();
     QHBoxLayout *jointTempLayout2 = new QHBoxLayout();
     QHBoxLayout *jointTempLayout3 = new QHBoxLayout();
     QHBoxLayout *jointTempLayout4 = new QHBoxLayout();
     QHBoxLayout *jointTempLayout5 = new QHBoxLayout();
     QHBoxLayout *jointTempLayout6 = new QHBoxLayout();

     QHBoxLayout *resetButtonLayout = new QHBoxLayout();
     QHBoxLayout *jointLayout1 = new QHBoxLayout();
     QHBoxLayout *jointLayout2 = new QHBoxLayout();
     QHBoxLayout *jointLayout3 = new QHBoxLayout();
     QHBoxLayout *jointLayout4 = new QHBoxLayout();
     QHBoxLayout *jointLayout5 = new QHBoxLayout();
     QHBoxLayout *jointLayout6 = new QHBoxLayout();

     jointTempLayout1->addWidget(jointLabel1);
     jointTempLayout1->addWidget(jointRowL1);
     jointTempLayout1->addWidget(jointBar1);
     jointTempLayout1->addWidget(jointRowR1);
     jointLayout1->addLayout(jointTempLayout1);
     jointLayout1->addWidget(jointLine1);
     jointLayout1->addWidget(unitLabel1);
     jointTempLayout2->addWidget(jointLabel2);
     jointTempLayout2->addWidget(jointRowL2);
     jointTempLayout2->addWidget(jointBar2);
     jointTempLayout2->addWidget(jointRowR2);
     jointLayout2->addLayout(jointTempLayout2);
     jointLayout2->addWidget(jointLine2);
     jointLayout2->addWidget(unitLabel2);
     jointTempLayout3->addWidget(jointLabel3);
     jointTempLayout3->addWidget(jointRowL3);
     jointTempLayout3->addWidget(jointBar3);
     jointTempLayout3->addWidget(jointRowR3);
     jointLayout3->addLayout(jointTempLayout3);
     jointLayout3->addWidget(jointLine3);
     jointLayout3->addWidget(unitLabel3);
     jointTempLayout4->addWidget(jointLabel4);
     jointTempLayout4->addWidget(jointRowL4);
     jointTempLayout4->addWidget(jointBar4);
     jointTempLayout4->addWidget(jointRowR4);
     jointLayout4->addLayout(jointTempLayout4);
     jointLayout4->addWidget(jointLine4);
     jointLayout4->addWidget(unitLabel4);
     jointTempLayout5->addWidget(jointLabel5);
     jointTempLayout5->addWidget(jointRowL5);
     jointTempLayout5->addWidget(jointBar5);
     jointTempLayout5->addWidget(jointRowR5);
     jointLayout5->addLayout(jointTempLayout5);
     jointLayout5->addWidget(jointLine5);
     jointLayout5->addWidget(unitLabel5);
     jointTempLayout6->addWidget(jointLabel6);
     jointTempLayout6->addWidget(jointRowL6);
     jointTempLayout6->addWidget(jointBar6);
     jointTempLayout6->addWidget(jointRowR6);
     jointLayout6->addLayout(jointTempLayout6);
     jointLayout6->addWidget(jointLine6);
     jointLayout6->addWidget(unitLabel6);

     QSpacerItem *space = new QSpacerItem(0.5*WIN_W-120,30);
     resetButtonLayout->addItem(space);
     resetButtonLayout->addWidget(resetButton);


     //关节复位按键暂不添加
     jointLayout->addLayout(resetButtonLayout);
     jointLayout->addLayout(jointLayout1);
     jointLayout->addLayout(jointLayout2);
     jointLayout->addLayout(jointLayout3);
     jointLayout->addLayout(jointLayout4);
     jointLayout->addLayout(jointLayout5);
     jointLayout->addLayout(jointLayout6);
     jointGroupBox->setLayout(jointLayout);


 //工具位置界面布局
     toolSetPosition = new QGroupBox();
     QGridLayout *toolSetLayout = new QGridLayout();
     QGridLayout *toolDisLayout = new QGridLayout();

     //工具位置设置控件
     toolSetLabelX = new QLabel();
     toolSetLabelY = new QLabel();
     toolSetLabelZ = new QLabel();
     toolSetLabelRx = new QLabel();
     toolSetLabelRy = new QLabel();
     toolSetLabelRz = new QLabel();

     QFont toolLabelFont("幼圆", 11);

     toolSetLabelX->setText("X");
     toolSetLabelY->setText("Y");
     toolSetLabelZ->setText("Z");
     toolSetLabelRx->setText("Rx");
     toolSetLabelRy->setText("Ry");
     toolSetLabelRz->setText("Rz");

     toolSetLabelX->setFont(toolLabelFont);
     toolSetLabelY->setFont(toolLabelFont);
     toolSetLabelZ->setFont(toolLabelFont);
     toolSetLabelRx->setFont(toolLabelFont);
     toolSetLabelRy->setFont(toolLabelFont);
     toolSetLabelRz->setFont(toolLabelFont);

     toolSetSpinBoxX = new QDoubleSpinBox();
     toolSetSpinBoxY = new QDoubleSpinBox();
     toolSetSpinBoxZ = new QDoubleSpinBox();
     toolSetSpinBoxRx = new QDoubleSpinBox();
     toolSetSpinBoxRy = new QDoubleSpinBox();
     toolSetSpinBoxRz = new QDoubleSpinBox();

     toolSetSpinBoxX->setFixedWidth(70);
     toolSetSpinBoxY->setFixedWidth(70);
     toolSetSpinBoxZ->setFixedWidth(70);
     toolSetSpinBoxRx->setFixedWidth(70);
     toolSetSpinBoxRy->setFixedWidth(70);
     toolSetSpinBoxRz->setFixedWidth(70);

     /*
     toolSetSpinBoxX->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     toolSetSpinBoxY->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     toolSetSpinBoxZ->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     toolSetSpinBoxRx->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     toolSetSpinBoxRy->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     toolSetSpinBoxRz->setStyleSheet("QDoubleSpinBox{border: 0px solid grey; border-radius:1px;}");
     */

     toolSetSpinBoxX->setAlignment(Qt::AlignRight);
     toolSetSpinBoxY->setAlignment(Qt::AlignRight);
     toolSetSpinBoxZ->setAlignment(Qt::AlignRight);
     toolSetSpinBoxRx->setAlignment(Qt::AlignRight);
     toolSetSpinBoxRy->setAlignment(Qt::AlignRight);
     toolSetSpinBoxRz->setAlignment(Qt::AlignRight);

     /*
     toolSetLineEditX->setText("0.00");
     toolSetLineEditY->setText("0.00");
     toolSetLineEditZ->setText("0.00");
     toolSetLineEditRx->setText("0.00");
     toolSetLineEditRy->setText("0.00");
     toolSetLineEditRz->setText("0.00");
     */

     toolSetUnitX = new QLabel();
     toolSetUnitY = new QLabel();
     toolSetUnitZ = new QLabel();
     toolSetUnitRx = new QLabel();
     toolSetUnitRy = new QLabel();
     toolSetUnitRz = new QLabel();

     toolSetUnitX->setText("mm");
     toolSetUnitY->setText("mm");
     toolSetUnitZ->setText("mm");
     toolSetUnitRx->setText(tr("。"));
     toolSetUnitRy->setText(tr("。"));
     toolSetUnitRz->setText(tr("。"));

     toolSetUnitX->setFont(toolLabelFont);
     toolSetUnitY->setFont(toolLabelFont);
     toolSetUnitZ->setFont(toolLabelFont);
     toolSetUnitRx->setFont(toolLabelFont);
     toolSetUnitRy->setFont(toolLabelFont);
     toolSetUnitRz->setFont(toolLabelFont);

     toolSetLayout->addWidget(toolSetLabelX, 0, 0);
     toolSetLayout->addWidget(toolSetLabelY, 1, 0);
     toolSetLayout->addWidget(toolSetLabelZ, 2, 0);
     toolSetLayout->addWidget(toolSetLabelRx, 3, 0);
     toolSetLayout->addWidget(toolSetLabelRy, 4, 0);
     toolSetLayout->addWidget(toolSetLabelRz, 5, 0);

     toolSetLayout->addWidget(toolSetSpinBoxX, 0, 1);
     toolSetLayout->addWidget(toolSetSpinBoxY, 1, 1);
     toolSetLayout->addWidget(toolSetSpinBoxZ, 2, 1);
     toolSetLayout->addWidget(toolSetSpinBoxRx, 3, 1);
     toolSetLayout->addWidget(toolSetSpinBoxRy, 4, 1);
     toolSetLayout->addWidget(toolSetSpinBoxRz, 5, 1);

     toolSetLayout->addWidget(toolSetUnitX, 0, 2);
     toolSetLayout->addWidget(toolSetUnitY, 1, 2);
     toolSetLayout->addWidget(toolSetUnitZ, 2, 2);
     toolSetLayout->addWidget(toolSetUnitRx, 3, 2);
     toolSetLayout->addWidget(toolSetUnitRy, 4, 2);
     toolSetLayout->addWidget(toolSetUnitRz, 5, 2);

     //工具位置显示控件
     toolDisLabelX = new QLabel();
     toolDisLabelY = new QLabel();
     toolDisLabelZ = new QLabel();
     toolDisLabelRx = new QLabel();
     toolDisLabelRy = new QLabel();
     toolDisLabelRz = new QLabel();

     toolDisLabelX->setText("X");
     toolDisLabelY->setText("Y");
     toolDisLabelZ->setText("Z");
     toolDisLabelRx->setText("Rx");
     toolDisLabelRy->setText("Ry");
     toolDisLabelRz->setText("Rz");

     toolDisLabelX->setFont(toolLabelFont);
     toolDisLabelY->setFont(toolLabelFont);
     toolDisLabelZ->setFont(toolLabelFont);
     toolDisLabelRx->setFont(toolLabelFont);
     toolDisLabelRy->setFont(toolLabelFont);
     toolDisLabelRz->setFont(toolLabelFont);

     toolDisLineEditX = new QLineEdit();
     toolDisLineEditY = new QLineEdit();
     toolDisLineEditZ = new QLineEdit();
     toolDisLineEditRx = new QLineEdit();
     toolDisLineEditRy = new QLineEdit();
     toolDisLineEditRz= new QLineEdit();

     toolDisLineEditX->setReadOnly(true);
     toolDisLineEditY->setReadOnly(true);
     toolDisLineEditZ->setReadOnly(true);
     toolDisLineEditRx->setReadOnly(true);
     toolDisLineEditRy->setReadOnly(true);
     toolDisLineEditRz->setReadOnly(true);

     toolDisLineEditX->setFixedWidth(65);
     toolDisLineEditY->setFixedWidth(65);
     toolDisLineEditZ->setFixedWidth(65);
     toolDisLineEditRx->setFixedWidth(65);
     toolDisLineEditRy->setFixedWidth(65);
     toolDisLineEditRz->setFixedWidth(65);

     toolDisLineEditX->setFont(lineEditFont);
     toolDisLineEditY->setFont(lineEditFont);
     toolDisLineEditZ->setFont(lineEditFont);
     toolDisLineEditRx->setFont(lineEditFont);
     toolDisLineEditRy->setFont(lineEditFont);
     toolDisLineEditRz->setFont(lineEditFont);

     toolDisLineEditX->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
     toolDisLineEditY->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
     toolDisLineEditZ->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
     toolDisLineEditRx->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
     toolDisLineEditRy->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");
     toolDisLineEditRz->setStyleSheet("QLineEdit{border: 1.3px solid grey; border-radius:1px; background-color:rgba(255,255,255,0%);}");

     //设置文本水平布局
     toolDisLineEditX->setAlignment(Qt::AlignRight);
     toolDisLineEditY->setAlignment(Qt::AlignRight);
     toolDisLineEditZ->setAlignment(Qt::AlignRight);
     toolDisLineEditRx->setAlignment(Qt::AlignRight);
     toolDisLineEditRy->setAlignment(Qt::AlignRight);
     toolDisLineEditRz->setAlignment(Qt::AlignRight);

     toolDisLineEditX->setText("0.00");
     toolDisLineEditY->setText("0.00");
     toolDisLineEditZ->setText("0.00");
     toolDisLineEditRx->setText("0.00");
     toolDisLineEditRy->setText("0.00");
     toolDisLineEditRz->setText("0.00");

     toolDisUnitX = new QLabel();
     toolDisUnitY = new QLabel();
     toolDisUnitZ = new QLabel();
     toolDisUnitRx = new QLabel();
     toolDisUnitRy = new QLabel();
     toolDisUnitRz = new QLabel();

     toolDisUnitX->setText("mm");
     toolDisUnitY->setText("mm");
     toolDisUnitZ->setText("mm");
     toolDisUnitRx->setText(tr("。"));
     toolDisUnitRy->setText(tr("。"));
     toolDisUnitRz->setText(tr("。"));

     toolDisUnitX->setFont(toolLabelFont);
     toolDisUnitY->setFont(toolLabelFont);
     toolDisUnitZ->setFont(toolLabelFont);
     toolDisUnitRx->setFont(toolLabelFont);
     toolDisUnitRy->setFont(toolLabelFont);
     toolDisUnitRz->setFont(toolLabelFont);

     toolDisLayout->addWidget(toolDisLabelX, 0, 0);
     toolDisLayout->addWidget(toolDisLabelY, 1, 0);
     toolDisLayout->addWidget(toolDisLabelZ, 2, 0);
     toolDisLayout->addWidget(toolDisLabelRx, 3, 0);
     toolDisLayout->addWidget(toolDisLabelRy, 4, 0);
     toolDisLayout->addWidget(toolDisLabelRz, 5, 0);

     toolDisLayout->addWidget(toolDisLineEditX, 0, 1);
     toolDisLayout->addWidget(toolDisLineEditY, 1, 1);
     toolDisLayout->addWidget(toolDisLineEditZ, 2, 1);
     toolDisLayout->addWidget(toolDisLineEditRx, 3, 1);
     toolDisLayout->addWidget(toolDisLineEditRy, 4, 1);
     toolDisLayout->addWidget(toolDisLineEditRz, 5, 1);

     toolDisLayout->addWidget(toolDisUnitX, 0, 2);
     toolDisLayout->addWidget(toolDisUnitY, 1, 2);
     toolDisLayout->addWidget(toolDisUnitZ, 2, 2);
     toolDisLayout->addWidget(toolDisUnitRx, 3, 2);
     toolDisLayout->addWidget(toolDisUnitRy, 4, 2);
     toolDisLayout->addWidget(toolDisUnitRz, 5, 2);

     //图片显示控件
     pictureDisplay = new QLabel();
     pictureDisplay->setFixedSize(0.26*WIN_W, 0.42*WIN_H);
     pictureDisplay->setScaledContents(true);
     pictureDisplay->setPixmap(QPixmap(":/image/kuka.jpg"));


     toolPosLayout->addLayout(toolSetLayout);
     toolPosLayout->addSpacing(5);
     toolPosLayout->addWidget(pictureDisplay);
     toolPosLayout->addSpacing(5);
     toolPosLayout->addLayout(toolDisLayout);

     toolPosGroupBox->setLayout(toolPosLayout);

     robotLayout->addWidget(toolPosGroupBox);
     robotLayout->addSpacing(12);
     robotLayout->addWidget(jointGroupBox);

     //主界面中间部分布局
     QVBoxLayout *middleLayout = new QVBoxLayout();
     middleLayout->addWidget(agvPosGroupBox);
     middleLayout->addSpacing(12);
     middleLayout->addLayout(operationLayout);

     thirdPageLayout->addLayout(middleLayout);
     thirdPageLayout->addSpacing(16);     
     thirdPageLayout->addLayout(robotLayout);
     thirdPageWidget->setLayout(thirdPageLayout);
 }

 //分界面4初始化
 void MainWindow::fourthPageInitialize()
 {
     QVBoxLayout *fourthPageLayout = new QVBoxLayout();

     QHBoxLayout *titleLayout = new QHBoxLayout();
     historyTitle = new QLabel();
     QFont titleFont("华文新魏", 33);
     historyTitle->setFont(titleFont);
     historyTitle->setStyleSheet("QLabel{color: #3a96d9;}");
     historyTitle->setText(tr("历 史 数 据"));
     historyTitle->setAlignment(Qt::AlignHCenter);

     QHBoxLayout *buttonLayout = new QHBoxLayout();

     QFont buttonFont("幼圆", 12);
     openSendBtn = new QPushButton();
     openSendBtn->setFont(buttonFont);
     openSendBtn->setText(tr("读取发送"));
     openSendBtn->setFixedWidth(100);
     openSendBtn->setFixedHeight(40);

     openReceiveBtn = new QPushButton();
     openReceiveBtn->setFont(buttonFont);
     openReceiveBtn->setText(tr("读取接收"));
     openReceiveBtn->setFixedWidth(100);
     openReceiveBtn->setFixedHeight(40);

     buttonLayout->addWidget(openSendBtn);
     buttonLayout->addWidget(openReceiveBtn);

     titleLayout->addSpacing(140);
     titleLayout->addWidget(historyTitle);
     titleLayout->addLayout(buttonLayout);
     titleLayout->addSpacing(10);

     disTableWidget = new QTableWidget();
     disTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
     disTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
     disTableWidget->setShowGrid(true);
     disTableWidget->setStyleSheet("QTableWidget{border: 3.5px solid grey; border-radius:6px; gridline-color: solid grey; background-color: rgba(255, 255, 255, 0);}");
     QFont tableFont("幼圆", 10);
     //表头样式
     disTableWidget->setColumnCount(9);
     disTableWidget->setFont(tableFont);
     QStringList tableHeader;
     tableHeader.append(tr("X"));
     tableHeader.append(tr("Y"));
     tableHeader.append(tr("Theta"));
     tableHeader.append(tr("Joint1"));
     tableHeader.append(tr("Joint2"));
     tableHeader.append(tr("Joint3"));
     tableHeader.append(tr("Joint4"));
     tableHeader.append(tr("Joint5"));
     tableHeader.append(tr("Joint6"));
     disTableWidget->setHorizontalHeaderLabels(tableHeader);
     disTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     disTableWidget->horizontalHeader()->setFixedHeight(30);
     disTableWidget->setFrameShape(QFrame::NoFrame);
     disTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border:0.5px solid grey; border-radius:1px; background: #5ea7ff;}");
     disTableWidget->verticalHeader()->setVisible(false);
     disTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

     //滚动条样式
     disTableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
     "QScrollBar::handle:hover{background:gray;}"
     "QScrollBar::sub-line{background:transparent;}"
     "QScrollBar::add-line{background:transparent;}");


     fourthPageLayout->addLayout(titleLayout);
     fourthPageLayout->addSpacing(5);
     fourthPageLayout->addWidget(disTableWidget);
     fourthPageWidget->setLayout(fourthPageLayout);
 }

 //分界面5初始化
 void MainWindow::fifthPageInitialize()
 {
     QVBoxLayout *fifthPageLayout = new QVBoxLayout();

     passWordTitle = new QLabel();
     QFont titleFont("华文新魏", 33);
     passWordTitle->setFont(titleFont);
     passWordTitle ->setText(tr(" 用 户 密 码 管 理 "));
     passWordTitle->setStyleSheet("QLabel{color: #3a96d9;}");


     //容器控件===》用于添加密码管理界面的控件
     containerBox = new QGroupBox();
     containerBox->setStyleSheet("QGroupBox{border: 3.5px solid grey; border-radius:6px; background-color: rgba(255, 255, 255, 0);}");
     containerBox->setFixedSize(550, 300);

     QVBoxLayout *containerLayout = new QVBoxLayout();
     containerBox->setLayout(containerLayout);


     QFont labelFont("幼圆", 11);
     passWordLabel = new QLabel();
     passWordLabel->setFont(labelFont);
     passWordLabel->setText(tr(("原 密 码:")));

     passWord = new QLineEdit();

     newPassWordLabel = new QLabel();
     newPassWordLabel->setFont(labelFont);
     newPassWordLabel->setText(tr("新 密 码:"));

     newPassWord = new QLineEdit();

     newPassWordAgainLabel = new QLabel();
     newPassWordAgainLabel->setFont(labelFont);
     newPassWordAgainLabel->setText(tr("重新输入:"));

     newPassWordAgain = new QLineEdit();

     QFont buttonLabel("幼圆", 11);
     passWordConfigBtn = new QPushButton();
     passWordConfigBtn->setFont(buttonLabel);
     passWordConfigBtn->setText(tr("确认修改"));
     passWordConfigBtn->setFixedSize(120, 40);

     passWordCancelBtn = new QPushButton();
     passWordCancelBtn->setFont(buttonLabel);
     passWordCancelBtn->setText(tr("返   回"));
     passWordCancelBtn->setFixedSize(120, 40);


     QHBoxLayout *passWordTitleLayout = new QHBoxLayout();
     QHBoxLayout *passWordLayout = new QHBoxLayout();
     QHBoxLayout *newPassWordLayout = new QHBoxLayout();
     QHBoxLayout *newPassWordAgainLayout = new QHBoxLayout();
     QHBoxLayout *buttonLayout = new QHBoxLayout();

     passWordTitleLayout->addSpacing(360);
     passWordTitleLayout->addWidget(passWordTitle);


     passWordLayout->addSpacing(70);
     passWordLayout->addWidget(passWordLabel);
     passWordLayout->addWidget(passWord);
     passWordLayout->addSpacing(70);


     newPassWordLayout->addSpacing(70);
     newPassWordLayout->addWidget(newPassWordLabel);
     newPassWordLayout->addWidget(newPassWord);
     newPassWordLayout->addSpacing(70);

     newPassWordAgainLayout->addSpacing(70);
     newPassWordAgainLayout->addWidget(newPassWordAgainLabel);
     newPassWordAgainLayout->addWidget(newPassWordAgain);
     newPassWordAgainLayout->addSpacing(70);

     buttonLayout->addSpacing(55);
     buttonLayout->addWidget(passWordConfigBtn);
     buttonLayout->addWidget(passWordCancelBtn);
     buttonLayout->addSpacing(55);

     containerLayout->addSpacing(50);
     containerLayout->addLayout(passWordLayout);
     containerLayout->addSpacing(12);
     containerLayout->addLayout(newPassWordLayout);
     containerLayout->addSpacing(12);
     containerLayout->addLayout(newPassWordAgainLayout);
     containerLayout->addSpacing(20);
     containerLayout->addLayout(buttonLayout);
     containerLayout->addSpacing(50);

     QHBoxLayout *tempLayout = new QHBoxLayout();
     tempLayout->addSpacing(250);
     tempLayout->addWidget(containerBox);

     fifthPageLayout->setAlignment(Qt::AlignVCenter);
     fifthPageLayout->addLayout(passWordTitleLayout);
     fifthPageLayout->addSpacing(5);
     fifthPageLayout->addLayout(tempLayout);

     fifthPageWidget->setLayout(fifthPageLayout);
 }

 //分界面6初始化
 void MainWindow::sixthPageInitialize()
 {
     QVBoxLayout *sixthPageLayout = new QVBoxLayout();

     helpTitle = new QLabel();
     QFont helpTitleFont("华文新魏", 33);
     helpTitle->setFont(helpTitleFont);
     helpTitle->setText(tr("系 统 操 作 流 程 介 绍"));
     helpTitle->setFixedWidth(900);
     helpTitle->setAlignment(Qt::AlignHCenter);
     helpTitle->setStyleSheet("QLabel{color: #3a96d9;}");

     helpTextEdit = new QTextEdit();
     helpTextEdit->setFixedSize(900, 550);
     helpTextEdit->setStyleSheet("QTextEdit{border: 2.5px solid grey; border-radius:6px; background-color: rgba(255, 255, 255, 0);}");
     helpTextEdit->setReadOnly(true);
     helpTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     helpTextEdit->append("<span style= font-size:11pt; font-weight:600;>一、 用户身份验证</span></p>");

     sixthPageLayout->addSpacing(35);
     sixthPageLayout->addWidget(helpTitle);
     sixthPageLayout->addSpacing(5);
     sixthPageLayout->addWidget(helpTextEdit);
     sixthPageLayout->addSpacing(50);
     sixthPageLayout->setAlignment(Qt::AlignVCenter);
     sixthPageLayout->setAlignment(Qt::AlignHCenter);

     sixthPageWidget->setLayout(sixthPageLayout);
 }


 //菜单栏槽函数
 void MainWindow::communicationBtn_clicked()
 {
     stackedWidget->setCurrentIndex(0);
 }

 void MainWindow::systemParaBtn_clicked()
 {
      stackedWidget->setCurrentIndex(1);
 }

 void MainWindow::userInterfaceBtn_clicked()
 {
     stackedWidget->setCurrentIndex(2);
 }

 void MainWindow::historyRecordBtn_clicked()
 {
     stackedWidget->setCurrentIndex(3);
 }


 void MainWindow::managerBtn_clicked()
 {
     stackedWidget->setCurrentIndex(4);
     passWord->clear();
     newPassWord->clear();
     newPassWordAgain->clear();
 }

 void MainWindow::aboutBtn_clicked()
 {
     stackedWidget->setCurrentIndex(5);
 }



/**********************************密码管理**********************************/
void MainWindow::passWordConfigBtn_Clicked()
{
    QSqlQuery query;
    query.exec("select * from user");
    query.next();
    if(query.value(1).toString() == passWord->text())
    {
       if(newPassWord->text()!=newPassWordAgain->text())
       {
           QMessageBox::warning(this, tr("提示信息"), tr("两次密码输入不一致！"), QMessageBox::Ok);
           return;
       }

       QSqlQuery query2;
       query2.prepare("update user set password = :password where name = :name");
       query2.bindValue(":name", "buaa");
       query2.bindValue(":password", newPassWord->text());
       if(!query2.exec())
       {
       qDebug()<<query2.lastError();
       }
       else
       {
       qDebug()<<"updated!";
       QMessageBox::warning(this, tr("提示信息"), tr("密码修改成功，请牢记新密码！！"), QMessageBox::Ok);
       }
    }
    else
    {
        QMessageBox::warning(this, tr("提示信息"), tr("原密码输入错误，请重新输入！"), QMessageBox::Ok);
        return;
    }
}

void MainWindow::passWordCancelBtn_Clicked()
{
    stackedWidget->setCurrentIndex(2);
}


/**********************************通信相关**********************************/

//kukaRobot 通信建立
void MainWindow::connectBtn_Clicked()
{
    /*
    receiveSize=0;
    tcpSocket->abort();
    ipAddress = QHostAddress(ipLineEdit->text());
    portNum = portLineEdit->text().toInt();
    tcpSocket->connectToHost(QHostAddress::LocalHost,portNum);
    */
    MainWindow::portName = serialPortComboBox->currentText();
    MainWindow::buadRate = buadRateComboBox->currentText().toInt();
    MainWindow::dataBitsIndex = dataBitsComboBox->currentIndex();
    MainWindow::parityIndex = stopBitsComboBox->currentIndex();
    MainWindow::stopBitsIndex = parityComboBox->currentIndex();

    MainWindow::communicationState = Communication::getInstance()->SerialInit();
    qDebug() << MainWindow::communicationState;
}

void MainWindow::resetBtn_Clicked()
{
    serialPortComboBox->setCurrentIndex(0);
    buadRateComboBox->setCurrentIndex(3);
    dataBitsComboBox->setCurrentIndex(3);
    stopBitsComboBox->setCurrentIndex(2);
    parityComboBox->setCurrentIndex(0);

    MainWindow::portName = serialPortComboBox->currentText();
    MainWindow::buadRate = buadRateComboBox->currentText().toInt();
    MainWindow::dataBitsIndex = dataBitsComboBox->currentIndex();
    MainWindow::parityIndex = stopBitsComboBox->currentIndex();
    MainWindow::stopBitsIndex = parityComboBox->currentIndex();

    if(MainWindow::communicationState)
    {
        Communication::getInstance()->SerialClose();
        MainWindow::communicationState = false;
    }


    tcpSocket->abort();
    ipLineEdit->setText("192.168.1.24");
    portLineEdit->setText("6666");

}

//kukaRobot 数据接收
void MainWindow::kukaReceiveMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_7);
    if(receiveSize == 0)
    {
        if(tcpSocket->bytesAvailable()<(int)sizeof(quint16))
            return;
        in>>receiveSize;
    }
    if(tcpSocket->bytesAvailable()<receiveSize)
        return;
    in>>receiveMsg;
    qDebug()<<receiveMsg;
}

//kukaRobot 通信故障信息提示
void MainWindow::displayError(QAbstractSocket::SocketError)
{
    tcpSocket->close();
    QMessageBox::warning(this, tr("KukaRobot通信故障"), tcpSocket->errorString(), QMessageBox::Ok);
}
//kukaRobot 数据发送
void MainWindow::kukaSendMessage()
{
    QByteArray block;
    block.resize(0);
    sendMsg = "123456789";
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<(quint16)0;
    out<<sendMsg.toLatin1();
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));
    tcpSocket->write(block);
}


/********************************数据波形显示*******************************/
double MainWindow::getData(double time)
{
    double s = qCos( time * M_PI * 2 ) ;
    return s;
}


 void MainWindow::updatedataSlot()
 {
     static QTime dataTime(QTime::currentTime());
     long int eltime = dataTime.elapsed();
     static int lastpointtime = 0;

     int size = (eltime - lastpointtime);

     if(size>0)
     {  //有数据传入
        ydata.erase(ydata.begin(),ydata.begin()+size);//擦除多余的数据
        for(int i=1;i<size+1;i++)
        {
           ydata.append(getData((((double)lastpointtime+i)/1000)));
        }
           lastpointtime = eltime;
     }
      curve->setSamples(xdata,ydata);
      curve->attach(qwtPlot);
      qwtPlot->replot();

      static double lastFpsKey;
      static int frameCount;
      ++frameCount;
      double fpstime = (double)eltime/1000.0-lastFpsKey;
      if ( fpstime> 2)
      {
             lastFpsKey = (double)eltime/1000.0;
             frameCount = 0;
      }
 }

 /*********************************系统收发数据相关****************************/
int MainWindow::saveSendRecordFun(float X, float Y, float Theta, float Joint1, float Joint2, float Joint3, float Joint4, float Joint5, float Joint6)
{
    *sendOut<< QString::number(X);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Y);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Theta);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint1);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint2);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint3);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint4);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint5);
    *sendOut<< QString(" ");
    *sendOut<< QString::number(Joint6);
    *sendOut<< QString("        ");
    *sendOut<< QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    *sendOut<< '\n';

    return 0;
}

int MainWindow::saveReceiveRecordFun(float X, float Y, float Theta, float Joint1, float Joint2, float Joint3, float Joint4, float Joint5, float Joint6)
{
    *receiveOut<< QString::number(X);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Y);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Theta);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint1);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint2);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint3);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint4);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint5);
    *receiveOut<< QString(" ");
    *receiveOut<< QString::number(Joint6);
    *receiveOut<< QString(" ");
    *receiveOut<< QDateTime::currentDateTime().toString("yyyy-MM-dd_hh:mm:ss");
    *receiveOut<< '\n';

    return 0;
}

void MainWindow::readSendRecordFun()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择日志文件"), QDir::currentPath());
    QFile file(fileName);
    if(!file.open(QIODevice::Text|QIODevice::ReadOnly|QIODevice::Truncate))
       QMessageBox::warning(this, tr("提示信息"), tr("日志文件读取失败！"), QMessageBox::Ok);
    QTextStream in(&file);

//   while(!in.atEnd())
//   {
       float X;
       float Y;
       float Theta;
       float Joint1;
       float Joint2;
       float Joint3;
       float Joint4;
       float Joint5;
       float Joint6;
       QString str;
       char ch;

       in>>X;
       in>>Y;
       in>>Theta;
       in>>Joint1;
       in>>Joint2;
       in>>Joint3;
       in>>Joint4;
       in>>Joint5;
       in>>Joint6;
       in>>str;
       in>>ch;
       disTableWidget->setRowCount(100);

       disTableWidget->setItem(1, 0, new QTableWidgetItem(QString::number(X)));
       disTableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(Y)));
       disTableWidget->setItem(3, 2, new QTableWidgetItem(QString::number(Theta)));
       disTableWidget->setItem(4, 3, new QTableWidgetItem(QString::number(Joint1)));
       disTableWidget->setItem(5, 4, new QTableWidgetItem(QString::number(Joint2)));
       disTableWidget->setItem(6, 5, new QTableWidgetItem(QString::number(Joint3)));
       disTableWidget->setItem(7, 6, new QTableWidgetItem(QString::number(Joint4)));
       disTableWidget->setItem(8, 7, new QTableWidgetItem(QString::number(Joint5)));
       disTableWidget->setItem(9, 8, new QTableWidgetItem(QString::number(Joint6)));

       qDebug()<<X;
       qDebug()<<Y;
       qDebug()<<Theta;
       qDebug()<<Joint1;
       qDebug()<<Joint2;
       qDebug()<<Joint3;
       qDebug()<<Joint4;
       qDebug()<<Joint5;
       qDebug()<<Joint6;
}

void MainWindow::readReceiveRecordFun()
{
    ;
}

/**  车臂系统轨迹规划功能函数定义 ***************************************/
// 点动函数定义
void MainWindow::pointToPoint()
{
    // 点动目标值设定(绝对位置)
    double terminalPos[6] = {0};
    terminalPos[0] = toolSetSpinBoxX->value();
    terminalPos[1] = toolSetSpinBoxY->value();
    terminalPos[2] = toolSetSpinBoxZ->value();
    terminalPos[3] = toolSetSpinBoxRx->value();
    terminalPos[4] = toolSetSpinBoxRy->value();
    terminalPos[5] = toolSetSpinBoxRz->value();

    // 解算关节值
    double jointValueObj[9] = {0};

    solver.inverseFun(terminalPos,MyThread::getInstance()->jointValueCur, jointValueObj);  // Ps: jointValueCur的实时更新问题

    for(int i=0; i<9; i++)
        qDebug()<<jointValueObj[i];
}

// 直线轨迹函数定义
void MainWindow::straightLine()
{
    // 直线插补关节目标位置(相对 Vs 绝对) ----->进一步商议
    double terminalPos[6] = {0};
    terminalPos[0] = toolSetSpinBoxX->value();
    terminalPos[1] = toolSetSpinBoxY->value();
    terminalPos[2] = toolSetSpinBoxZ->value();
    terminalPos[3] = toolSetSpinBoxRx->value();
    terminalPos[4] = toolSetSpinBoxRy->value();
    terminalPos[5] = toolSetSpinBoxRz->value();

    // 解算关节序列
    double jointValueArray[100][9] = {0};
    solver.lineInterpolationFun(terminalPos, MyThread::getInstance()->jointValueCur, *jointValueArray);

    for(int i=0; i<100; i++)
        for(int j=0; j<9; j++)
            qDebug()<<jointValueArray[i][j];
}

// 圆弧轨迹函数定义
void MainWindow::circularPath()
{
    /********* 待定义 **********/
}
// 急停函数定义
void MainWindow::emergencyStop()
{
    /****** 待定义 ********/
}
// 速度值更新函数定义
void MainWindow::velocityUpdate(int value)
{
    velocityLineEdit->setText(QString::number(value, 10)+".00");
    velocityValue = value;
}
// 模式选择更新槽函数定义
void MainWindow::modelStraightPath(bool checked)
{
     if(checked)
         ms = Model::straightPath;
}
void MainWindow::modelCircularPath(bool checked)
{
    if(checked)
        ms = Model::circularPath;
}
void MainWindow::modelPointToPoint(bool checked)
{
    if(checked)
        ms = Model::pointTopoint;
}
void MainWindow::modelDemo1(bool checked)
{
    if(checked)
        ms = Model::demo1;
}
void MainWindow::modelDemo2(bool checked)
{
    if(checked)
        ms = Model::demo2;
}
// 执行按键槽函数
void MainWindow::executeRoutine()
{
    switch(ms){
        case Model::straightPath:
            qDebug() << "straightPath";
            straightLine();
            break;
        case Model::circularPath:
            qDebug() << "circularPath";
            break;
        case Model::pointTopoint:
            pointToPoint();
            qDebug() << "pointTopoint";
            break;
        case Model::demo1:
            qDebug() << "demo1";
            break;
        case Model::demo2:
            qDebug() << "demo2";
            break;
    }
}
// 急停槽函数定义
void MainWindow::stopRoutine()
{
    qDebug() << "急停";
}
// 单一关节微调槽函数定义
void MainWindow::jointL1Clicked()
{
    qDebug() << "jointL1Clicked";
}
void MainWindow::jointL2Clicked()
{
    qDebug() << "jointL2Clicked";
}
void MainWindow::jointL3Clicked()
{
    qDebug() << "jointL3Clicked";
}
void MainWindow::jointL4Clicked()
{
    qDebug() << "jointL4Clicked";
}
void MainWindow::jointL5Clicked()
{
    qDebug() << "jointL5Clicked";
}
void MainWindow::jointL6Clicked()
{
    qDebug() << "jointL6Clicked";
}
void MainWindow::jointR1Clicked()
{
    qDebug() << "jointR1Clicked";
}
void MainWindow::jointR2Clicked()
{
    qDebug() << "jointR2Clicked";
}
void MainWindow::jointR3Clicked()
{
    qDebug() << "jointR3Clicked";
}
void MainWindow::jointR4Clicked()
{
    qDebug() << "jointR4Clicked";
}
void MainWindow::jointR5Clicked()
{
    qDebug() << "jointR5Clickedy";
}
void MainWindow::jointR6Clicked()
{
    qDebug() << "jointR6Clicked";
}

// 关节复位槽函数定义
void MainWindow::resetButtonClicked()
{
    qDebug() << "jointReset";
}

// 界面定时更新槽函数
void MainWindow::updateInterface()
{
   updatedataSlot();
   jointBar1->setValue(MyThread::getInstance()->jointValueCur[0]);
   jointBar2->setValue(MyThread::getInstance()->jointValueCur[1]);
   jointBar3->setValue(MyThread::getInstance()->jointValueCur[2]);
   jointBar4->setValue(MyThread::getInstance()->jointValueCur[3]);
   jointBar5->setValue(MyThread::getInstance()->jointValueCur[4]);
   jointBar6->setValue(MyThread::getInstance()->jointValueCur[5]);

   toolDisLineEditX->setText("0.00");
   toolDisLineEditY->setText("0.00");
   toolDisLineEditZ->setText("0.00");
   toolDisLineEditRx->setText("0.00");
   toolDisLineEditRy->setText("0.00");
   toolDisLineEditRz->setText("0.00");
}


