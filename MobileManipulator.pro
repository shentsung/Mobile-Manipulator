#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T15:41:20
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MobileManipulator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        QRoundProgressBar.cpp \
    authentication.cpp \
    kinematicsolution.cpp \
    mythread.cpp \
    communication.cpp

HEADERS  += mainwindow.h \
         QRoundProgressBar.h \
    databaseconnection.h \
    authentication.h \
    kinematicsolution.h \
    inverse.h \
    lineInterpolation.h \
    mythread.h \
    communication.h


RESOURCES += \
          images.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Study_Software/Qt5.7.0/5.7/msvc2015_64/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Study_Software/Qt5.7.0/5.7/msvc2015_64/lib/ -lqwtd

INCLUDEPATH += $$PWD/../../../../Study_Software/Qt5.7.0/5.7/msvc2015_64/include
DEPENDPATH += $$PWD/../../../../Study_Software/Qt5.7.0/5.7/msvc2015_64/include

INCLUDEPATH += $$PWD/C:/Study_Software/Qt5.7.0/5.7/msvc2015_64/include/qwt

FORMS +=

INCLUDEPATH += C:\Study_SoftWare\Matlab\R2014a\extern\include

LIBS += C:/Users/yanglei/Desktop/MobileManipulator/inverse.lib
LIBS += C:/Users/yanglei/Desktop/MobileManipulator/lineInterpolation.lib

LIBS += -L$$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft/ -lmclmcr

INCLUDEPATH += $$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft
DEPENDPATH += $$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft/ -lmclmcrrt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft/ -lmclmcrrt

INCLUDEPATH += $$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft
DEPENDPATH += $$PWD/../../../../Study_SoftWare/Matlab/R2014a/extern/lib/win64/microsoft
