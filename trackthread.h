#ifndef TRACKTHREAD_H
#define TRACKTHREAD_H
#include <QThread>


class TrackThread : public QThread
{
public:
    void stop();

    static TrackThread* getInstance()
    {
        if(singleton == 0)
            singleton = new TrackThread();
        return singleton;
    }

    static void destroy()
    {
        if(singleton != 0)
        {
            delete singleton;
            singleton = 0;
        }
    }


private:
    TrackThread();
    static TrackThread* singleton;



};

#endif // TRACKTHREAD_H
