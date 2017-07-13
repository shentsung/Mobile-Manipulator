#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

/**  **********************基于单件设计模式 *************************/

class MyThread : public QThread
{
     Q_OBJECT
public: 
    double jointValueCur[9];      // 当前机器人姿态
    void stop();

    static MyThread* getInstance()
    {
        if(singleton == 0)
            singleton = new MyThread();
        return singleton;
    }

    static void destory()
    {
        if(singleton == 0)
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
};

#endif // MYTHREAD_H
