#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

class Communication
{
public:
    static Communication* getInstance()
    {
        if(singleton == 0)
            singleton = new Communication();
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

    bool SerialInit();
    void SerialClose();
    int SerialWrite(QString strBuf);


    QByteArray SerialRead();

/**  成员函数  **/
private:
    Communication();
    ~Communication();
    static Communication* singleton;


/**  成员变量  **/
private:
    QSerialPort *agvPort;
};

#endif // COMMUNICATION_H
