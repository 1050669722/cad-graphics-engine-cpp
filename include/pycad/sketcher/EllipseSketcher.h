//
// Created by liuxingbo03 on 2021/11/15.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ELLIPSESKETCHER_H
#define ATUOCADDATAANALYSISPYCADCPP_ELLIPSESKETCHER_H


#include "pycad/Structs.h"


class EllipseSketcher {
private:
    // 成员属性
    double startAngle{}; //TODO: 这两个属性是没有用的属性
    double endAngle{}; //TODO: 考虑删除
    map<string, double>* majorAxis = nullptr;
    map<string, double>* minorAxis = nullptr;
    map<string, double>* center = nullptr;
    vector<double> Rad;
    vector<double> Rho;
    double semiMajorAxisLength{};
    double semiMinorAxisLength{};

    // 成员方法
    void getMajorAxisAndMinorAxis(
                                    map<string, double>& majorAxis_,
                                    map<string, double>& minorAxis_
                                    );
    void getRho();
    void getRad(
                    bool& closed,
                    double& normalZ,
                    int& fineLevel
                    );
    // void convertAtomIntoNewCoordinateSpace();
    void getAtom();
    static bool isEqual(const double& a, const double& b, const double& threshold=1e-2);

public:
    // 成员属性
    Atom atom_x;
    Atom atom_y;

    // 成员方法
    EllipseSketcher();
    explicit EllipseSketcher(
                                double& startAngle_,
                                double& endAngle_,
                                map<string, double>& center_,
                                map<string, double>& majorAxis,
                                map<string, double>& minorAxis,
                                bool& closed,
                                double& normalZ,
                                int fineLevel
                                );
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ELLIPSESKETCHER_H
