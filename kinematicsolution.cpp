#include "kinematicsolution.h"
#include "inverse.h"
#include "lineInterpolation.h"
#include <qDebug>

// 构造函数定义
KinematicSolution::KinematicSolution()
{

    // 函数初始化
    if(!inverseInitialize())
        qDebug()<< "initialize inverseFun failed";
    if(!lineInterpolationInitialize())
         qDebug()<< "initialize lineInterpolationFun failed";
}
// 析构函数定义
KinematicSolution::~KinematicSolution()
{
    // 函数释放
    inverseTerminate();
    lineInterpolationTerminate();
}


// terminalPos 数组：x y z wx wy wz
// jointValue  数组：x y th theta1 theta2 theta3 theta4 theta5 theta6
int KinematicSolution::inverseFun(/*in*/double* terminalPos, double* jointValueCur, /*out*/double* jointValueObj)
{

    // terminalPos
    mwArray x_cur(1,1,mxDOUBLE_CLASS);
    mwArray y_cur(1,1,mxDOUBLE_CLASS);
    mwArray z_cur(1,1,mxDOUBLE_CLASS);
    mwArray wx_cur(1,1,mxDOUBLE_CLASS);
    mwArray wy_cur(1,1,mxDOUBLE_CLASS);
    mwArray wz_cur(1,1,mxDOUBLE_CLASS);

    x_cur(1, 1) = terminalPos[0];
    y_cur(1, 1) = terminalPos[1];
    z_cur(1, 1) = terminalPos[2];
    wx_cur(1, 1) = terminalPos[3];
    wy_cur(1, 1) = terminalPos[4];
    wz_cur(1, 1) = terminalPos[5];

    // jontValueCur
    mwArray x(1,1,mxDOUBLE_CLASS);
    mwArray y(1,1,mxDOUBLE_CLASS);
    mwArray th(1,1,mxDOUBLE_CLASS);
    mwArray theta1(1,1,mxDOUBLE_CLASS);
    mwArray theta2(1,1,mxDOUBLE_CLASS);
    mwArray theta3(1,1,mxDOUBLE_CLASS);
    mwArray theta4(1,1,mxDOUBLE_CLASS);
    mwArray theta5(1,1,mxDOUBLE_CLASS);
    mwArray theta6(1,1,mxDOUBLE_CLASS);

    x(1, 1) = jointValueCur[0];
    y(1, 1) = jointValueCur[1];
    th(1, 1) = jointValueCur[2];
    theta1(1, 1) = jointValueCur[3];
    theta2(1, 1) = jointValueCur[4];
    theta3(1, 1) = jointValueCur[5];
    theta4(1, 1) = jointValueCur[6];
    theta5(1, 1) = jointValueCur[7];
    theta6(1, 1) = jointValueCur[8];


    mwArray jointValue(1, 9, mxDOUBLE_CLASS);

    // inverse 函数调用
    inverse(1, jointValue, x, y, th, theta1, theta2, theta3, theta4, theta5, theta6, x_cur, y_cur, z_cur, wx_cur, wy_cur, wz_cur);

    // jontValueObj
    jointValue.GetData(jointValueObj, 9);
    return 0;
}

int KinematicSolution::lineInterpolationFun(/*in*/double* deltaTerminalPos, double* jointValueCur, /*out*/double* jointValueArray)
{
    // deltaTerminalPos
    mwArray deltaX_cur(1,1,mxDOUBLE_CLASS);
    mwArray deltaY_cur(1,1,mxDOUBLE_CLASS);
    mwArray deltaZ_cur(1,1,mxDOUBLE_CLASS);
    mwArray deltaWX_cur(1,1,mxDOUBLE_CLASS);
    mwArray deltaWY_cur(1,1,mxDOUBLE_CLASS);
    mwArray deltaWZ_cur(1,1,mxDOUBLE_CLASS);

    deltaX_cur(1, 1) = deltaTerminalPos[0];
    deltaY_cur(1, 1) = deltaTerminalPos[1];
    deltaZ_cur(1, 1) = deltaTerminalPos[2];
    deltaWX_cur(1, 1) = deltaTerminalPos[3];
    deltaWY_cur(1, 1) = deltaTerminalPos[4];
    deltaWZ_cur(1, 1) = deltaTerminalPos[5];


    // jontValueCur
    mwArray x(1,1,mxDOUBLE_CLASS);
    mwArray y(1,1,mxDOUBLE_CLASS);
    mwArray th(1,1,mxDOUBLE_CLASS);
    mwArray theta1(1,1,mxDOUBLE_CLASS);
    mwArray theta2(1,1,mxDOUBLE_CLASS);
    mwArray theta3(1,1,mxDOUBLE_CLASS);
    mwArray theta4(1,1,mxDOUBLE_CLASS);
    mwArray theta5(1,1,mxDOUBLE_CLASS);
    mwArray theta6(1,1,mxDOUBLE_CLASS);

    x(1, 1) = jointValueCur[0];
    y(1, 1) = jointValueCur[1];
    th(1, 1) = jointValueCur[2];
    theta1(1, 1) = jointValueCur[3];
    theta2(1, 1) = jointValueCur[4];
    theta3(1, 1) = jointValueCur[5];
    theta4(1, 1) = jointValueCur[6];
    theta5(1, 1) = jointValueCur[7];
    theta6(1, 1) = jointValueCur[8];

    // 函数传出矩阵固定
    mwArray jointValue(100, 9, mxDOUBLE_CLASS);

    // 函数调用
    lineInterpolation(1, jointValue, x, y, th, theta1, theta2, theta3, theta4, theta5, theta6, deltaX_cur, deltaY_cur, deltaZ_cur, deltaWX_cur, deltaWX_cur, deltaWZ_cur);

    // jointValueArray
    jointValue.GetData(jointValueArray, 900);

    return 0;
}


