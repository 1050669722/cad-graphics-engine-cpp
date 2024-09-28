//
// Created by liuxingbo03 on 2021/10/9.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_ELEMENTS_H
#define ATUOCADDATAANALYSISPYCADCPP_ELEMENTS_H


#include "pycad/Structs.h"
#include "pycad/parse/ParseStructs.h"
#include "pycad/tools/ElementTools.h"


using namespace std;


// #define BaseAttr map<string, vector<string>>;


// *** BaseIns ****************************************************************************************************
class BaseIns : virtual public BaseFieldsInJsonInfoDictAndElementInses {
protected: //private:
    // 成员属性
    MatrixXd* AffineMat = nullptr;//=MatrixXd::Identity(3, 3);

    // 成员方法
    // TODO: 平移至原点的方法还没有写

public:
    // 本类新增 成员属性
    // map<string, vector<string>> baseAttrCri{};
    Coord coord_x;
    Coord coord_y;
    string color; //TODO: string类型 会被自动初始化吗？
    int fineLevel = 120;
    map<string, double>* ptr_min{};
    map<string, double>* ptr_max{};
    vector<string>* ptr_xdatas{};

    // 由于 引用 而衍生出来的 必要的 用于筛选的 属性
    Criterion criterion;

    // 成员方法
    // 构造函数
    BaseIns();
    explicit BaseIns(BaseFieldsInJsonInfoDictAndElementInses& infoDict, const char* hostHandle, map<string, double>& origin);

    // 仿射变换
    virtual void affine(
                        MatrixXd* affineMat,
                        vector<string> fields
                        );
    // origin机制
    void originTranslate(map<string, double>& origin);
    // // 各类属性 //TODO 针对pycad修改
    // static map<string, vector<string>> outputBaseAttrCri();
    // 报告判据
    void report(
            Layers& ts_layers,
            IsDyns& ts_isDyns,
            BLockReferenceHandles& ts_BRHandles,
            Pedigree& t_pedigree,
            string& t_b_handle,
            Visibles& ts_visibles,//
            IsFromHatchs& ts_isFromHatchs
            );
};
// ****************************************************************************************************************


// *** LineIns ****************************************************************************************************
class LineIns : public BaseIns, public LineInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_startPoint{};
    map<string, double>* ptr_endPoint{};
    map<string, double>* ptr_normal{};

    // 成员方法
    LineIns();
    explicit LineIns(LineInfoDict& lineInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** PolylineIns ************************************************************************************************
class PolylineIns : public BaseIns, public PolylineInfoDict { //这种菱形继承会不会比较危险？尽管Base和Polyline没有值冲突的字段，只有名字冲突的字段；
private:
    // 成员方法
    void getOriCoord();

public:
    // 成员属性
    Coord ori_coord_x;
    Coord ori_coord_y;
    vector<map<string, double>>* ptr_vertexs{};

    // 成员方法
    PolylineIns();
    explicit PolylineIns(PolylineInfoDict& polylineInfoDict, const char* hostHandle, map<string, double>& origin);
    void affine(
                    MatrixXd* affineMat,
                    vector<string> fields
                ) override;
};
// ****************************************************************************************************************


// *** CircleIns **************************************************************************************************
class CircleIns : public BaseIns, public CircleInfoDict {
private:
    // 成员属性
    map<string, double> startPoint;
    map<string, double> endPoint;

public:
    // 成员属性
    map<string, double>* ptr_center{};
    map<string, double>* ptr_normal{};

    // 成员方法
    CircleIns();
    explicit CircleIns(CircleInfoDict& circleInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** ArcIns *****************************************************************************************************
class ArcIns : public BaseIns, public ArcInfoDict {
public:
    //成员属性
    map<string, double>* ptr_center{};
    map<string, double>* ptr_startPoint{};
    map<string, double>* ptr_endPoint{};
    map<string, double>* ptr_normal{};

    // 成员方法
    ArcIns();
    explicit ArcIns(ArcInfoDict& arcInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** EllipseIns *************************************************************************************************
class EllipseIns : public BaseIns, public EllipseInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_center{};
    map<string, double>* ptr_majorAxis{};
    map<string, double>* ptr_minorAxis{};
    map<string, double>* ptr_normal{};

    // 成员方法
    EllipseIns();
    explicit EllipseIns(EllipseInfoDict& ellipseInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** PointIns ***************************************************************************************************
class PointIns : public BaseIns, public PointInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_position{};
    map<string, double>* ptr_normal{};

    // 成员方法
    PointIns();
    explicit PointIns(PointInfoDict& pointInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** SplineIns **************************************************************************************************
class SplineIns : public BaseIns, public SplineInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_startFitTangent{};
    map<string, double>* ptr_endFitTangent{};
    vector<map<string, double>>* ptr_controlPoints{};
    vector<map<string, double>>* ptr_fitPoints{};

    // 成员方法
    SplineIns();
    explicit SplineIns(SplineInfoDict& splineInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** TextIns ****************************************************************************************************
class TextIns : public BaseIns, public TextInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_position{};
    map<string, double>* ptr_normal{};

    // 成员方法
    TextIns();
    explicit TextIns(TextInfoDict& textInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** MtextIns ***************************************************************************************************
class MtextIns : public BaseIns, public MtextInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_direction{};
    map<string, double>* ptr_location{};
    map<string, double>* ptr_normal{};
    int lineNum{};

    // 成员方法
    MtextIns();
    explicit MtextIns(MtextInfoDict& mtextInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** AttributeIns ***********************************************************************************************
class AttributeIns : public BaseIns, public AttributeInfoDict {
public:
    // 成员属性
    map<string, double>* ptr_position{};
    map<string, double>* ptr_normal{};

    // 成员方法
    AttributeIns();
    explicit AttributeIns(AttributeInfoDict& attributeInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** DimensionIns ***********************************************************************************************
class DimensionIns : public BaseIns, public DimensionInfoDict {
// TODO: 因为不知道其它Dimension的字段，不能对比抽象出共同字段，所以这里Dimension将AlignedDimension中的全部字段搬运过来；后续如果字段丰富了，应该需要改进；
public:
    // 成员属性
    map<string, double>* ptr_textPosition{};
    map<string, double>* ptr_dimLinePoint{};
    map<string, double>* ptr_xLine1Point{};
    map<string, double>* ptr_xLine2Point{};

    // 成员方法
    DimensionIns();
    explicit DimensionIns(DimensionInfoDict& dimensionInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// *** AlignedDimensionIns ****************************************************************************************
class AlignedDimensionIns : public DimensionIns, public AlignedDimensionInfoDict {
public:
    // 成员属性

    // 成员方法
    AlignedDimensionIns();
    explicit AlignedDimensionIns(AlignedDimensionInfoDict& alignedDimensionInfoDict, const char* hostHandle, map<string, double>& origin);
};
// ****************************************************************************************************************


// class TextIns : public BaseIns {
// public:
//     // 本类新增 成员属性
//     string info;
//     double widthFactor;
//     double height;
//
//     // 成员方法
//     Text();
//     explicit Text(const ElementInfoDict &textInfoDict);
//     // Text(const ElementInfoDict &textInfoDict);
// };


/*
 * 如何添加一个新的图元类型？
 * a.在Elements.h和Elements.cpp中添加新的class；
 * b.继承BaseIns和Structs.h中相应的结构体；
 * c.在Elements.cpp中利用BaseIns和Structs.h中相应的结构体进行初始化；
 * d.在Elements.cpp中初始化Structs.h中相应的结构体比BaseIns多出来的属性；
 * e.如需要，还需新建XXXSketcher.cpp和XXXSketcher.h文件，以计算得到atom；
 * f.在BLock.h中添加相应的 图元实例容器；
 * g.在BLock.cpp中添加相应的 实例化逻辑；
 * h.在Tree.cpp中添加 计算坐标、计算判据、写数据库 的if-else分支；
 * i.在Tree.cpp和Tree.h中添加新的enrich函数，并在Tree::sprout中调用；
 * j.在Painter.h和Painter.cpp中添加相应的 绘制方法 和 loadData中相应的if-else分支；
 * k.某图元类型的额外字段需要在Tree, Filter, Clip, Painter中添加；
 */


#endif //ATUOCADDATAANALYSISPYCADCPP_ELEMENTS_H
