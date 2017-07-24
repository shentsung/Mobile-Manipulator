#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

/**  **********************基于单件设计模式 *************************/
/**  实时采集系统各关节位置信息  **/

class MyThread : public QThread
{
     Q_OBJECT
public: 
    // jointValueCur  数组：x y th theta1 theta2 theta3 theta4 theta5 theta6
    double jointValueCur[9];
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
