#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

/**  **********************基于单件设计模式 *************************/
/**  实时采集系统位置信息：AGV全向车+KUKA六自由度机械臂  **/

class MyThread : public QThread
{
     Q_OBJECT
public: 
    // agvValueCur(agv全向车当前位置)  数组：motor1 motor2 motor3 motor4;
    double agvValueCur[4];
    // jointValueCur(Kuka机械臂当前位置)  数组: posX posY posZ posA posB posC; theta1 theta2 theta3 theta4 theta5 theta6;PointNum
    double jointValueCur[13];
    void stop();

    static MyThread* getInstance()
    {
        if(singleton == 0)
            singleton = new MyThread();
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

protected:
    void run();

private:
    MyThread();
    static MyThread* singleton;
    volatile bool stopped;
    QByteArray receivedArray;
};

#endif // MYTHREAD_H
