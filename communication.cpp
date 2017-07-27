#include "communication.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "mainwindow.h"

Communication* Communication::singleton = 0;

Communication::Communication()
{
    ;
}

Communication::~Communication()
{
    CommunicationClose();
}

/**  485通信函数定义  **/
bool Communication::SerialInit()
{
    agvPort = new QSerialPort(MainWindow::portName);
    if(agvPort->open(QIODevice::ReadWrite))
    {
        qDebug()<< "485 establish success";
    }
    else
    {
        qDebug()<< "485 establish failed";
        return false;
    }
    agvPort->setBaudRate(MainWindow::buadRate);
    agvPort->setDataBits((QSerialPort::DataBits)MainWindow::dataBitsIndex);
    agvPort->setParity((QSerialPort::Parity)MainWindow::parityIndex);
    agvPort->setStopBits((QSerialPort::StopBits)MainWindow::stopBitsIndex);
    agvPort->setFlowControl(QSerialPort::NoFlowControl);
    return true;
}

void Communication::SerialClose()
{
    agvPort->close();
}

int Communication::SerialWrite(QString strBuf)
{
   return agvPort->write(strBuf.toLatin1());
}

int Communication::SerialAvailableBytes()
{
    return agvPort->bytesAvailable();
}

QByteArray Communication::SerialRead()
{
    return agvPort->readAll();
}


/**  Tcp通信函数定义  **/
bool Communication::TcpInit()
{
    tcpSocket = new QTcpSocket();

    tcpSocket->abort();

    // ip地址   端口号
    tcpSocket->connectToHost(QHostAddress::LocalHost,MainWindow::portNum);
    return true;
}

void Communication::TcpClose()
{
    tcpSocket->abort();
}

int Communication::TcpWrite(QString strBuf)
{
   return tcpSocket->write(strBuf.toLatin1());
}

QByteArray Communication::TcpReceive()
{ 
    return tcpSocket->readAll();
}

int Communication::TcpAvailableBytes()
{
    return tcpSocket->bytesAvailable();
}


/**  通信模块接口函数    全向车协议+机械臂协议  **/
bool Communication::CommunicationInit()
{
    bool serialStatus = SerialInit();
    bool tcpStatus = TcpInit();
    if(serialStatus && tcpStatus)
        return true;
    return false;
}

void Communication::CommunicationClose()
{
    TcpClose();
    SerialClose();
}


bool Communication::SendInstruction(double* sendArray)
{
    QString str1 = "Serial testing...";
    QString str2 = "Tcp testing...";

    if((-1!=SerialWrite(str1.toLatin1())) && (-1 != TcpWrite(str2.toLatin1())))
    {
        qDebug() << "send Message test....";
        return true;
    }
    return false;
}

int Communication::ReceiveInstruction(double* receiveArray)
{
    return 0;
}


