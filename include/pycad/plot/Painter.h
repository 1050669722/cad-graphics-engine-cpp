//
// Created by liuxingbo03 on 2021/10/29.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_PAINTER_H
#define ATUOCADDATAANALYSISPYCADCPP_PAINTER_H


#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

#include "pycad/Structs.h"
#include "pycad/Configuration.h"
#include "pycad/Database.h"


using namespace std;
using namespace cv;
using namespace Eigen;


struct AggData {
    string text_info;
    Atom text_coord_x_atom;
    Atom text_coord_y_atom;
};


class Painter {
private:
    // 成员属性
    Configuration* config{};
    Database* database{};
    vector<AggData> aggData;
    MatrixXd restoreAffine;
    Coords* painting_coords_x = nullptr;
    Coords* painting_coords_y = nullptr;
    vector<string>* painting_colors = nullptr;

    // 成员方法
    void adjustAppropriateScaleFactor(double cadxmin, double cadymin, double cadxmax, double cadymax);
    void setEnv();

    // 对当前 这一类型的图元 的 所有实例 的坐标 画
    void paintC();
    void loadDataForOneFieldInDatabase(BaseFieldInDatabase& database_field);
    void shader(const char* eType); //意为着色器，原本的方法名是loadData
    void paintCore(const char* eType);
    void paintC_lines();
    void paintC_polylines();
    void paintC_circles();
    void paintC_arcs();
    void paintC_ellipses();
    void paintC_points();
    void paintC_splines();
    void paintC_texts();
    void paintC_mtexts();
    void paintC_attributes();
    void paintC_alignedDimensions();
    // void paintC_texts(); //TODO

public:
    // 成员属性
    Mat screen;

    // 成员方法
    Painter();
    explicit Painter(Configuration& config, Database& database);
    vector<double> getMaxAreaLBRT();
    void paint();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_PAINTER_H
