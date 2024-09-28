//
// Created by liuxingbo03 on 2021/10/22.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_TREETOOLS_H
#define ATUOCADDATAANALYSISPYCADCPP_TREETOOLS_H


#include "pycad/Structs.h"
#include "pycad/parse/BLock.h"


class TreeTools {
private:
    // 成员属性

    // 成员方法

public:
    // 成员方法
    // 计算仿射变换矩阵
    static bool isEqual(const double& a, const double& b, const double& threshold=1e-2);
    static void resetAffineMat(MatrixXd& AffineMat);
    static void generateAffineMat(
                                    MatrixXd& AffineMat,
                                    map<string, vector<double>>& scaleFactors,
                                    vector<double>& rotations,
                                    map<string, vector<double>>& positions,
                                    map<string, vector<double>>& normals
                                );
    // 仿射变换
    static void blockOrientedAffine(Atom& atom_x, Atom& atom_y, MatrixXd& affineMat);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_TREETOOLS_H
