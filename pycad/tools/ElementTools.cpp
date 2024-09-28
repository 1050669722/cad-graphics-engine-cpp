//
// Created by liuxingbo03 on 2021/10/26.
//

#include "pycad/tools/ElementTools.h"
#include "pycad/Utils.h"


void ElementTools::atomOrientedAffine(Atom &atom_x, Atom &atom_y, MatrixXd &affineMat) {
    /*
     * 面向Atom的矩阵计算坐标
     * TODO: 是否可以实现地址传递，避免值拷贝？
     */
    // // 坐标矩阵 //TODO 能否构造出利用ElementTools::fillTransMat的逻辑？ //TODO: 考虑删除
    // auto atom_size = atom_x.size();
    // VectorXd vec_x = Map<VectorXd>(atom_x.data(), (Index)atom_size);
    // VectorXd vec_y = Map<VectorXd>(atom_y.data(), (Index)atom_size);
    // VectorXd vec_ones = VectorXd::Ones((Index)atom_size);
    // MatrixXd coordMat = MatrixXd::Zero((Index)atom_size, 3);
    // coordMat << vec_x, vec_y, vec_ones;
    // coordMat.transposeInPlace();
    //
    // // 仿射变换
    // coordMat = AffineMat * coordMat;
    //
    // // 转换回来
    // coordMat.transposeInPlace();
    // atom_x = Atom(coordMat.col(0).data(), coordMat.col(0).data() + coordMat.col(0).size());
    // atom_y = Atom(coordMat.col(1).data(), coordMat.col(1).data() + coordMat.col(1).size());

    Utils::atomOrientedAffine(atom_x, atom_y, affineMat);
}
