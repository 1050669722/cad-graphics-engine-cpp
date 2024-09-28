//
// Created by liuxingbo03 on 2021/11/15.
//

#include <cmath>

#include "pycad/tools/SketcherTools.h"


void SketcherTools::startAngleEndAngle2SplitRad(
        vector<double> &container,
        double &startAngle,
        double &endAngle,
        bool closed,
        int &fineLevel
        ) {
    /*
     * 弧度起始点 转换为 一系列分割点
     */
    // 确保 endAngle 大于 startAngle
    while (startAngle >= endAngle) endAngle += 2 * M_PI;

    // 分割点数目
    int num = max((int)(fineLevel * (abs(endAngle - startAngle) / (2 * M_PI))), (int)(fineLevel / 2)); //数目不能低于缺省数目的一半

    // 弧度分割点
    Utils::linspace(container, startAngle, endAngle, num);
    if (closed) container.push_back(startAngle);
}


double SketcherTools::calculateDistanceBetweenTwoPoints(vector<double> &pt0, vector<double> &pt1) {
    return Utils::calculateDistanceBetweenTwoPoints(pt0, pt1);
}


double SketcherTools::calculateDistanceBetweenTwoPoints(map<string, double> &pt0, map<string, double> &pt1) {
    vector<double> point0{pt0["x"], pt0["y"]}, point1{pt1["x"], pt1["y"]};
    return calculateDistanceBetweenTwoPoints(point0, point1);
}


void SketcherTools::atomOrientedAffine(Atom &atom_x, Atom &atom_y, MatrixXd &affineMat) {
    Utils::atomOrientedAffine(atom_x, atom_y, affineMat);
}


void SketcherTools::restrict(double &value) {
    Utils::restrict(value, -1.0, 1.0);
}


double SketcherTools::point2Angle(map<string, double> &point, map<string, double> &center) {
    // 计算 半径
    double radius = calculateDistanceBetweenTwoPoints(point, center);

    // 计算 余弦值 正弦值
    double cosTheta = (point["x"] - center["x"]) / max(radius, 1e-6);
    double sinTheta = (point["y"] - center["y"]) / max(radius, 1e-6);
    restrict(cosTheta);
    restrict(sinTheta);

    // 计算弧度 判断 优弧 或 劣弧
    double theta = acos(cosTheta); //[0, pi]
    theta = sinTheta >= 0.0 ? theta : -1.0 * theta; //[-pi, pi]
    theta = theta >= 0.0 ? theta : 2 * M_PI + theta; //[pi, 3pi] //2?

    // 返回
    return theta;
}


