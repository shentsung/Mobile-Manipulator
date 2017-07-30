#include "communication.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtXml>
#include <QDomDocument>

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

// 发送数组 double sendArray[6]: agv_x,agv_y,agv_th,posX,posY,posZ,posA,posB,posC;
bool Communication::SendInstruction(double* sendArray, int commander)
{
    // 创建KUKA机器人通信的XML文本
    QDomDocument doc;
    QDomElement root = doc.createElement(QString("Robot"));
    doc.appendChild(root);

    QDomElement posX = doc.createElement(QString("RPosX"));
    QDomElement posY = doc.createElement(QString("RPosY"));
    QDomElement posZ = doc.createElement(QString("RPosZ"));
    QDomElement posA = doc.createElement(QString("RPosA"));
    QDomElement posB = doc.createElement(QString("RPosB"));
    QDomElement posC = doc.createElement(QString("RPosC"));
    QDomElement command = doc.createElement(QString("RCommand"));


    QDomText text;

    text = doc.createTextNode(QString::number(sendArray[3],10,2));
    posX.appendChild(text);
    text = doc.createTextNode(QString::number(sendArray[4],10,2));
    posY.appendChild(text);
    text = doc.createTextNode(QString::number(sendArray[5],10,2));
    posZ.appendChild(text);
    text = doc.createTextNode(QString::number(sendArray[6],10,2));
    posA.appendChild(text);
    text = doc.createTextNode(QString::number(sendArray[7],10,2));
    posB.appendChild(text);
    text = doc.createTextNode(QString::number(sendArray[8],10,2));
    posC.appendChild(text);
    text = doc.createTextNode(QString::number(commander));
    command.appendChild(text);


    root.appendChild(posX);
    root.appendChild(posY);
    root.appendChild(posZ);
    root.appendChild(posA);
    root.appendChild(posB);
    root.appendChild(posC);
    root.appendChild(command);

    QString kukaXmlStr = doc.toString();

    // AGV全向车发送指令
    QString str1 = "Serial testing...";

    if((-1!=SerialWrite(str1.toLatin1())) && (-1 != TcpWrite(kukaXmlStr.toLatin1())))
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


