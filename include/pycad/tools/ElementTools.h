//
// Created by liuxingbo03 on 2021/10/26.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ELEMENTTOOLS_H
#define ATUOCADDATAANALYSISPYCADCPP_ELEMENTTOOLS_H


#include <Eigen/Dense>

#include "pycad/Structs.h"


using namespace Eigen;


class ElementTools {
public:
    static void atomOrientedAffine(Atom& atom_x, Atom& atom_y, MatrixXd& affineMat);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_ELEMENTTOOLS_H
