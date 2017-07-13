#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QRoundProgressBar.h>
#include <QProgressBar>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QSlider>

#include <QHBoxLayout>
#include <QStackedWidget>

#include "qwt/qwt_plot.h"
#include "qwt/qwt_plot_curve.h"


#include <QTimer>

#include <QVector>
#include <math.h>

#include<QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#include <QTcpServer>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

#include <QFile>
#include <QTextStream>

#include "kinematicsolution.h"

#include "mythread.h"
#include <QTimer>

//按钮大小宏定义
#define B_S_W 144  //按钮宽度
#define B_S_H 50   //按钮高度

//界面总体尺寸宏定义
#define WIN_W QApplication::desktop()->width()-10
#define WIN_H QApplication::desktop()->height()-70

//菜单按键大小宏定义
#define M_B_W  0.12*WIN_W
#define M_B_H  0.05*WIN_W



//QT5与QT4的兼容性
#define QT5

#ifdef QT5
#define strProcss tr  // QT5
#else
#define strProcss QStringLiteral
#endif


class QTcpSocket;

namespace Ui {
class MainWindow;
}

// 模式选择枚举类型定义
enum Model{
    straightPath,
    circularPath,
    pointTopoint,
    demo1,
    demo2
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

/****************************界面初始化成员函数*******************************/
public:
    void buttonInitialize();
    void roundProgressBarInit();
    void MainWindowInit();
    void menuPageInitialize();
    void firstPageInitialize();
    void secondPageInitialize();
    void thirdPageInitialize();
    void fourthPageInitialize();
    void fifthPageInitialize();
    void sixthPageInitialize();
    void signalSlotConnection();


//定义槽函数
private slots:
     void managerBtn_clicked();
     void communicationBtn_clicked();
     void userInterfaceBtn_clicked();
     void systemParaBtn_clicked();
     void historyRecordBtn_clicked();
     void aboutBtn_clicked();



//***************************界面控件定义***********************************/
private:
    QPushButton *maxBtn;
    QPushButton *minBtn;
    QPushButton *closeBtn;
//菜单栏PushBtn定义
    QPushButton *managerBtn;
    QPushButton *communicationBtn;
    QPushButton *userInterfaceBtn;
    QPushButton *systemParaBtn;
    QPushButton *historyRecordBtn;
    QPushButton *aboutBtn;

//移动平台速度显示控件定义
    QRoundProgressBar *wheelBar1;
    QRoundProgressBar *wheelBar2;
    QRoundProgressBar *wheelBar3;
    QRoundProgressBar *wheelBar4;
//移动平台速度标签定义
    QLabel wheelLabel1;
    QLabel wheelLabel2;
    QLabel wheelLabel3;
    QLabel wheelLabel4;



//菜单栏
    QStackedWidget *menuStackedWidget;
    QWidget *menuPageWidget;
//主界面区
    QStackedWidget *stackedWidget;
    QWidget *firstPageWidget;
    QWidget *secondPageWidget;
    QWidget *thirdPageWidget;
    QWidget *fourthPageWidget;
    QWidget *fifthPageWidget;
    QWidget *sixthPageWidget;

//状态信息显示
    QGroupBox *stateMessageBox;
    QGroupBox *menuGroupBox;
    QGroupBox *jointGroupBox;
    QGroupBox *toolPosGroupBox;
    QGroupBox *agvPosGroupBox;
    QGroupBox *modelGroupBox;
    QGroupBox *speedSetGroupBox;

//关节位置界面控件定义
    QProgressBar *jointBar1;
    QProgressBar *jointBar2;
    QProgressBar *jointBar3;
    QProgressBar *jointBar4;
    QProgressBar *jointBar5;
    QProgressBar *jointBar6;

    QLabel *jointLabel1;
    QLabel *jointLabel2;
    QLabel *jointLabel3;
    QLabel *jointLabel4;
    QLabel *jointLabel5;
    QLabel *jointLabel6;

    QLabel *unitLabel1;
    QLabel *unitLabel2;
    QLabel *unitLabel3;
    QLabel *unitLabel4;
    QLabel *unitLabel5;
    QLabel *unitLabel6;

    QPushButton *jointRowL1;
    QPushButton *jointRowL2;
    QPushButton *jointRowL3;
    QPushButton *jointRowL4;
    QPushButton *jointRowL5;
    QPushButton *jointRowL6;
    QPushButton *jointRowR1;
    QPushButton *jointRowR2;
    QPushButton *jointRowR3;
    QPushButton *jointRowR4;
    QPushButton *jointRowR5;
    QPushButton *jointRowR6;

    QLineEdit *jointLine1;
    QLineEdit *jointLine2;
    QLineEdit *jointLine3;
    QLineEdit *jointLine4;
    QLineEdit *jointLine5;
    QLineEdit *jointLine6;

    QPushButton *resetButton;

    //工具位置界面控件定义
    QGroupBox *toolSetPosition;

    //工具位置设置控件定义
    QLabel *toolSetLabelX;
    QLabel *toolSetLabelY;
    QLabel *toolSetLabelZ;
    QLabel *toolSetLabelRx;
    QLabel *toolSetLabelRy;
    QLabel *toolSetLabelRz;

    QDoubleSpinBox *toolSetSpinBoxX;
    QDoubleSpinBox *toolSetSpinBoxY;
    QDoubleSpinBox *toolSetSpinBoxZ;
    QDoubleSpinBox *toolSetSpinBoxRx;
    QDoubleSpinBox *toolSetSpinBoxRy;
    QDoubleSpinBox *toolSetSpinBoxRz;

    QLabel *toolSetUnitX;
    QLabel *toolSetUnitY;
    QLabel *toolSetUnitZ;
    QLabel *toolSetUnitRx;
    QLabel *toolSetUnitRy;
    QLabel *toolSetUnitRz;
    //工具位置显示控件定义
    QLabel *toolDisLabelX;
    QLabel *toolDisLabelY;
    QLabel *toolDisLabelZ;
    QLabel *toolDisLabelRx;
    QLabel *toolDisLabelRy;
    QLabel *toolDisLabelRz;

    QLineEdit *toolDisLineEditX;
    QLineEdit *toolDisLineEditY;
    QLineEdit *toolDisLineEditZ;
    QLineEdit *toolDisLineEditRx;
    QLineEdit *toolDisLineEditRy;
    QLineEdit *toolDisLineEditRz;

    QLabel *toolDisUnitX;
    QLabel *toolDisUnitY;
    QLabel *toolDisUnitZ;
    QLabel *toolDisUnitRx;
    QLabel *toolDisUnitRy;
    QLabel *toolDisUnitRz;

    QLabel *pictureDisplay;

    //曲线图对象
    QwtPlot *qwtPlot;
    //曲线图数据对象定义
    QVector<double> xdata;
    QVector<double> ydata;
    QwtPlotCurve *curve ;
    double getData(double inteval);


    //操作面板部分界面定义
    // 基本模式
    QRadioButton *pointToPointRadioBtn;
    QRadioButton *straightLineRadioBtn;
    QRadioButton *circularPathRadioBtn;
    // 任务模式
    QRadioButton *demo1RadioBtn;
    QRadioButton *demo2RadioBtn;

    QPushButton *operationBtn;
    QPushButton *stopBtn;

    QSlider *velocitySlider;
    QLineEdit *velocityLineEdit;
    QLabel *veloctyUnitLabel;
    
    
    //密码管理页面控件定义
    QLabel *passWordTitle;
    QLabel *passWordLabel;
    QLineEdit *passWord;
    QLabel *newPassWordLabel;
    QLineEdit *newPassWord;
    QLineEdit *newPassWordAgain;
    QLabel *newPassWordAgainLabel;
    QPushButton *passWordConfigBtn;
    QPushButton *passWordCancelBtn;
    QGroupBox *containerBox;
    
    //帮助页面控件定义
    QTextEdit *helpTextEdit;
    QLabel *helpTitle;
    
    //通信设置页面控件定义       两部分：KUKA机械臂通讯(TCP/IP)    AGV通讯(暂时未知)

    //布局控件
    QLabel *communicationTitleLabel;
    QGroupBox *CommunicationGroupBox;

    //kuka机械臂
    QGroupBox *kukaGroupBox;
    QLabel *ipLabel;
    QLineEdit *ipLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QPushButton *connectBtn;
    QPushButton *resetBtn;

    //AGV全向车
    QGroupBox *agvGroupBox;

    //历史记录访问页面
    QLabel *historyTitle;
    QTableWidget *disTableWidget;
    QPushButton *openSendBtn;
    QPushButton *openReceiveBtn;


/********************************通信模块***********************************/
private:  
    //kukaRobot Tcp通信
    QTcpSocket *tcpSocket;
    QHostAddress ipAddress;
    int portNum;

    QString receiveMsg;
    quint16 receiveSize;
    QString sendMsg;

private slots:
    //通信相关槽函数定义
    void connectBtn_Clicked();
    void resetBtn_Clicked();
    void kukaReceiveMessage();
    void kukaSendMessage();
    void displayError(QAbstractSocket::SocketError);


/*******************************波形图表显示************************************/
public:
    void updatedataSlot();



/*******************************密码管理模块************************************/
private slots:
    void passWordConfigBtn_Clicked();
    void passWordCancelBtn_Clicked();

/****************************收发数据文件读写模块********************************/
public:
    int saveSendRecordFun(float X, float Y, float Theta, float Joint1, float Joint2, float Joint3, float Joint4, float Joint5, float Joint6 );
    int saveReceiveRecordFun(float X, float Y, float Theta, float Joint1, float Joint2, float Joint3, float Joint4, float Joint5, float Joint6 );

private slots:
    void readSendRecordFun();
    void readReceiveRecordFun();

private:
    QString sendFileName;
    QString receiveFileName;
    QFile *sendFileInstance;
    QFile *receiveFileInstance;
    QTextStream *sendOut;
    QTextStream *receiveOut;


/** *****************************机器人轨迹规划**********************************/
private:
    KinematicSolution solver;
    double velocityValue;
    Model ms;
    //MyThread updateThread;
    QTimer updateInterfaceTimer;


// 车臂系统轨迹规划功能函数定义
public:
    // 1. 点动函数
    void pointToPoint();
    // 2. 直线轨迹函数
    void straightLine();
    // 3. 圆弧轨迹函数
    void circularPath();
    // 4. 急停函数
    void emergencyStop();

// 车臂系统功能槽函数定义
private slots:
    // 速度值更新
    void velocityUpdate(int value);
    // 选择模式更新槽函数
    void modelStraightPath(bool checked);
    void modelCircularPath(bool checked);
    void modelPointToPoint(bool checked);
    void modelDemo1(bool checked);
    void modelDemo2(bool checked);
    // 执行槽函数
    void executeRoutine();
    // 急停槽函数
    void stopRoutine();
    // 关节微调槽函数定义
    void jointL1Clicked();
    void jointL2Clicked();
    void jointL3Clicked();
    void jointL4Clicked();
    void jointL5Clicked();
    void jointL6Clicked();

    void jointR1Clicked();
    void jointR2Clicked();
    void jointR3Clicked();
    void jointR4Clicked();
    void jointR5Clicked();
    void jointR6Clicked();
    // 关节复位槽函数
    void resetButtonClicked();
    // 界面定时更新槽函数
    void updateInterface();

};

#endif // MAINWINDOW_H
