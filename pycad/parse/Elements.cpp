//
// Created by liuxingbo03 on 2021/10/9.
//

#include <iostream>
#include <vector>
#include <cassert>

#include "pycad/Utils.h"
#include "pycad/parse/Elements.h"
#include "pycad/sketcher/PolylineSketcher.h"
#include "pycad/sketcher/ArcSketcher.h"
#include "pycad/sketcher/EllipseSketcher.h"
#include "pycad/sketcher/SplineSketcher.h"
#include "pycad/sketcher/AlignedDimensionSketcher.h"


BaseIns::BaseIns() = default; //TODO: Ins - Instance, 这里理解成“实体”，意思更接近于Entity；而不接近“实例”；


// *** BaseIns ****************************************************************************************************
BaseIns::BaseIns(BaseFieldsInJsonInfoDictAndElementInses& infoDict, const char* hostHandle, map<string, double> &origin) : BaseFieldsInJsonInfoDictAndElementInses() {
    // 预分配空间
    coord_x.reserve(100);
    coord_y.reserve(100);

    // // 初始化 baseAttrCri 成员属性 //原本在python中是类属性 //实例化的时候，不需要这一属性，只是在需要的地方出现，比如Filter中
    // baseAttrCri = BaseIns::outputBaseAttrCri();

    // 谁初始化了这个Base，谁来写这个字段 //现在外部初始化Base的是BLock
    criterion.belongToWhichBlock = hostHandle;

    // 初始化 数据固有 成员属性 ///这只是为了配合源数据的字段
    id = infoDict.id;
    handle = infoDict.handle;
    name = infoDict.name;
    layer = infoDict.layer;
    // min = infoDict.min;
    ptr_min = &(infoDict.min);
    // max = infoDict.max;
    ptr_max = &(infoDict.max);
    // xdatas = infoDict.xdatas;
    ptr_xdatas = &(infoDict.xdatas);
    visible = infoDict.visible;
    isDyn = infoDict.isDyn; //TODO: infoDict中的isDyn是从哪里来的？ //isDynBlock仅在Reference中出现过
    isFromHatch = infoDict.isFromHatch; //TODO: infoDict中的isFromHatch是从哪里来的？ //来自于json文件的block中的hatchs字段

    // 由于 引用 而 衍生出来的 属性 ///这才是Base真正有用的属性，可以配合Tree
    criterion.layerHistory.push_back(layer); //这个属性，每一个图元是具有的
    criterion.visibleHistory.push_back(visible); //这个属性，每一个图元是具有的
    criterion.isDynHistory.push_back(isDyn); //这个属性，每一个图元是不具有的
    criterion.isFromHatchHistory.push_back(isFromHatch); //这个属性，每一个图元是不具有的

    // origin机制，BaseIns在初始化末尾，会有一个坐标的归位
    originTranslate(origin);
}


void BaseIns::affine(
                    MatrixXd* affineMat,
                    vector<string> fields
                    ) {
    // 调整字段fields
    if (fields.empty()) {
        fields.emplace_back("coord_x");
        fields.emplace_back("coord_y");
    }

    // 先将仿射变换矩阵准备好
    this->AffineMat = affineMat;

    // 字段选择 //字段名字有问题，这里类型确实是Coord
    Coord* atoms_x; //=nullptr;
    Coord* atoms_y; //=nullptr;
    if ((fields[0] == "coord_x") && (fields[1] == "coord_y")) {
        atoms_x = &(this->coord_x);
        atoms_y = &(this->coord_y);
    }
    else {
        throw runtime_error("[ERROR] Invalid Parameter \"fields\"");
    }

    // 对于坐标列表中的每一个原子
    for (int i = 0; i < atoms_x->size(); ++i) {
        ElementTools::atomOrientedAffine((*atoms_x)[i], (*atoms_y)[i], *AffineMat);
    }
}


void BaseIns::originTranslate(map<string, double> &origin) {
    MatrixXd affineMat = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat, 1.0, 1.0, 0.0, origin["x"], origin["y"]);
    affine(&affineMat, vector<string>{"coord_x", "coord_y"});
}


void BaseIns::report(
        Layers& ts_layers,
        IsDyns& ts_isDyns,
        BLockReferenceHandles& ts_BRHandles,
        Pedigree& t_pedigree,
        string& t_b_handle,
        Visibles& ts_visibles,//
        IsFromHatchs& ts_isFromHatchs
                    ) {
    // TODO:
    // layerHistory ////因为Base自己有layer，所以这里采用insert
    if (!(ts_layers.empty())) {
        criterion.layerHistory.insert(criterion.layerHistory.end(), ts_layers.rbegin(), ts_layers.rend()); //TODO: 这样是反序拼接吧？
    }

    // isDynHistory ////Base自己有isDynHistory
    if (!(ts_isDyns.empty())) {
        criterion.isDynHistory.insert(criterion.isDynHistory.end(), ts_isDyns.rbegin(), ts_isDyns.rend());
    }

    // BRHandleHistory //TODO: Base自己的BRHandleHistory为空，如果不会修改ts_BRHandles，可以采用等于号=
    if (!(ts_BRHandles.empty())) {
        criterion.BRHandleHistory.insert(criterion.BRHandleHistory.end(), ts_BRHandles.rbegin(), ts_BRHandles.rend());
    }

    // pedigreeLogger //TODO: Base自己的pedigreeLogger为空，如果不会修改t_pedigree，可以采用等于号=
    if (!(t_pedigree.empty())) {
        criterion.pedigreeLogger.insert(criterion.pedigreeLogger.end(), t_pedigree.begin(), t_pedigree.end());
        assert(!(&criterion.pedigreeLogger == &t_pedigree)); //不应该改变t_pedigree //TODO: 考虑删除
    }
    criterion.pedigreeLogger.push_back(t_b_handle);

    // visibleHistory ////因为Base自己有visible，所以这里采用insert
    if (!(ts_visibles.empty())) {
        criterion.visibleHistory.insert(criterion.visibleHistory.end(), ts_visibles.rbegin(), ts_visibles.rend());
    }

    // isFromHatchHistory ////因为Base自己有isFromHatch，所以这里采用insert
    if (!(ts_isFromHatchs.empty())) {
        criterion.isFromHatchHistory.insert(criterion.isFromHatchHistory.end(), ts_isFromHatchs.rbegin(), ts_isFromHatchs.rend());
    }
}
// ****************************************************************************************************************


// *** LineIns ****************************************************************************************************
LineIns::LineIns() = default;


LineIns::LineIns(LineInfoDict &lineInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(lineInfoDict, hostHandle, origin), LineInfoDict() {
    // 初始化 数据固有 成员属性
    closed = lineInfoDict.closed;
    // startPoint = lineInfoDict.startPoint;
    ptr_startPoint = &(lineInfoDict.startPoint);
    // endPoint = lineInfoDict.endPoint;
    ptr_endPoint = &(lineInfoDict.endPoint);
    // normal = lineInfoDict.normal;
    ptr_normal = &(lineInfoDict.normal);

    // 原子获取坐标
    auto atom_x = Atom{}, atom_y = Atom{};
    atom_x.reserve(2); atom_y.reserve(2);
    atom_x.push_back((*ptr_startPoint)["x"]), atom_y.push_back((*ptr_startPoint)["y"]);
    atom_x.push_back((*ptr_endPoint)["x"]), atom_y.push_back((*ptr_endPoint)["y"]);

    // 坐标列表获取原子
    coord_x.push_back(atom_x), coord_y.push_back(atom_y);

    // 默认颜色
    if (color.empty()) color = "g";
}
// ****************************************************************************************************************


// *** PolylineIns ************************************************************************************************
PolylineIns::PolylineIns() = default;


PolylineIns::PolylineIns(PolylineInfoDict& polylineInfoDict, const char* hostHandle, map<string, double> &origin) : BaseIns(polylineInfoDict, hostHandle, origin), PolylineInfoDict() {
    // 初始化 数据固有 成员属性
    closed = polylineInfoDict.closed;
    // vertexs = polylineInfoDict.vertexs;
    ptr_vertexs = &(polylineInfoDict.vertexs);

    // 用于规范边界的原始坐标
    getOriCoord(); //注意getOriCoord()的位置

    // 遍历
    for (int i = 0; i < (*ptr_vertexs).size() - 1; ++i) {
        // 原子获取坐标
        PolylineSketcher polylineSketcher = PolylineSketcher((*ptr_vertexs)[i], (*ptr_vertexs)[i + 1]);
        // 坐标列表获取原子
        coord_x.push_back(polylineSketcher.atom_x), coord_y.push_back(polylineSketcher.atom_y);
    }
    // closed //PolylineIns比较特殊，闭合逻辑需要在Sketcher外部做，因为此时Coord的闭合并不等价于Atom的闭合
    if (closed) {
        // 原子获取坐标
        PolylineSketcher polylineSketcher = PolylineSketcher((*ptr_vertexs)[(*ptr_vertexs).size() - 1], (*ptr_vertexs)[0]);
        // 坐标列表获取原子
        coord_x.push_back(polylineSketcher.atom_x), coord_y.push_back(polylineSketcher.atom_y);
    }

    // 默认颜色
    if (color.empty()) color = "r";
}


void PolylineIns::getOriCoord() {
    /*
     * 获取 用于规范边界的原始坐标
     */
    // 预分配空间
    ori_coord_x.reserve(100);
    ori_coord_y.reserve(100);

    // 原子获取坐标
    auto atom_x = Atom{}, atom_y = Atom{};
    atom_x.reserve(100); atom_y.reserve(100);
    for (auto& vertex : (*ptr_vertexs)) {
        atom_x.push_back(vertex["x"]), atom_y.push_back(vertex["y"]);
    }
    // 确保首尾相接 //TODO closed?
    if (closed) {
        atom_x.push_back((*ptr_vertexs)[0]["x"]), atom_y.push_back((*ptr_vertexs)[0]["y"]);
    }

    // 坐标列表获取原子
    ori_coord_x.push_back(atom_x), ori_coord_y.push_back(atom_y);
};


void PolylineIns::affine(
                            MatrixXd* affineMat,
                            vector<string> fields
                        ) {
    // 调整字段fields
    if (fields.empty()) {
        fields.emplace_back("coord_x");
        fields.emplace_back("coord_y");
    }

    // 先将仿射变换矩阵准备好
    this->AffineMat = affineMat;

    // 字段选择 //字段名字有问题，这里类型确实是Coord
    Coord* atoms_x; //=nullptr;
    Coord* atoms_y; //=nullptr;
    if ((fields[0] == "coord_x") && (fields[1] == "coord_y")) {
        atoms_x = &(this->coord_x);
        atoms_y = &(this->coord_y);
    }
    else if ((fields[0] == "ori_coord_x") && (fields[1] == "ori_coord_y")) {
        atoms_x = &(this->ori_coord_x);
        atoms_y = &(this->ori_coord_y);
    }
    else {
        throw runtime_error("[ERROR] Invalid Parameter \"fields\"");
    }

    // 对于坐标列表中的每一个原子
    for (int i = 0; i < (*atoms_x).size(); ++i) {
        ElementTools::atomOrientedAffine((*atoms_x)[i], (*atoms_y)[i], *AffineMat);
    }

}
// ****************************************************************************************************************


// *** CircleIns **************************************************************************************************
CircleIns::CircleIns() = default;


CircleIns::CircleIns(CircleInfoDict &circleInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(circleInfoDict, hostHandle, origin), CircleInfoDict() {
    // 初始化 数据固有 成员属性
    // center = circleInfoDict.center;
    ptr_center = &(circleInfoDict.center);
    radius = circleInfoDict.radius;
    closed = circleInfoDict.closed;
    // normal = circleInfoDict.normal;
    ptr_normal = &(circleInfoDict.normal);

    // 起点 和 终点
    startPoint["x"] = (*ptr_center)["x"] + radius * cos(0.0), startPoint["y"] = (*ptr_center)["y"] + radius * sin(0.0), startPoint["z"] = 0.0;
    endPoint["x"] = (*ptr_center)["x"] + radius * cos(-0.1), endPoint["y"] = (*ptr_center)["y"] + radius * sin(-0.1), endPoint["z"] = 0.0; //endPoint转了一圈，快要回到startPoint

    // 原子获取坐标
    ArcSketcher arcSketcher = ArcSketcher(
                                        startPoint,
                                        endPoint,
                                        (*ptr_center),
                                        radius,
                                        closed,
                                        (*ptr_normal)["z"],
                                        fineLevel
                                        );

    // 坐标列表获取原子
    coord_x.push_back(arcSketcher.atom_x), coord_y.push_back(arcSketcher.atom_y);

    // 默认颜色
    if (color.empty()) color = "b";
}
// ****************************************************************************************************************


// *** ArcIns *****************************************************************************************************
ArcIns::ArcIns() = default;

ArcIns::ArcIns(ArcInfoDict &arcInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(arcInfoDict, hostHandle, origin), ArcInfoDict() {
    // 初始化 数据固有 成员属性
    closed = arcInfoDict.closed;
    // center = arcInfoDict.center;
    ptr_center = &(arcInfoDict.center);
    radius = arcInfoDict.radius;
    // startPoint = arcInfoDict.startPoint;
    ptr_startPoint = &(arcInfoDict.startPoint);
    // endPoint = arcInfoDict.endPoint;
    ptr_endPoint = &(arcInfoDict.endPoint);
    // normal = arcInfoDict.normal;
    ptr_normal = &(arcInfoDict.normal);

    // 原子获取坐标
    ArcSketcher arcSketcher = ArcSketcher(
                                        (*ptr_startPoint),
                                        (*ptr_endPoint),
                                        (*ptr_center),
                                        radius,
                                        closed,
                                        (*ptr_normal)["z"],
                                        fineLevel
                                        );

    // 坐标列表获取原子
    coord_x.push_back(arcSketcher.atom_x), coord_y.push_back(arcSketcher.atom_y);

    // 默认颜色
    if (color.empty()) color = "m";
}
// ****************************************************************************************************************


// *** EllipseIns *************************************************************************************************
EllipseIns::EllipseIns() = default;

EllipseIns::EllipseIns(EllipseInfoDict &ellipseInfoDict, const char* hostHandle, map<string, double> &origin) : BaseIns(ellipseInfoDict, hostHandle, origin), EllipseInfoDict() {
    // 初始化 数据固有 成员属性
    closed = ellipseInfoDict.closed;
    // center = ellipseInfoDict.center;
    ptr_center = &(ellipseInfoDict.center);
    startAngle = ellipseInfoDict.startAngle;
    endAngle = ellipseInfoDict.endAngle;
    // majorAxis = ellipseInfoDict.majorAxis; //TODO: 后续应该修改为majorAxis
    ptr_majorAxis = &(ellipseInfoDict.majorAxis); //TODO: 后续应该修改为majorAxis
    // minorAxis = ellipseInfoDict.minorAxis;
    ptr_minorAxis = &(ellipseInfoDict.minorAxis);
    radiusRatio = ellipseInfoDict.radiusRatio; //TODO: 后续应该修改为radiusRatio，但是好像这个字段用不到
    // normal = ellipseInfoDict.normal;
    ptr_normal = &(ellipseInfoDict.normal);

    // 原子获取坐标
    EllipseSketcher ellipseSketcher = EllipseSketcher(
                                                        startAngle,
                                                        endAngle,
                                                        (*ptr_center),
                                                        (*ptr_majorAxis), //TODO: 后续应该修改为majorAxis
                                                        (*ptr_minorAxis),
                                                        closed,
                                                        (*ptr_normal)["z"],
                                                        fineLevel
                                                    );

    // 坐标列表获取原子
    coord_x.push_back(ellipseSketcher.atom_x), coord_y.push_back(ellipseSketcher.atom_y);

    // 计算旋转弧度值 //TODO: 这里是与python版本保持了一致
    double x, y;
    // if (sqrt(pow((*ptr_majorAxis)["x"], 2.0) + pow((*ptr_majorAxis)["y"], 2.0))
    //         >= sqrt(pow((*ptr_minorAxis)["x"], 2.0) + pow((*ptr_minorAxis)["y"], 2.0))) {
    if ((*ptr_majorAxis)["x"] >= (*ptr_majorAxis)["y"]) {
        x = (*ptr_majorAxis)["x"];
        y = (*ptr_majorAxis)["y"];
    }
    else {
        x = (*ptr_minorAxis)["x"];
        y = (*ptr_minorAxis)["y"];
    }
    double cosTheta = x / sqrt(pow(x, 2.0) + pow(y, 2.0));
    double theta = acos(cosTheta);
    if (y < 0) theta *= -1.0;

    // 绕着椭圆中心旋转 //TODO: 这里是与python版本保持了一致
    MatrixXd affineMat0 = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat0, 1.0, 1.0, 0.0, -1.0 * (*ptr_center)["x"], -1.0 * (*ptr_center)["y"]);
    affine(&affineMat0, vector<string>{"coord_x", "coord_y"});

    MatrixXd affineMat1 = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat1, 1.0, 1.0, theta, 0.0, 0.0);
    affine(&affineMat1, vector<string>{"coord_x", "coord_y"});

    MatrixXd affineMat2 = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat2, 1.0, 1.0, 0.0, (*ptr_center)["x"], (*ptr_center)["y"]);
    affine(&affineMat2, vector<string>{"coord_x", "coord_y"});

    // 默认颜色
    if (color.empty()) color = "pink";
}
// ****************************************************************************************************************


// *** PointIns ***************************************************************************************************
PointIns::PointIns() = default;

PointIns::PointIns(PointInfoDict &pointInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(pointInfoDict, hostHandle, origin), PointInfoDict() {
    // 初始化 数据固有 成员属性
    ptr_position = &(pointInfoDict.position);
    ecsRotation = pointInfoDict.ecsRotation;
    ptr_normal = &(pointInfoDict.normal);

    // 原子获取坐标
    auto atom_x = Atom{(*ptr_position)["x"]}, atom_y = Atom{(*ptr_position)["y"]};

    // 坐标列表获取原子
    coord_x.push_back(atom_x), coord_y.push_back(atom_y);

    // 默认颜色
    if (color.empty()) color = "yellow";
}
// ****************************************************************************************************************


// *** SplineIns **************************************************************************************************
SplineIns::SplineIns() = default;

SplineIns::SplineIns(SplineInfoDict &splineInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(splineInfoDict, hostHandle, origin), SplineInfoDict() {
    // 初始化 数据固有 成员属性
    closed = splineInfoDict.closed;
    degree = splineInfoDict.degree;
    // startFitTangent = splineInfoDict.startFitTangent;
    ptr_startFitTangent = &(splineInfoDict.startFitTangent);
    // endFitTangent = splineInfoDict.endFitTangent;
    ptr_endFitTangent = &(splineInfoDict.endFitTangent);
    hasFitData = splineInfoDict.hasFitData;
    isRational = splineInfoDict.isRational;
    controlPointNub = splineInfoDict.controlPointNub;
    // controlPoints = splineInfoDict.controlPoints;
    ptr_controlPoints = &(splineInfoDict.controlPoints);
    fitPointNub = splineInfoDict.fitPointNub;
    // fitPoints = splineInfoDict.fitPoints;
    ptr_fitPoints = &(splineInfoDict.fitPoints);

    // 原子获取坐标
    SplineSketcher splineSketcher = SplineSketcher(
                                                    degree,
                                                    0.1, //0.01 //TODO: 采样点多少比较合适？
                                                    (*ptr_controlPoints)
                                                    );

    // 坐标列表获取原子
    coord_x.push_back(splineSketcher.atom_x), coord_y.push_back(splineSketcher.atom_y);

    // 默认颜色
    if (color.empty()) color = "orange";
}
// ****************************************************************************************************************


// *** TextIns ****************************************************************************************************
TextIns::TextIns() = default;

TextIns::TextIns(TextInfoDict& textInfoDict, const char* hostHandle, map<string, double>& origin) : BaseIns(textInfoDict, hostHandle, origin), TextInfoDict() {
    // 初始化 数据固有 成员属性
    height = textInfoDict.height;
    widthFactor = textInfoDict.widthFactor;
    text = textInfoDict.text;
    ptr_position = &(textInfoDict.position);
    rotation = textInfoDict.rotation;
    ptr_normal = &(textInfoDict.normal);
    horizontalMode = textInfoDict.horizontalMode;
    verticalMode = textInfoDict.verticalMode;

    // 原子获取坐标 //TODO: 编码方式可能随着机器平台改变
    double width = widthFactor * height * (double)(Utils::getUtf8LetterNumber(text));
    auto atom_x = Atom{0.0, width, width, 0.0, 0.0};
    auto atom_y = Atom{0.0, 0.0, height, height, 0.0};

    // 坐标列表获取原子 //TODO: python版本中的Painter.py等效地做了这个if逻辑，所以这里先不写这个if逻辑了
    // if (not ((strcmp(text.c_str(), "\"\"") == 0) or (strcmp(text.c_str(), "\'\'") == 0) or (strcmp(text.c_str(), "\"") == 0) or (strcmp(text.c_str(), "\'") == 0))) {
    coord_x.push_back(atom_x);
    coord_y.push_back(atom_y);
    // }

    // 内部仿射变换
    MatrixXd affineMat = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat, (*ptr_normal)["z"] * 1.0, 1.0, (*ptr_normal)["z"] * rotation, (*ptr_position)["x"], (*ptr_position)["y"]);
    affine(&affineMat, vector<string>{"coord_x", "coord_y"});

    // 默认颜色
    if (color.empty()) color = "black";
}
// ****************************************************************************************************************


// *** MtextIns ***************************************************************************************************
MtextIns::MtextIns() = default;

MtextIns::MtextIns(MtextInfoDict &mtextInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(mtextInfoDict, hostHandle, origin), MtextInfoDict() {
    // TODO: 画出来的框与python版本一致，但是与AutoCAD显示的情况有出入，应该是对齐方式的问题
    // 初始化 数据固有 成员属性
    actualHeight = mtextInfoDict.actualHeight;
    actualWidth = mtextInfoDict.actualWidth;
    contents = mtextInfoDict.contents;
    ptr_direction = &(mtextInfoDict.direction); //TODO: direction应该是需要换算到rotation上
    ptr_location = &(mtextInfoDict.location);
    ptr_normal = &(mtextInfoDict.normal);
    rotation = mtextInfoDict.rotation;

    // 原子获取坐标
    int descent = 50;
    lineNum = (int)(actualHeight / descent);
    auto atom_x = Atom{location["x"], location["x"] + actualWidth, location["x"] + actualWidth, location["x"], location["x"]};
    auto atom_y = Atom{location["y"], location["y"], location["y"] + actualHeight, location["y"] + actualHeight, location["y"]};

    // 坐标列表获取原子
    coord_x.push_back(atom_x);
    coord_y.push_back(atom_y);

    // 内部仿射变换
    MatrixXd affineMat = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat, (*ptr_normal)["z"] * 1.0, 1.0, (*ptr_normal)["z"] * rotation, (*ptr_location)["x"], (*ptr_location)["y"]);
    affine(&affineMat, vector<string>{"coord_x", "coord_y"});

    // 修改contents //TODO: 目前仅知道\P为换行符，其余规则有待探索
    replace(contents.begin(), contents.end(), '\P', '\n');

    // 默认颜色
    if (color.empty()) color = "mazarine";
}
// ****************************************************************************************************************


// *** AttributeIns ***********************************************************************************************
AttributeIns::AttributeIns() = default;

AttributeIns::AttributeIns(AttributeInfoDict& attributeInfoDict, const char* hostHandle, map<string, double>& origin) : BaseIns(attributeInfoDict, hostHandle, origin), AttributeInfoDict() {
    // 初始化 数据固有 成员属性
    height = attributeInfoDict.height;
    widthFactor = attributeInfoDict.widthFactor;
    text = attributeInfoDict.text;
    ptr_position = &(attributeInfoDict.position);
    rotation = attributeInfoDict.rotation;
    ptr_normal = &(attributeInfoDict.normal);
    horizontalMode = attributeInfoDict.horizontalMode;
    verticalMode = attributeInfoDict.verticalMode;
    tag = attributeInfoDict.tag;

    // 原子获取坐标 //TODO: 编码方式可能随着机器平台改变
    double width = widthFactor * height * (double)(Utils::getUtf8LetterNumber(text));
    auto atom_x = Atom{0.0, width, width, 0.0, 0.0};
    auto atom_y = Atom{0.0, 0.0, height, height, 0.0};

    // 坐标列表获取原子 //TODO: python版本中的Painter.py等效地做了这个if逻辑，所以这里先不写这个if逻辑了
    // if (not ((strcmp(text.c_str(), "\"\"") == 0) or (strcmp(text.c_str(), "\'\'") == 0) or (strcmp(text.c_str(), "\"") == 0) or (strcmp(text.c_str(), "\'") == 0))) {
    coord_x.push_back(atom_x);
    coord_y.push_back(atom_y);
    // }

    // 内部仿射变换
    MatrixXd affineMat = MatrixXd::Identity(3, 3);
    Utils::generateAffineMat(affineMat, (*ptr_normal)["z"] * 1.0, 1.0, (*ptr_normal)["z"] * rotation, (*ptr_position)["x"], (*ptr_position)["y"]);
    affine(&affineMat, vector<string>{"coord_x", "coord_y"});

    // 默认颜色
    if (color.empty()) color = "grassgreen";
}
// ****************************************************************************************************************


// *** DimensionIns ***********************************************************************************************
DimensionIns::DimensionIns() = default;

DimensionIns::DimensionIns(DimensionInfoDict &dimensionInfoDict, const char *hostHandle, map<string, double> &origin) : BaseIns(dimensionInfoDict, hostHandle, origin), DimensionInfoDict() {
    // 初始化 数据固有 成员属性
    textAttachment = dimensionInfoDict.textAttachment;
    textWidth = dimensionInfoDict.textWidth;
    textHeight = dimensionInfoDict.textHeight;
    textLineSpacingFactor = dimensionInfoDict.textLineSpacingFactor;
    ptr_textPosition = &(dimensionInfoDict.textPosition);
    textRotation = dimensionInfoDict.textRotation;
    isUsingDefaultTextPosition = dimensionInfoDict.isUsingDefaultTextPosition;
    dimensionText = dimensionInfoDict.dimensionText;
    measurement = dimensionInfoDict.measurement;
    searchValue = dimensionInfoDict.searchValue;
    ptr_dimLinePoint = &(dimensionInfoDict.dimLinePoint);
    ptr_xLine1Point = &(dimensionInfoDict.xLine1Point);
    ptr_xLine2Point = &(dimensionInfoDict.xLine2Point);
    oblique = dimensionInfoDict.oblique;

    // 默认颜色
    if (color.empty()) color = "chocolate";
}
// ****************************************************************************************************************


// *** AlignedDimensionIns ****************************************************************************************
AlignedDimensionIns::AlignedDimensionIns() = default;

AlignedDimensionIns::AlignedDimensionIns(AlignedDimensionInfoDict &alignedDimensionInfoDict, const char *hostHandle, map<string, double> &origin) : DimensionIns(alignedDimensionInfoDict, hostHandle, origin), AlignedDimensionInfoDict() {
    // 原子获取坐标
    AlignedDimensionSketcher alignedDimensionSketcher(
            (*ptr_dimLinePoint),
            (*ptr_xLine1Point),
            (*ptr_xLine2Point)
            );

    // 坐标列表获取原子
    coord_x.push_back(alignedDimensionSketcher.atom_x), coord_y.push_back(alignedDimensionSketcher.atom_y);
}
// ****************************************************************************************************************
