#include "mainwindow.h"
#include <QApplication>
#include <authentication.h>
#include "linear_Interpolation.h"
#include "add.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //中文兼容性考虑
    #ifdef QT4
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForCStrings(codec);
    #endif

    MainWindow w;


/*
    if (!linear_InterpolationInitialize())
        {
            cout << "initilize failed!!!" << endl;
            return -1;
        }

        mwArray x(1, 1, mxDOUBLE_CLASS);
        mwArray y(1, 1, mxDOUBLE_CLASS);
        mwArray z(1, 1, mxDOUBLE_CLASS);
        mwArray wx(1, 1, mxDOUBLE_CLASS);
        mwArray wy(1, 1, mxDOUBLE_CLASS);
        mwArray wz(1, 1, mxDOUBLE_CLASS);

        x(1, 1) = 0.3;
        y(1, 1) = 0.3;
        z(1, 1) = 0.3;
        wx(1, 1) = 0;
        wy(1, 1) = 1.74;
        wz(1, 1) = 0;

        mwArray dataS(6, 100, mxDOUBLE_CLASS);

        //输出参数个数,输出参数,输入参数。
        linear_Interpolation(1,dataS, x, y, z, wx, wy, wz);
        linear_InterpolationTerminate();
        */


    /*
    Authentication *user = new Authentication();
    user->show();

    if(user->exec()==QDialog::Accepted)
    {
       w.show();
       return a.exec();
    }
    else
        return 0;
    */

    w.show();
    return a.exec();
}
