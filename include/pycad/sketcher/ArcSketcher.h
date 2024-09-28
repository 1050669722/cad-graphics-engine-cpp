//
// Created by liuxingbo03 on 2021/11/8.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ARCSKETCHER_H
#define ATUOCADDATAANALYSISPYCADCPP_ARCSKETCHER_H


/*
 * 该文件 对应于 python版本中的 abstractArc.py
 */


#include "pycad/Structs.h"


class ArcSketcher {
private:
    // 成员属性 //TODO: 这些属性应该尽可能修改为指针 //TODO: 为什么这里替换pointA, pointB, center中的任意一个为指针类型，都会报错：进程已结束，退出代码为 -1073741819 (0xC0000005)
    map<string, double> pointA; //{("x", 0.0), ("y", 0.0)};
    map<string, double> pointB; //{("x", 0.0), ("y", 0.0)};
    map<string, double> center; //{("x", 0.0), ("y", 0.0)};
    vector<double> Rad;
    double radius{};

    // 成员方法
    void getPoint(
                    map<string, double>& startPoint,
                    map<string, double>& endPoint,
                    map<string, double>& center_
                    );
    void getPoint(
                    vector<double>& startPoint,
                    vector<double>& endPoint,
                    vector<double>& center_
                    );
    void getRad(
                    bool& closed,
                    double& normalZ,
                    int& fineLevel
                    );
    void getAtom();
    static bool isEqual(const double& a, const double& b, const double& threshold=1e-2);

public:
    // 成员属性
    Atom atom_x;
    Atom atom_y;

    // 成员方法
    ArcSketcher();
    explicit ArcSketcher(
                            map<string, double>& startPoint,
                            map<string, double>& endPoint,
                            map<string, double>& center_,
                            double& radius_,
                            bool& closed,
                            double& normalZ,
                            int fineLevel
                            );
    explicit ArcSketcher(
                            vector<double>& startPoint,
                            vector<double>& endPoint,
                            vector<double>& center_,
                            double& radius_,
                            bool closed,
                            double normalZ,
                            int fineLevel
                            );
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ARCSKETCHER_H
