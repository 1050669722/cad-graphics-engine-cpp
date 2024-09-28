//
// Created by liuxingbo03 on 2021/11/16.
//

#include <cmath>
#include <cassert>

#include <Eigen/Dense>
#include "pycad/sketcher/PolylineSketcher.h"
#include "pycad/sketcher/ArcSketcher.h"
#include "pycad/tools/SketcherTools.h"


using namespace Eigen;


PolylineSketcher::PolylineSketcher() = default;


PolylineSketcher::PolylineSketcher(
        map<string, double>& vertex1,
        map<string, double>& vertex2
        ) {
    // 初始化成员属性
    pt1 = vector<double>{vertex1["x"], vertex1["y"]};
    bulge = vertex1["bulge"];
    startWidth = vertex1["startWidth"];
    endWidth = vertex1["endWidth"];
    pt2 = vector<double>{vertex2["x"], vertex2["y"]};

    // 直边梯形
    if (PolylineSketcher::isEqual(bulge, 0.0)) {
        // 获取 直边梯形 以及 获取 坐标原子
        trapezoid();
    }

    // 曲边梯形
    else {
        // 获取 曲边梯形 以及 获取 坐标原子
        arcTrapezoid();
    }
}


void PolylineSketcher::getCornerOfTrapezoid(
        double &x1,
        double &y1,
        double &x2,
        double &y2,
        double &startWidth_,
        double &endWidth_
        ) {
    // 竖直
    if (PolylineSketcher::isEqual(x1, x2)) {
        x11 = x1 - startWidth_ / 2;
        x12 = x1 + startWidth_ / 2;
        y11 = y1;
        y12 = y1;

        x21 = x2 - endWidth_ / 2;
        x22 = x2 + endWidth_ / 2;
        y21 = y2;
        y22 = y2;
    }

    // 水平
    else if (PolylineSketcher::isEqual(y1, y2)) {
        x11 = x1;
        x12 = x1;
        y11 = y1 - startWidth_ / 2;
        y12 = y1 + startWidth_ / 2;

        x21 = x2;
        x22 = x2;
        y21 = y2 - endWidth_ / 2;
        y22 = y2 + endWidth_ / 2;
    }

    // 其它
    else {
        double k0 = (y2 - y1) / (x2 - x1);
        double k = -1.0 / k0;

        x11 = x1 - (startWidth_ / 2) / sqrt(pow(k, 2) + 1);
        x12 = x1 + (startWidth_ / 2) / sqrt(pow(k, 2) + 1);
        y11 = y1 - k * (x1 - x11); //负负得正
        y12 = y1 - k * (x1 - x12);

        x21 = x2 - (endWidth_ / 2) / sqrt(pow(k, 2) + 1);
        x22 = x2 + (endWidth_ / 2) / sqrt(pow(k, 2) + 1);
        y21 = y2 - k * (x2 - x21);
        y22 = y2 - k * (x2 - x22);
    }
}


void PolylineSketcher::trapezoid() {
    /*
     * 寻找 直边梯形的 4个顶点
     */
    // 起点
    double x1 = pt1[0];
    double y1 = pt1[1];

    // 终点
    double x2 = pt2[0];
    double y2 = pt2[1];

    // 获取 直边梯形的 4个顶点坐标
    getCornerOfTrapezoid(x1, y1, x2, y2, startWidth, endWidth);

    // 获取 坐标原子
    getAtom();
}


void PolylineSketcher::trapezoid(
        vector<double> &startPoint,
        vector<double> &endPoint,
        double &startWidth_,
        double &endWidth_
        ) {
    // 起点
    double x1 = startPoint[0];
    double y1 = startPoint[1];

    // 终点
    double x2 = endPoint[0];
    double y2 = endPoint[1];

    // 获取 直边梯形的 4个顶点坐标
    getCornerOfTrapezoid(x1, y1, x2, y2, startWidth_, endWidth_);
}


void PolylineSketcher::bulge2Center(
        vector<double>& circleCenter,
        vector<double>& startPoint,
        vector<double>& endPoint
        ) const {
    double x1 = startPoint[0], y1 = startPoint[1], x2 = endPoint[0], y2 = endPoint[1];
    double b = 0.5 * (1 / bulge - bulge);
    double x = 0.5 * ((x1 + x2) - b * (y2 - y1));
    double y = 0.5 * ((y1 + y2) + b * (x2 - x1));
    circleCenter.push_back(x);
    circleCenter.push_back(y);
}


vector<double> PolylineSketcher::getCircleCenter() {
    vector<double> circleCenter;
    bulge2Center(circleCenter, pt1, pt2);
    return circleCenter;
}


vector<double> PolylineSketcher::getCircleCenter(
        vector<double> &startPoint,
        vector<double> &endPoint
        ) {
    vector<double> circleCenter;
    bulge2Center(circleCenter, startPoint, endPoint);
    return circleCenter;
}


vector<double> PolylineSketcher::pointRotateAroundCenter(
        vector<double> &point,
        vector<double> &center,
        const double &rotation
        ) {
    // 获取坐标
    double x = point[0], y = point[1];
    double cx = center[0], cy = center[1];

    // 平移矩阵
    MatrixXd mat0 = MatrixXd::Identity(3, 3);
    Utils::generateDisplaceMat(mat0, -1.0 * cx, -1.0 * cy);

    // 旋转矩阵
    MatrixXd mat1 = MatrixXd::Identity(3, 3);
    Utils::generateRotateMat(mat1, rotation);

    // 平移矩阵
    MatrixXd mat2 = MatrixXd::Identity(3, 3);
    Utils::generateDisplaceMat(mat2, cx, cy);

    // 点乘
    VectorXd vecxy(3);
    vecxy(0) = x, vecxy(1) = y, vecxy(2) = 1.0;
    VectorXd resxy = mat2 * (mat1 * (mat0 * vecxy));

    // 返回
    vector<double> res;
    res.push_back(resxy(0));
    res.push_back(resxy(1));
    return res;
}


void PolylineSketcher::arcTrapezoid() {
    /*
     * 寻找 曲边梯形的 4个顶点
     */
    // 圆心
    vector<double> circleCenter = getCircleCenter();

    // 内起止点 外起止点
    vector<double> startPointInner, endPointInner, startPointOuter, endPointOuter;

    // radius with pt1 //将半径旋转至切线方向
    vector<double> circleCenter1 = pointRotateAroundCenter(circleCenter, pt1, M_PI / 2.0);
    trapezoid(circleCenter1, pt1, startWidth, startWidth);
    startPointOuter.push_back(x22), startPointOuter.push_back(y22);
    startPointInner.push_back(x21), startPointInner.push_back(y21);

    // radius with pt2 //将半径旋转至切线方向
    vector<double> circleCenter2 = pointRotateAroundCenter(circleCenter, pt2, M_PI / 2.0);
    trapezoid(circleCenter2, pt2, endWidth, endWidth);
    endPointOuter.push_back(x22), endPointOuter.push_back(y22);
    endPointInner.push_back(x21), endPointInner.push_back(y21);

    // 判定 inner, outer
    if (SketcherTools::calculateDistanceBetweenTwoPoints(circleCenter, startPointInner) >
    SketcherTools::calculateDistanceBetweenTwoPoints(circleCenter, startPointOuter)) {
        startPointInner.swap(startPointOuter);
    }
    if (SketcherTools::calculateDistanceBetweenTwoPoints(circleCenter, endPointInner) >
    SketcherTools::calculateDistanceBetweenTwoPoints(circleCenter, endPointOuter)) {
        endPointInner.swap(endPointOuter);
    }

    // 内外圆心
    vector<double> circleCenterInner = getCircleCenter(startPointInner, endPointInner);
    vector<double> circleCenterOuter = getCircleCenter(startPointOuter, endPointOuter);

    // 半径
    double radiusInner = SketcherTools::calculateDistanceBetweenTwoPoints(circleCenterInner, startPointInner);
    double radiusOuter = SketcherTools::calculateDistanceBetweenTwoPoints(circleCenterOuter, startPointOuter);

    // 弧
    if (bulge < 0.0) {
        startPointInner.swap(endPointInner);
        startPointOuter.swap(endPointOuter);
    }
    // arcInnerAtom
    ArcSketcher arcSketcherInner = ArcSketcher(
            startPointInner,
            endPointInner,
            circleCenterInner,
            radiusInner,
            false,
            1.0,
            120
    );
    // arcOuterAtom
    ArcSketcher arcSketcherOuter = ArcSketcher(
            startPointOuter,
            endPointOuter,
            circleCenterOuter,
            radiusOuter,
            false,
            1.0,
            120
    );

    // getAtom
    getAtom(
            startPointInner,
            startPointOuter,
            endPointInner,
            endPointOuter,
            arcSketcherInner.atom_x,
            arcSketcherInner.atom_y,
            arcSketcherOuter.atom_x,
            arcSketcherOuter.atom_y
            );
}


void PolylineSketcher::getAtom() {
    /*
     * 获取 坐标向量
     */
    // atom_x
    atom_x.push_back(x11);
    atom_x.push_back(x12);
    atom_x.push_back(x22);
    atom_x.push_back(x21);
    atom_x.push_back(x11);
    // atom_y
    atom_y.push_back(y11);
    atom_y.push_back(y12);
    atom_y.push_back(y22);
    atom_y.push_back(y21);
    atom_y.push_back(y11);
}


void PolylineSketcher::joinAtom(
        Atom &atom,
        double &startInner,
        double &startOuter,
        double &endInner,
        double &endOuter,
        Atom &arcInnerAtom,
        Atom &arcOuterAtom
        ) {
    atom.push_back(startInner);
    atom.insert(atom.end(), arcInnerAtom.begin(), arcInnerAtom.end());
    atom.push_back(endInner);
    atom.push_back(endOuter);
    atom.insert(atom.end(), arcOuterAtom.rbegin(), arcOuterAtom.rend());
    atom.push_back(startOuter);
    atom.push_back(startInner);
}


void PolylineSketcher::getAtom(
        vector<double> &startPointInner,
        vector<double> &startPointOuter,
        vector<double> &endPointInner,
        vector<double> &endPointOuter,
        Atom &arcInnerAtomX,
        Atom &arcInnerAtomY,
        Atom &arcOuterAtomX,
        Atom &arcOuterAtomY
        ) {
    // atom_x
    joinAtom(
            atom_x,
            startPointInner[0],
            startPointOuter[0],
            endPointInner[0],
            endPointOuter[0],
            arcInnerAtomX,
            arcOuterAtomX
            );
    // atom_y
    joinAtom(
            atom_y,
            startPointInner[1],
            startPointOuter[1],
            endPointInner[1],
            endPointOuter[1],
            arcInnerAtomY,
            arcOuterAtomY
    );
}


bool PolylineSketcher::isEqual(const double &a, const double &b, const double &threshold) {
    return Utils::isEqual(a, b, threshold);
}
