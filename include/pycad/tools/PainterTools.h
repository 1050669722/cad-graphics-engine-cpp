//
// Created by liuxingbo03 on 2021/10/29.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_PAINTERTOOLS_H
#define ATUOCADDATAANALYSISPYCADCPP_PAINTERTOOLS_H


#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

#include "pycad/Structs.h" //须在#include "pycad/Configuration.h"之前
#include "pycad/Configuration.h"
#include "pycad/Database.h"


using namespace std;
using namespace cv;
using namespace Eigen;


class PainterTools {
private:
    // 成员属性

    // 成员方法


public:
    // 成员属性

    // 成员方法
    PainterTools();
    static bool isEqual(double& a, double& b, const double& threshold=1e-2);
    double static calculateDistanceBetweenTwoPoints(Atom& pt0, Atom& pt1);
    double static calculateAreaOfTriangle(Atom& p0, Atom& p1, Atom& p2);
    double static calculateAreaOfPolygon(Atom& coords_x, Atom& coords_y);
    vector<double> static getMaxAreaLBRT(Database& database);

    MatrixXd static CAD2OpenCV(Configuration& config, Mat& screen, Atom& originalPoint);
    void static atomOrientedAffine(Atom& atom_x, Atom& atom_y, MatrixXd& affineMat);
    void static RGB2BGR();
    void static BGR2RGB();
    void static screen2Array();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_PAINTERTOOLS_H
