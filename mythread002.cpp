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
    for(int i=0; i<7; i++)
        agvValueCur[i] = 0;
}

// 线程循环体主程序
void MyThread::run()
{
    while(!stopped)
    {
        if(MainWindow::communicationState)
        {

            // 检测485、TCP/IP 端口数据接收情况
            int serialReceivedNum = Communication::getInstance()->SerialAvailableBytes();
            int tcpReceivedNum = Communication::getInstance()->TcpAvailableBytes();

            /** 接受并处理来自AGV全向车的状态信息 **/
            if(serialReceivedNum >= 10)
                qDebug() << Communication::getInstance()->SerialRead();

            /** 接受并处理来自KUKA工业机器人的XML文本信息 **/
            if(tcpReceivedNum >= 300)
            {

                QString receicedXml = Communication::getInstance()->TcpReceive();
                // XML文本解析，提取重要数据    忽略XML异常文本
                // 处理速度测试结果：0.001s
                QDomDocument doc;
                if(!doc.setContent(receicedXml)){
                    // 结束本次循环并进入下一次循环
                    continue;
                }

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
        stopped = false;
    }
}




