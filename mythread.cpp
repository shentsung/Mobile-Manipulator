#include "mythread.h"
#include <QDebug>
#include "mainwindow.h"
#include "communication.h"
#include <Windows.h>

MyThread* MyThread::singleton = 0;

MyThread::MyThread()
{
    stopped = false;
    receivedArray.clear();
    for(int i=0; i<9; i++)
        jointValueCur[i] = 0;
}


void MyThread::run()
{
    while(!stopped)
    {
        if(MainWindow::communicationState)
        {
            int serialReceivedNum = Communication::getInstance()->SerialAvailableBytes();
            int tcpReceivedNum = Communication::getInstance()->TcpAvailableBytes();
            if(serialReceivedNum >= 10)
                qDebug() << Communication::getInstance()->SerialRead();
            if(tcpReceivedNum >= 10)
                qDebug() << Communication::getInstance()->TcpReceive();
        }
    }
    stopped = false;
}


void MyThread::stop()
{
    stopped = true;
}



