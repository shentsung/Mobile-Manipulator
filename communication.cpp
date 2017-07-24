#include "communication.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "mainwindow.h"

Communication* Communication::singleton = 0;

Communication::Communication()
{

}

Communication::~Communication()
{
    SerialClose();
}

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

QByteArray Communication::SerialRead()
{
    return agvPort->readAll();
}

// 通信协议封装    全向车协议+机械臂协议
int Communication::sendControlInstruction(double x, double y, double th, double theta1, double theta2, double theta3, double theta4, double theta5, double theta6)
{
    return 0;
}

int Communication::receiveControlInstruction(double* x, double* y, double* th, double* theta1, double* theta2, double* theta3, double* theta4, double* theta5, double* theta6)
{
    return 0;
}


