#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include<QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#include <QTcpServer>

class Communication
{
public:
    static Communication* getInstance()
    {
        if(singleton == 0)
            singleton = new Communication();
        return singleton;
    }

    static void destory()
    {
        if(singleton != 0)
        {
            delete singleton;
            singleton = 0;
        }
    }

    /**  485通信  **/
    bool SerialInit();
    void SerialClose();
    int SerialWrite(QString strBuf);
    int SerialAvailableBytes();
    QByteArray SerialRead();


    /**  TCP通信  **/
    bool TcpInit();
    void TcpClose();
    int TcpWrite(QString strBuf);
    int TcpAvailableBytes();
    QByteArray TcpReceive();



    /**  通信初始化与关闭  **/
    bool CommunicationInit();
    void CommunicationClose();



    /** 通信主要接口函数  **/
    // 发送数组 double sendArray[6]: agv_x,agv_y,agv_th,posX,posY,posZ,posA,posB,posC;
    bool SendInstruction(double* sendArray);
    // 接受数组 double receiveArray[15]:agv_x,agv_y,agv_th,theta1,theta2,theta3,theta4,theta5,theta6,posX,posY,posZ,posA,posB,posC;
    int ReceiveInstruction(double* receiveArray);

/**  成员函数  **/
private:
    Communication();
    ~Communication();



/**  成员变量  **/
private:
    static Communication* singleton;
    QSerialPort *agvPort;
    QTcpSocket *tcpSocket;
};

#endif // COMMUNICATION_H
