#include "mythread.h"
#include <QDebug>

MyThread* MyThread::singleton = 0;

MyThread::MyThread()
{
    stopped = false;
    for(int i=0; i<9; i++)
        jointValueCur[i] = 0;
}


void MyThread::run()
{
    while(!stopped)
    {
        qDebug() << "nyThread...";
        sleep(1);
    }
    stopped = false;
}


void MyThread::stop()
{
    stopped = true;
}



