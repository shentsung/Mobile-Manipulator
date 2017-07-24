#include "mythread.h"
#include <QDebug>
#include "mainwindow.h"
#include "communication.h"

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
            // qDebug() << "nyThread...";
            sleep(0.01);
            receivedArray = Communication::getInstance()->SerialRead();

            // qDebug() << receivedArray;
        }

    }
    stopped = false;
}


void MyThread::stop()
{
    stopped = true;
}



