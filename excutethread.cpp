#include "excutethread.h"
#include "mainwindow.h"
#include "communication.h"

ExcuteThread* ExcuteThread::singleton = 0;

ExcuteThread::ExcuteThread()
{
    stopped = false;
    qDebug() << "establish Excute Singleton";
}


ExcuteThread::~ExcuteThread()
{
    qDebug() << "destroy a Excute Singleton";
}



// 线程循环体主程序
void ExcuteThread::run()
{
    int index = 0;
    double terminalPos[9] = {0};
    while(!stopped)
    {
        if(MainWindow::communicationState)
        {
            int tcpReceivedNum = Communication::getInstance()->TcpAvailableBytes();
            if(MainWindow::excuteFlag)
            {
                //if(index == MyThread::getInstance()->jointValueCur[12])
                if(1)
                {
                    Communication::getInstance()->SendInstruction(terminalPos, 1);
                    sleep(1);
                    index++;
                    if(index == 10)
                    {
                       MainWindow::excuteFlag = false;
                       qDebug()<<"123456";
                    }
                    qDebug()<<MainWindow::jointsArray[index];
                }
            }
        }
    }
}
