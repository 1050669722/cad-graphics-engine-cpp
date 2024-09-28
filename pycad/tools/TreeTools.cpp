//
// Created by liuxingbo03 on 2021/10/22.
//


#include <iostream>

#include "pycad/tools/JsonTools.h"
#include "pycad/tools/TreeTools.h"
#include "pycad/Utils.h"


using namespace std;


// ***计算仿射变换矩阵*******************************************************************************
bool TreeTools::isEqual(const double& a, const double& b, const double& threshold) {
    return Utils::isEqual(a, b, threshold);
}


void TreeTools::resetAffineMat(MatrixXd &AffineMat) {
    AffineMat = MatrixXd::Identity(3, 3);
}


void TreeTools::generateAffineMat(
        MatrixXd& AffineMat,
        map<string, vector<double>>& scaleFactors,
        vector<double>& rotations,
        map<string, vector<double>>& positions,
        map<string, vector<double>>& normals) {
    /*
     * 计算仿射变换矩阵
     */
    // 重置this->AffineMat
    TreeTools::resetAffineMat(AffineMat);

    // 检查形状 //TODO 这里应该是复数 是列表scaleFactors
    // for-test
    // unsigned long long length = scaleFactors["x"].size();
    auto length = scaleFactors["x"].size();
    // for-test
    // printf("length bytes numner: %d\n", sizeof(length));
    // int length = 0; //TODO: unsigned long long转换为int
    // for (auto& it : scaleFactors["x"]) { //TODO: 如何优雅地获取一个vector的长度
    //     length++;
    // }
    assert(!scaleFactors.empty());
    assert((length == positions["x"].size())
           && (length == rotations.size())
           && (length == normals["z"].size()));

    // 累积仿射变换矩阵
    // for (auto m_idx = length - 1; m_idx >= 0; --m_idx) {
    // for (auto m_idx = (int)(length - 1); m_idx >= 0; --m_idx) { //不正确的写法：(auto m_idx = length - 1; m_idx > -1; --m_idx)
    for (auto m_idx = (int)(length - 1); m_idx > -1; --m_idx) { //-1, --等，就不应该遇到usigned
    // for (int m_idx = length - 1; m_idx > -1; --m_idx) { //-1, --等，就不应该遇到usigned
        // 镜像因子 normalZ //将normal的Z分量乘在ScaleFactor的X分量上，并且也将旋转rotation与normal的Z分量相乘
        double& normalZ = normals["z"][m_idx];
        // for-test
        // cout << "length - 1: " << length - 1 << endl;
        // cout << "(int)(length - 1): " << (int)(length - 1) << endl;
        // printf("m_idx: %d\n", m_idx);
        // printf("m_idx: %l\n", m_idx);
        // printf("normalZ: %lf\n", normalZ);
        // cout << endl;
        // assert((TreeTools::isEqual(normalZ, 1.0)) || (TreeTools::isEqual(normalZ, -1.0)));
        if (not ((TreeTools::isEqual(normalZ, 1.0)) || (TreeTools::isEqual(normalZ, -1.0)))) {
            printf("[WARNING] Expect normalZ is equal to 1.0 or -1.0, but got %lf\n", normalZ);
        }

        // 本轮的 仿射变换 矩阵
        MatrixXd affineMat = MatrixXd::Identity(3, 3);
        Utils::generateAffineMat(
                affineMat,
                normalZ * scaleFactors["x"][m_idx],
                scaleFactors["y"][m_idx],
                normalZ * rotations[m_idx],
                positions["x"][m_idx],
                positions["y"][m_idx]
                );

        // 积累 仿射变换矩阵
        AffineMat = affineMat * AffineMat;
    }

}
// ***********************************************************************************************


// *** 仿射变换 ***********************************************************************************
void TreeTools::blockOrientedAffine(Atom &atom_x, Atom &atom_y, MatrixXd &affineMat) {
    /*
     * 面向BLock的矩阵计算坐标
     */
    Utils::atomOrientedAffine(atom_x, atom_y, affineMat);
}
// ***********************************************************************************************
