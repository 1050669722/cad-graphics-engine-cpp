//
// Created by liuxingbo03 on 2021/10/25.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_DATABASE_H
#define ATUOCADDATAANALYSISPYCADCPP_DATABASE_H


// 这是一个最抽象的，最被依赖的头文件


#include <deque>
#include "nlohmann/json.hpp"

#include "pycad/Structs.h"


using namespace std;
using json = nlohmann::json;


// struct BaseFieldInDatabase {
//     Coords coords_x;
//     Coords coords_y;
//     vector<string> colors;
//     vector<string> handles;
//     vector<Criterion> criterions;
// };


class BaseFieldInDatabase { //应该是指lines, polylines等图元类型
public:
    // 成员属性
    Coords coords_x;
    Coords coords_y;
    vector<string> colors;
    vector<string> handles;
    vector<Criterion> criterions;

    // 成员方法
    BaseFieldInDatabase() {
        coords_x.reserve(10000);
        coords_y.reserve(10000);
        colors.reserve(10000);
        handles.reserve(10000);
        criterions.reserve(10000);
    };
};


struct Lines : public BaseFieldInDatabase {

};


// struct Polylines : public BaseFieldInDatabase {
//     // vector<vector<double>> startWidths; //startW, endW, bulge不是database需要的
//     // vector<vector<double>> endWidths;
//     Coords ori_coords_x;
//     Coords ori_coords_y;
// };


class Polylines : public BaseFieldInDatabase {
public:
    // 成员属性
    Coords ori_coords_x;
    Coords ori_coords_y;
    // 成员方法
    Polylines() : BaseFieldInDatabase() {
        ori_coords_x.reserve(10000);
        ori_coords_y.reserve(10000);
    }
};


struct Circles : public BaseFieldInDatabase {

};


struct Arcs : public BaseFieldInDatabase {

};


struct Ellipses : public BaseFieldInDatabase {

};


struct Points : public BaseFieldInDatabase {

};


struct Splines : public BaseFieldInDatabase {

};


struct Solids : public BaseFieldInDatabase {

};


struct Texts : public BaseFieldInDatabase {
    // TODO: 字段名应该修改得更合适一些 //最起码应该是复数 //相应地，Formalization.py中也应该做相应调整
    vector<string> info; //TODO: 合适的字段名：textInfos
    vector<double> rota;
    vector<double> widthFactor;
    vector<string> horizontalModes;
    vector<string> verticalModes;
};


// TODO 原本的pycad中就没有对mtexts做完整
struct Mtexts : public BaseFieldInDatabase {
    vector<string> info; //TODO: 合适的字段名：mtextInfos
    vector<double> rota;
    vector<double> widthFactor;
    vector<string> horizontalModes; //TODO: 目前.json文件中，关于mtexts，没有这两个字段，这里只是为了迎合python版本
    vector<string> verticalModes;
    vector<int> lineNums;
};


// TODO 以下这些数据结构的字段和类型是否 对Structs.h和Elements.h中的结构 有用
struct ATtributes : public BaseFieldInDatabase {
    vector<string> info; //TODO: 合适的字段名：attributeInfos
    vector<double> rota;
    vector<double> widthFactor;
    vector<string> horizontalModes;
    vector<string> verticalModes;
};


struct Dimensions : public BaseFieldInDatabase {
    // 各种Dimensions都具有的独特字段
    // TODO: 在Structs.h的DimensionInfoDict中，除了measurement，还有其它字段，但是这里目前仅搜集这一个字段的信息
    vector<double> measurements;
};


struct AlignedDimensions : public Dimensions {

};


struct RotatedDimensions : public Dimensions {

};


struct LineAngularDimensions : public Dimensions {

};


struct ArcDimensions : public Dimensions {

};


struct RadialDimensions : public Dimensions {

};


struct DiametricDimensions : public Dimensions {

};


struct OrdinateDimensions : public Dimensions {

};


struct RadialDimensionLarges : public Dimensions {

};


// Database
// struct Database {
//     Lines lines;
//     Polylines polylines;
//     Circles circles;
//     Arcs arcs;
//     Ellipses ellipses;
//     Points points;
//     Splines splines;
//     Solids solids;
//     Texts texts;
//     Mtexts mtexts;
//     ATtributes attributes;
//     AlignedDimensions alignedDimensions;
//     RotatedDimensions rotatedDimensions;
//     LineAngularDimensions lineAngularDimensions;
//     ArcDimensions arcDimensions;
//     RadialDimensions radialDimensions;
//     DiametricDimensions diametricDimensions;
//     OrdinateDimensions ordinateDimensions;
//     RadialDimensionLarges radialDimensionLarges;
// };

class Database {
public:
    // 成员属性
    Lines lines;
    Polylines polylines;
    Circles circles;
    Arcs arcs;
    Ellipses ellipses;
    Points points;
    Splines splines;
    Solids solids;
    Texts texts;
    Mtexts mtexts;
    ATtributes attributes;
    AlignedDimensions alignedDimensions;
    RotatedDimensions rotatedDimensions;
    LineAngularDimensions lineAngularDimensions;
    ArcDimensions arcDimensions;
    RadialDimensions radialDimensions;
    DiametricDimensions diametricDimensions;
    OrdinateDimensions ordinateDimensions;
    RadialDimensionLarges radialDimensionLarges;

    // 成员方法
    Database();

    // 搬运至DatabaseConverter.cpp
    // // 这些 保存 和 加载 方法，是用于 [缓存机制] 和 [接口传输] 的 //开发版本不用这些
    // // 甚至 [缓存机制] 都用不着这些，因为 [缓存机制] 放在 python层 去做，明显更好
    // // 保存
    // void dumps(string& jsonstr);
    // void dump(string& filepath);
    // // void dumpCache(string& cache, string& name);
    // // 加载
    // // static void loads(Database& database, string& jsonstr);
    // // static void load(Database& database, string& cache);
    // // // static void loadCache(Database& database, string& cache);
};


#endif //ATUOCADDATAANALYSISPYCADCPP_DATABASE_H
