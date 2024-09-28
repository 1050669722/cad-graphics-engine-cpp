//
// Created by liuxingbo03 on 2021/11/15.
//

#include <iostream> //TODO: 考虑删除
#include <cmath>
#include <cassert>

#include "pycad/Utils.h"
#include "pycad/sketcher/EllipseSketcher.h"
#include "pycad/tools/SketcherTools.h"


using namespace std; //TODO: 考虑删除


EllipseSketcher::EllipseSketcher() = default;


EllipseSketcher::EllipseSketcher(
                                    double &startAngle_,
                                    double &endAngle_,
                                    map<string, double> &center_,
                                    map<string, double> &majorAxis,
                                    map<string, double> &minorAxis,
                                    bool &closed,
                                    double &normalZ,
                                    int fineLevel
                                    ) {
    // 获取 椭圆的 起始弧度 终止弧度 中心
    startAngle = startAngle_;
    endAngle = endAngle_;
    center = &center_;
    // 获取 椭圆的 半长轴长 和 半短轴长 以及 中心
    getMajorAxisAndMinorAxis(majorAxis, minorAxis);
    // 获取 弧度值向量Rad
    getRad(closed, normalZ, fineLevel);
    // 获取 Rho
    getRho();
    // 获取 坐标原子
    getAtom();
}


void EllipseSketcher::getMajorAxisAndMinorAxis(
                                                map<string, double> &majorAxis_,
                                                map<string, double> &minorAxis_
                                                ) {
    /*
     * 获取 椭圆的 半长轴长 和 半短轴长 以及 中心
     * 根据json的规则，这里以原点(0, 0)为基准
     */
    majorAxis = &majorAxis_;
    minorAxis = &minorAxis_;
    semiMajorAxisLength = sqrt(pow(majorAxis_["x"], 2.0) + pow(majorAxis_["y"], 2.0));
    semiMinorAxisLength = sqrt(pow(minorAxis_["x"], 2.0) + pow(minorAxis_["y"], 2.0));
    // double a = max(semiMajorAxisLength, semiMinorAxisLength); //TODO: 这里是与python版本保持了一致
    // double b = min(semiMajorAxisLength, semiMinorAxisLength);
    // semiMajorAxisLength = a;
    // semiMinorAxisLength = b;
    if ((*majorAxis)["x"] < (*majorAxis)["y"]) { //TODO: 这里是与python版本保持了一致
        double tmp = semiMajorAxisLength;
        semiMajorAxisLength = semiMinorAxisLength;
        semiMinorAxisLength = tmp;
    }
}


void EllipseSketcher::getRad(
                                bool& closed,
                                double& normalZ,
                                int& fineLevel
                                ) {
    /*
     * 获取 弧度值向量
     */
    // // 根据 normalZ 判断 是否应该交换 startAngle 和 endAngle //这一步骤是否真地需要？
    // assert((EllipseSketcher::isEqual(normalZ, 1.0)) || (EllipseSketcher::isEqual(normalZ, -1.0)));
    if (not ((EllipseSketcher::isEqual(normalZ, 1.0)) || (EllipseSketcher::isEqual(normalZ, -1.0)))) {
        printf("[WARNING] Expect normalZ is equal to 1.0 or -1.0, but got %lf\n", normalZ);
    }
    // if (normalZ < 0.0) { //TODO: 这里是与python版本保持了一致
    //     // swap<double>(startAngle, endAngle); // TODO: 考虑删除 //为什么不能这样写? Utils::swap<double>(startAngle, endAngle);
    //     (*majorAxis).swap(*minorAxis);
    // }

    // // 由 点 到 弧度
    // double alpha = SketcherTools::point2Angle(*majorAxis, *center);
    // double beta = SketcherTools::point2Angle(*minorAxis, *center);

    // 弧度起始点 转换为 一系列分割点
    SketcherTools::startAngleEndAngle2SplitRad(Rad, startAngle, endAngle, closed, fineLevel); // TODO: 考虑删除 //TODO: 这里是与python版本保持了一致
    // SketcherTools::startAngleEndAngle2SplitRad(Rad, alpha, beta, closed, fineLevel); //TODO: 这里是与python版本保持了一致
    // cout << "alpha: " << alpha << ", beta: " << beta << endl; //TODO: 考虑删除
}


void EllipseSketcher::getRho() {
    /*
     * 获取 Rho
     */
    for (auto& rad : Rad) {
        Rho.push_back( //TODO: 能否用矩阵或者向量实现呢？
                (semiMajorAxisLength * semiMinorAxisLength) / sqrt(pow((semiMajorAxisLength * sin(rad)), 2.0) + pow((semiMinorAxisLength * cos(rad)), 2.0))
                );
    }
}


// TODO: 考虑删除
// void EllipseSketcher::convertAtomIntoNewCoordinateSpace() {
//     // 旧坐标 应缩放的 倍率
//     double scaleFactor_x = signbit((*majorAxis)["x"]) ? -1.0 : 1.0;
//     double scaleFactor_y = signbit((*majorAxis)["y"]) ? -1.0 : 1.0;
//     scaleFactor_x = 1.0 / scaleFactor_x;
//     scaleFactor_y = 1.0 / scaleFactor_y;
//
//     // 旧坐标 应旋转的 弧度角
//     Vector2d majorV((*majorAxis)["x"], (*majorAxis)["y"]);
//     Vector2d originHorizontalAxis(1.0, 0.0);
//     double cosRotation = majorV.normalized().dot(originHorizontalAxis.normalized());
//     double rotation = signbit((*majorAxis)["y"]) ? -1.0 * acos(cosRotation) : acos(cosRotation);
//     rotation *= -1.0;
//
//     // 旧坐标 应位移 向量
//     double displace_x = 0.0;
//     double displace_y = 0.0;
//     displace_x *= -1.0;
//     displace_y *= -1.0;
//
//     // 旧坐标 应乘的 变换矩阵
//     MatrixXd scaleMat = MatrixXd::Identity(3, 3);
//     // SketcherTools::generateScaleMat(scaleMat, scaleFactor_x, scaleFactor_y);
//     SketcherTools::generateScaleMat(scaleMat, 1.0, 1.0);
//     MatrixXd rotateMat = MatrixXd::Identity(3, 3);
//     // SketcherTools::generateRotateMat(rotateMat, rotation);
//     SketcherTools::generateRotateMat(rotateMat, 0.0);
//     MatrixXd displaceMat = MatrixXd::Identity(3, 3);
//     SketcherTools::generateDisplaceMat(displaceMat, displace_x, displace_y);
//     MatrixXd affineMat = displaceMat * rotateMat * scaleMat;
//
//     // 变换
//     SketcherTools::atomOrientedAffine(atom_x, atom_y, affineMat);
// }


void EllipseSketcher::getAtom() {
    /*
     * 获取 坐标向量
     */
    for (int i = 0; i < Rad.size(); ++i) {
        double rad = Rad[i];
        double rho = Rho[i];
        atom_x.push_back(rho * cos(rad) + (*center)["x"]);
        atom_y.push_back(rho * sin(rad) + (*center)["y"]);
    }

    // TODO: 考虑删除
    // // 此时的atom_x, atom_y是当坐标轴为(1, 0), (0, 1)时的，而majorAxis, minorAxis指示出了椭圆会在一个新的坐标系下 //所以需要转换
    // convertAtomIntoNewCoordinateSpace();
}


bool EllipseSketcher::isEqual(const double &a, const double &b, const double &threshold) {
    return Utils::isEqual(a, b, threshold);
}
