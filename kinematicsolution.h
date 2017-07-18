#ifndef KINEMATICSOLUTION_H
#define KINEMATICSOLUTION_H

#include <vector>
#include "inverse.h"
#include "kinematicsolution.h"
#include "forward.h"

class KinematicSolution
{
public:
    static KinematicSolution* getInstance()
    {
        if(singleton == 0)
            singleton = new KinematicSolution();
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

// 成员变量定义
private:
    KinematicSolution();
    ~KinematicSolution();
    static KinematicSolution* singleton;

// 成员函数定义
public:
    // 单步运动规划函数：从当前位置 --> 目标位置(绝对位置)----> point to point 点动动作
    // terminalPos 数组：x y z wx wy wz
    // jointValueCur  数组：x y th theta1 theta2 theta3 theta4 theta5 theta6
    // jointValueObj  数组：同上
    int inverseFun(/*in*/const double* terminalPos, const double* jointValueCur, /*out*/double* jointValueObj);

    // 正运动学函数
    // jointValueCur  数组：x y th theta1 theta2 theta3 theta4 theta5 theta6
    // terminalPos    数组：x y z wx wy wz   末端位姿
    int forwardFun(/*in*/const double* jointValueCur, /*out*/double* terminalPos);

    // 直线插补规划函数：从当前位置 --> 目标位置(规划位置)----> straight line 直线轨迹
    // deltaTerminalPos 数组：delta_x  delta_y  delta_z  delta_wx  delta_wy  delta_wz
    // jointValueArray 规划结果序列  矩阵: 100*9
    int lineInterpolationFun(/*in*/const double* deltaTerminalPos, const double* jointValueCur, /*out*/double* jointValueArray);
};

#endif // KINEMATICSOLUTION_H
