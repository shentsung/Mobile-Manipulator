#ifndef KINEMATICSOLUTION_H
#define KINEMATICSOLUTION_H

#include <vector>
#include "inverse.h"
#include "kinematicsolution.h"

class KinematicSolution
{
public:
    KinematicSolution();
    ~KinematicSolution();


// 成员变量定义
private:

// 成员函数定义
public:
    // 单步运动规划函数：从当前位置 --> 目标位置(绝对位置)----> point to point 点动动作
    // terminalPos 数组：x y z wx wy wz
    // jointValueCur  数组：x y th theta1 theta2 theta3 theta4 theta5 theta6
    // jointValueObj  数组：同上
    int inverseFun(/*in*/double* terminalPos, double* jointValueCur, /*out*/double* jointValueObj);

    // 直线插补规划函数：从当前位置 --> 目标位置(规划位置)----> straight line 直线轨迹
    // deltaTerminalPos 数组：delta_x  delta_y  delta_z  delta_wx  delta_wy  delta_wz
    // jointValueArray 规划结果序列  矩阵: 100*9
    int lineInterpolationFun(/*in*/double* deltaTerminalPos, double* jointValueCur, /*out*/double* jointValueArray);
};

#endif // KINEMATICSOLUTION_H
