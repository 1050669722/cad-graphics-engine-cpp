//
// Created by liuxingbo03 on 2021/11/8.
//

#include <cmath>
#include <cassert>

#include "pycad/Utils.h"
#include "pycad/sketcher/ArcSketcher.h"
#include "pycad/tools/SketcherTools.h"


ArcSketcher::ArcSketcher() = default;


ArcSketcher::ArcSketcher(
                            map<string, double>& startPoint,
                            map<string, double>& endPoint,
                            map<string, double>& center_,
                            double &radius_,
                            bool &closed,
                            double &normalZ,
                            int fineLevel
                            ) {
    // 获取 半径
    radius = radius_;
    // 获取 起点、终点、圆心
    getPoint(startPoint, endPoint, center_);
    // 获取 弧度值向量
    getRad(closed, normalZ, fineLevel);
    // 获取 坐标原子
    getAtom();
}


ArcSketcher::ArcSketcher(
        vector<double> &startPoint,
        vector<double> &endPoint,
        vector<double> &center_,
        double &radius_,
        bool closed,
        double normalZ,
        int fineLevel
        ) {
    // 获取 半径
    radius = radius_;
    // 获取 起点、终点、圆心
    getPoint(startPoint, endPoint, center_);
    // 获取 弧度值向量
    getRad(closed, normalZ, fineLevel);
    // 获取 坐标原子
    getAtom();
}


void ArcSketcher::getPoint(
                            map<string, double>& startPoint,
                            map<string, double>& endPoint,
                            map<string, double>& center_
                            ) {
    /*
     * 获取 起点、终点、圆心
     */
    pointA = startPoint;
    pointB = endPoint;
    center = center_;
}


void ArcSketcher::getPoint(
                            vector<double> &startPoint,
                            vector<double> &endPoint,
                            vector<double> &center_
                            ) {
    /*
     * 获取 起点、终点、圆心
     */
    pointA["x"] = startPoint[0],  pointA["y"] = startPoint[1];
    pointB["x"] = endPoint[0],  pointB["y"] = endPoint[1];
    center["x"] = center_[0],  center["y"] = center_[1];
}


void ArcSketcher::getRad(
                            bool& closed,
                            double& normalZ,
                            int& fineLevel
                            ) {
    /*
     * 获取 弧度值向量
     */
    // 根据 normalZ 判断 是否应该交换 starpoint 和 endpoint
    // assert((ArcSketcher::isEqual(normalZ, 1.0)) || (ArcSketcher::isEqual(normalZ, -1.0)));
    if (not ((ArcSketcher::isEqual(normalZ, 1.0)) || (ArcSketcher::isEqual(normalZ, -1.0)))) {
        printf("[WARNING] Expect normalZ is equal to 1.0 or -1.0, but got %lf\n", normalZ);
    }
    if (normalZ < 0.0) {
        pointA.swap(pointB);
    }

    double alpha = SketcherTools::point2Angle(pointA, center);
    double beta = SketcherTools::point2Angle(pointB, center);

    // 弧度起始点 转换为 一系列分割点
    SketcherTools::startAngleEndAngle2SplitRad(Rad, alpha, beta, closed, fineLevel);
}


void ArcSketcher::getAtom() {
    /*
     * 获取 坐标向量
     */
    for (auto& rad : Rad) {
        atom_x.push_back(radius * cos(rad) + center["x"]);
        atom_y.push_back(radius * sin(rad) + center["y"]);
    }
}


bool ArcSketcher::isEqual(const double& a, const double& b, const double& threshold) {
    return Utils::isEqual(a, b, threshold);
}
