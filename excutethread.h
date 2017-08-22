#ifndef EXCUTETHREAD_H
#define EXCUTETHREAD_H
#include <QThread>

class ExcuteThread : public QThread
{
    Q_OBJECT
public:
    void stop(){stopped = true;}

    static ExcuteThread* getInstance_Test()
    {
        if(singleton == 0)
            singleton = new ExcuteThread();
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
    ExcuteThread();
     ~ExcuteThread();
    static ExcuteThread* singleton;
    volatile bool stopped;
};

#endif // EXCUTETHREAD_H
