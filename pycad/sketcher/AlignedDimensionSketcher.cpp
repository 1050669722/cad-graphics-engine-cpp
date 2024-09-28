//
// Created by liuxingbo03 on 2021/12/27.
//

#include "pycad//Utils.h"
#include "pycad/sketcher/AlignedDimensionSketcher.h"


AlignedDimensionSketcher::AlignedDimensionSketcher() = default;


AlignedDimensionSketcher::AlignedDimensionSketcher(
        map<string, double> &dimLinePoint,
        map<string, double> &xLine1Point,
        map<string, double> &xLine2Point
        ) {
    // 获取坐标原子
    getAtom(dimLinePoint, xLine1Point, xLine2Point);
}


void AlignedDimensionSketcher::getAtom(
        map<string, double> &dp,
        map<string, double> &xp1,
        map<string, double> &xp2
        ) {
    // ”控制点“的横纵坐标
    double x1 = xp1["x"], y1 = xp1["y"];
    double x2 = xp2["x"], y2 = xp2["y"];
    double dp1_x, dp1_y, dp2_x, dp2_y;

    // 竖直
    if (isEqual(x1, x2)) {
        dp1_x = dp["x"];
        dp1_y = y1;
        dp2_x = dp["x"];
        dp2_y = y2;
    }

    // 水平
    else if (isEqual(y1, y2)) {
        dp1_x = x1;
        dp1_y = dp["y"];
        dp2_x = x2;
        dp2_y = dp["y"];
    }

    // 一般情况
    else {
        double k0 = (y2 - y1) / (x2 - x1);
        dp1_x = (x1 + pow(k0, 2) * dp["x"] + k0 * y1 - k0 * dp["y"]) / (pow(k0, 2) + 1);
        dp1_y = k0 * (dp1_x - dp["x"]) + dp["y"];
        dp2_x = (x2 + pow(k0, 2) * dp["x"] + k0 * y2 - k0 * dp["y"]) / (pow(k0, 2) + 1);
        dp2_y = k0 * (dp2_x - dp["x"]) + dp["y"];
    }

    // 搜集坐标
    atom_x = Atom{x1, dp1_x, dp2_x, x2};
    atom_y = Atom{y1, dp1_y, dp2_y, y2};
}


bool AlignedDimensionSketcher::isEqual(const double &a, const double &b, const double &threshold) {
    return Utils::isEqual(a, b, threshold);
}

