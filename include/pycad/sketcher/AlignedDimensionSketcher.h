//
// Created by liuxingbo03 on 2021/12/27.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ALIGNEDDIMENSIONSKETCHER_H
#define ATUOCADDATAANALYSISPYCADCPP_ALIGNEDDIMENSIONSKETCHER_H


#include "pycad/Structs.h"


class AlignedDimensionSketcher {
private:
    // 成员属性

    // 成员方法
    void getAtom(
            map<string, double>& dp,
            map<string, double>& xp1,
            map<string, double>& xp2
            );
    static bool isEqual(const double& a, const double& b, const double& threshold=1e-2);

public:
    // 成员属性
    Atom atom_x;
    Atom atom_y;

    // 成员方法
    AlignedDimensionSketcher();
    explicit AlignedDimensionSketcher(
            map<string, double>& dimLinePoint,
            map<string, double>& xLine1Point,
            map<string, double>& xLine2Point
            );
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ALIGNEDDIMENSIONSKETCHER_H
