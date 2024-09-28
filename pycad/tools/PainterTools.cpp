//
// Created by liuxingbo03 on 2021/10/29.
//

#include <cmath>
#include <vector>
#include <algorithm>

#include "pycad/tools/PainterTools.h"
#include "pycad/Utils.h"


using namespace std;


PainterTools::PainterTools() = default;


bool PainterTools::isEqual(double& a, double& b, const double& threshold) {
    return Utils::isEqual(a, b, threshold);
}


double PainterTools::calculateDistanceBetweenTwoPoints(Atom& pt0, Atom& pt1) {
    return Utils::calculateDistanceBetweenTwoPoints(pt0, pt1);
}


double PainterTools::calculateAreaOfTriangle(Atom& p0, Atom& p1, Atom& p2) {
    double a = calculateDistanceBetweenTwoPoints(p0, p1);
    double b = calculateDistanceBetweenTwoPoints(p0, p2);
    double c = calculateDistanceBetweenTwoPoints(p1, p2);
    double p = (a + b + c) / 2.0;
    return sqrt(abs(p * (p - a) * (p - b) * (p - c)));
}


double PainterTools::calculateAreaOfPolygon(Atom& atom_x, Atom& atom_y) {
    return Utils::calculateAreaOfPolygon(atom_x, atom_y);
}


vector<double> PainterTools::getMaxAreaLBRT(Database &database) {
    /*
     * 找到最大面积的矩形
     */
    // 最大面积
    double max_area = 0.0;
    Atom* max_area_atom_x;
    Atom* max_area_atom_y;

    for (int i = 0; i < database.polylines.ori_coords_x.size(); ++i) {
        // 每一个polyline
        Coord& ori_coord_x = database.polylines.ori_coords_x[i];
        Coord& ori_coord_y = database.polylines.ori_coords_y[i];

        for (int j = 0; j < ori_coord_x.size(); ++j) {
            // 每一个atom //ori - 没有经过梯形化
            Atom& atom_x = ori_coord_x[j];
            Atom& atom_y = ori_coord_y[j];
            // 判断为封闭图形
            if (PainterTools::isEqual(atom_x[atom_x.size() - 1], atom_x[0])
            && PainterTools::isEqual(atom_y[atom_y.size() - 1], atom_y[0])) {
                // 计算面积
                double cur_area = PainterTools::calculateAreaOfPolygon(atom_x, atom_y);
                // 与 已有 最大面积 比较
                if (max_area < cur_area) {
                    max_area = cur_area;
                    max_area_atom_x = &atom_x;
                    max_area_atom_y = &atom_y;
                }
            }

        }
    }

    // 最小坐标 最大坐标 //TODO: 需要添加assert或者raise，针对不存在polyline的情况
    double xmin = *min_element(max_area_atom_x->begin(), max_area_atom_x->end()); //TODO 可以优化写法吗？
    double ymin = *min_element(max_area_atom_y->begin(), max_area_atom_y->end());
    double xmax = *max_element(max_area_atom_x->begin(), max_area_atom_x->end());
    double ymax = *max_element(max_area_atom_y->begin(), max_area_atom_y->end());

    // 返回
    return vector<double>{xmin, ymin, xmax, ymax};
}


MatrixXd PainterTools::CAD2OpenCV(Configuration& config, Mat& screen, Atom& originalPoint) {
    // CAD两个最小横纵坐标
    double& cadxmin = originalPoint[0], cadymin = originalPoint[1];

    // 初始化
    MatrixXd restoreAffine = MatrixXd::Identity(3, 3); //这个函数使用频率较低，一张图才用两回，而且传递的值占用资源不大
    // restoreAffine(0, 1) = 1.0; //既有缩放又有旋转的时候再添加这两行 //所以就返回这个函数内部的值好了
    // restoreAffine(1, 0) = 1.0;

    // 比例缩放
    restoreAffine(0, 0) *= config.restoreScaleFactor; //sx
    restoreAffine(1, 1) *= config.restoreScaleFactor; //sy
    // restoreAffine(0, 1) *= self.config.restoreScaleFactor; //sy //既有缩放又有旋转的时候再添加这两行
    // restoreAffine(1, 0) *= self.config.restoreScaleFactor; //sx

    // 纵坐标翻折
    restoreAffine(1, 1) *= -1;
    restoreAffine(1, 2) += 0.0 + (0.0 + screen.rows); // -y + y0 = -y + top + bottom

    // 平移 左下角对齐
    Atom cadleft = Atom{cadxmin}, cadbottom = Atom{cadymin};
    atomOrientedAffine(cadleft, cadbottom, restoreAffine);
    restoreAffine(0, 2) += 0.0 - cadleft[0] + config.redundantBorder / 2.0;
    restoreAffine(1, 2) += (0.0 + screen.rows) - cadbottom[0] - config.redundantBorder / 2.0;

    return restoreAffine;
}


void PainterTools::atomOrientedAffine(Atom& atom_x, Atom& atom_y, MatrixXd& affineMat) {
    /*
        # 坐标
        coords_x = [100.123, 100,456, 16.0, ...]
        coords_y = [2345.890, 12345.567, 324627.867, ...]

        # 外部在构造_Affine的时候应该采用矩阵相乘的方式，即rotate x scale (注意，先做scale，再做rotate)
        # 下面给矩阵的元素赋值的做法仅适用于缩放和矩阵都存在有效值的情况
        # 矩阵
        _Affine = np.eye(3)
        _Affine[0][1] = 1.0
        _Affine[1][0] = 1.0
        # 缩放
        _Affine[0][0] *= scaleFactors["x"]
        _Affine[1][1] *= scaleFactors["y"]
        _Affine[0][1] *= scaleFactors["y"]
        _Affine[1][0] *= scaleFactors["x"]
        # 旋转
        _Affine[0][0] *= np.cos(rotation)
        _Affine[0][1] *= -np.sin(rotation)
        _Affine[1][0] *= np.sin(rotation)
        _Affine[1][1] *= np.cos(rotation)
        # 位移
        _Affine[0][2] = position["x"]
        _Affine[1][2] = position["y"]
     */
    Utils::atomOrientedAffine(atom_x, atom_y, affineMat);
}


void PainterTools::RGB2BGR() {

}


void PainterTools::BGR2RGB() {

}


void PainterTools::screen2Array() {

}
