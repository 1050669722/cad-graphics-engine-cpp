//
// Created by liuxingbo03 on 2021/11/16.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_POLYLINESKETCHER_H
#define ATUOCADDATAANALYSISPYCADCPP_POLYLINESKETCHER_H


#include "pycad/Structs.h"


class PolylineSketcher {
private:
    // 成员属性
    vector<double> pt1;
    double bulge{};
    double startWidth{};
    double endWidth{};
    vector<double> pt2;
    // 4个顶点坐标
    double x11{}, x12{}, x22{}, x21{};
    double y11{}, y12{}, y22{}, y21{};

    // 成员方法
    void getCornerOfTrapezoid(
            double& x1,
            double& y1,
            double& x2,
            double& y2,
            double& startWidth_,
            double& endWidth_
            );
    void trapezoid();
    void trapezoid(
            vector<double>& startPoint,
            vector<double>& endPoint,
            double &startWidth_,
            double &endWidth_
            );
    void bulge2Center(
            vector<double>& circleCenter,
            vector<double>& startPoint,
            vector<double>& endPoint
            ) const;
    vector<double> getCircleCenter();
    vector<double> getCircleCenter(
            vector<double>& startPoint,
            vector<double>& endPoint
            );
    static vector<double> pointRotateAroundCenter(
            vector<double>& point,
            vector<double>& center,
            const double& rotation
            );
    void arcTrapezoid();
    void getAtom();
    static void joinAtom(
            Atom& atom,
            double& startPointInner,
            double& startPointOuter,
            double& endPointInner,
            double& endPointOuter,
            Atom& arcInnerAtom,
            Atom& arcOuterAtom
            );
    void getAtom(
            vector<double>& startPointInner,
            vector<double>& startPointOuter,
            vector<double>& endPointInner,
            vector<double>& endPointOuter,
            Atom& arcInnerAtomX,
            Atom& arcInnerAtomY,
            Atom& arcOuterAtomX,
            Atom& arcOuterAtomY
            );
    static bool isEqual(const double& a, const double& b, const double& threshold=1e-2);

public:
    // 成员属性
    Atom atom_x;
    Atom atom_y;

    // 成员方法
    PolylineSketcher();
    explicit PolylineSketcher(
            map<string, double>& vertex1,
            map<string, double>& vertex2
            );

};


#endif //ATUOCADDATAANALYSISPYCADCPP_POLYLINESKETCHER_H
