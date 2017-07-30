#include "mythread.h"
#include <QDebug>
#include "mainwindow.h"
#include "communication.h"
#include <Windows.h>
#include <QtXml>
#include <QDomDocument>

MyThread* MyThread::singleton = 0;

MyThread::MyThread()
{
    stopped = false;
    receivedArray.clear();
    for(int i=0; i<13; i++)
        jointValueCur[i] = 0;
    for(int i=0; i<4; i++)
        agvValueCur[i] = 0;
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
            {
                qDebug() << "xml analysis";
                // 接受XML文本
                QString receicedXml = Communication::getInstance()->TcpReceive();
                // XML文本解析，提取重要数据  ====>如何校验接受指令的正确性？
                QDomDocument doc;
                if(!doc.setContent(receicedXml))
                        return;
                QDomElement docElem = doc.documentElement();
                QDomNodeList list = docElem.childNodes();

                for(int i=0; i<list.count(); i++)
                {
                    QDomNode node = list.at(i);
                    if(node.isElement()){
                        jointValueCur[i] = node.toElement().text().toDouble();
                        qDebug()<< qPrintable(node.toElement().tagName())
                                << ":"
                                << qPrintable(node.toElement().text());
                    }
                }

            }
        }
    }
    stopped = false;
}


void MyThread::stop()
{
    stopped = true;
}



