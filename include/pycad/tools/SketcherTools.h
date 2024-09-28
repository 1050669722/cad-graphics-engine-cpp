//
// Created by liuxingbo03 on 2021/11/15.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_SKETCHERTOOLS_H
#define ATUOCADDATAANALYSISPYCADCPP_SKETCHERTOOLS_H


#include "pycad/Utils.h"


class SketcherTools {
public:
    // 成员方法
    // 起止弧度 转换为 分割弧度
    static void startAngleEndAngle2SplitRad(
            vector<double> &container,
            double &startAngle,
            double &endAngle,
            bool closed,
            int& fineLevel
            );

    // 计算两点之间的距离
    static double calculateDistanceBetweenTwoPoints(vector<double>& pt0, vector<double>& pt1);
    static double calculateDistanceBetweenTwoPoints(map<string, double>& pt0, map<string, double>& pt1);

    // 面向Atom的仿射变换
    static void atomOrientedAffine(Atom& atom_x, Atom& atom_y, MatrixXd& affineMat);

    // 限制一个数的范围 in-place
    static void restrict(double& value);

    // 由 点 获取 弧度角
    static double point2Angle(map<string, double>& point, map<string, double>& center);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_SKETCHERTOOLS_H
