#include "mainwindow.h"
#include <QApplication>
#include <authentication.h>
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
