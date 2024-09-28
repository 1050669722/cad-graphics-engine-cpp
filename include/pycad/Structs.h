//
// Created by liuxingbo03 on 2021/10/9.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_STRUCTS_H
#define ATUOCADDATAANALYSISPYCADCPP_STRUCTS_H


// 这是一个最抽象的，最被依赖的头文件，也是最有理由不被修改的


#include <string>
#include <vector>
#include <deque>
#include <map>


using namespace std;


// 坐标
// #define Atom vector<double>
typedef vector<double> Atom;

// #define Coord vector<Atom>
typedef vector<Atom> Coord;

// #define Coords vector<Coord>
typedef vector<Coord> Coords;


// 判据
typedef vector<string> LayerHistory;
typedef vector<string> BLockReferenceHandleHistory;
typedef vector<string> PedigreeLogger;
typedef deque<bool> VisibleHistory;
typedef deque<bool> IsDynHistory;
typedef deque<bool> IsFromHatchHistory;

// struct Criterion {
//     string belongToWhichBlock;                          //belongToWhichBlocks //新老字段对比
//     LayerHistory layerHistory;                       //layers //新老字段对比
//     bool isDynHistory;                                     //isDyns //新老字段对比 //TODO: 是否可以改成isDynHistory？ //并非isDynHistory，因为这是一个一票否决的字段，不关注历史
//     BLockReferenceHandleHistory BRHandleHistory;                 //BRHandles //新老字段对比
//     PedigreeLogger pedigreeLogger;                   //blockHandlesPedigrees //新老字段对比
//     VisibleHistory visibleHistory;                   //visibles //新老字段对比
//     IsFromHatchHistory isFromHatchHistory;           //isFromHatchs //新老字段对比
// };

class Criterion {
public:
    // 成员属性
    string belongToWhichBlock;                          //belongToWhichBlocks //新老字段对比
    LayerHistory layerHistory;                       //layers //新老字段对比
    BLockReferenceHandleHistory BRHandleHistory;                 //BRHandles //新老字段对比
    PedigreeLogger pedigreeLogger;                   //blockHandlesPedigrees //新老字段对比
    VisibleHistory visibleHistory;                   //visibles //新老字段对比
    // bool isDynLast;                               //isDyns //新老字段对比 //TODO: 是否可以改成isDynHistory？ //并非isDynHistory，因为这是一个一票否决的字段，不关注历史[？？？？？？]
    IsDynHistory isDynHistory;
    IsFromHatchHistory isFromHatchHistory;           //isFromHatchs //新老字段对比

    // 成员方法
    Criterion() {
        layerHistory.reserve(100); //预分配空间
        BRHandleHistory.reserve(100); //预分配空间
        pedigreeLogger.reserve(100); //预分配空间
    };
};


// 属性判据分类 //TODO: 放在这里是合适的，但是非得写成一个类吗？
class BaseAttrCriterion {
public:
    static map<string, vector<string>> outputBaseAttrCriterion() {
        map<string, vector<string>> baseAttrCri = map<string, vector<string>> {
            // TODO: 以下字段或许可以独立出去，作为单独的接口
            // TODO: 以下字段的分类可能需要调整
            // pair<string, vector<string>>("coordAttr", vector<string>({"coord_x", "coord_y"})),
            // pair<string, vector<string>>("openCriAttr", vector<string>({"color", "layerHistory", "isDynHistory", "belongToWhichBlock", "BRHandleHistory", "pedigreeLogger"})), //"color", "layers", "isDyns", "belongToWhichBlocks", "BRHandles", "blockHandlesPedigree"
            // pair<string, vector<string>>("closeCriAttr", vector<string>({"visibleHistory", "isFromHatchHistory"})), //"visibles", "isFromHatchs"
            // pair<string, vector<string>>("extraAttr", vector<string>({"handle"}))
            pair<string, vector<string>>("coordAttr", vector<string>({"coord_x", "coord_y"})),
            pair<string, vector<string>>("openCriAttr", vector<string>({"color", "layerHistory", "belongToWhichBlock", "BRHandleHistory", "pedigreeLogger"})), //"color", "layers", "isDyns", "belongToWhichBlocks", "BRHandles", "blockHandlesPedigree"
            pair<string, vector<string>>("closeCriAttr", vector<string>({"visibleHistory", "isDynHistory"})), //"visibles", "isFromHatchs"
            pair<string, vector<string>>("extraAttr", vector<string>({"handle"}))
        };
        return baseAttrCri;
    }
};


struct LayerInfoDict {
    string id;
    string handle;
    string name;
    bool on;
    bool frozen;
    bool locked;
    bool plottable;
    string color;
};


struct BaseFieldsInJsonInfoDictAndElementInses {
    string id;
    string handle;
    string name;
    string layer;
    bool visible;
    map<string, double> min;
    map<string, double> max;
    vector<string> xdatas;
    // map<string, double>* min;
    // map<string, double>* max;
    // vector<string>* xdatas;

    bool isDyn = false; //.json文件中没有这个字段；假装有这个字段，因为有的block没有被引用 //TODO: 考虑移动到Elements的BaseIns的成员属性中 //这是为Elements准备的，json中的isDynBlock是BlockReference中的字段；
    bool isFromHatch = false; //.json文件中没有这个字段 //TODO: 这是BLockTools要用的字段
};


// class BaseFieldsInJsonInfoDictAndElementInses {
// public:
//     string id;
//     string handle;
//     string name;
//     string layer;
//     bool visible;
//     map<string, double>* min;
//     map<string, double>* max;
//     vector<string> xdatas;
//
//     bool isDyn;// = false; //TODO: 考虑移动到Elements的BaseIns的成员属性中 //这是为Elements准备的，json中的isDynBlock是BlockReference中的字段；
//     bool isFromHatch; //TODO: 考虑移动到Elements的BaseIns的成员属性中 //这是为Elements准备的，json中的hatchs是图元级别的字段；
//
//
//     BaseFieldsInJsonInfoDictAndElementInses() = default;
// };


struct LineInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    map<string, double> startPoint;
    map<string, double> endPoint;
    map<string, double> normal;
};


struct PolylineInfoDict : virtual public BaseFieldsInJsonInfoDictAndElementInses { //TODO: 这里为什么会多出来一个virtual？
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    vector<map<string, double>> vertexs; //TODO: 虽然这里拼写错误，但是不容易修改，应该是vertexes；
};


struct CircleInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    map<string, double> center;
    double radius;
    map<string, double> normal;
};


struct ArcInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    map<string, double> center;
    double radius;
    map<string, double> startPoint;
    map<string, double> endPoint;
    map<string, double> normal;
};


struct EllipseInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    map<string, double> center;
    double startAngle;
    double endAngle;
    map<string, double> majorAxis;
    map<string, double> minorAxis;
    double radiusRatio; //TODO: 后续应该修改为radiusRatio，但是好像这个字段用不到
    map<string, double> normal;
};


struct PointInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {//79
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    map<string, double> position;
    double ecsRotation;
    map<string, double> normal;
};


struct SplineInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    int degree;
    map<string, double> startFitTangent;
    map<string, double> endFitTangent;
    bool hasFitData;
    bool isRational;
    int controlPointNub;
    vector<map<string, double>> controlPoints;
    int fitPointNub;
    vector<map<string, double>> fitPoints;
};


struct TextInfoDict : public BaseFieldsInJsonInfoDictAndElementInses { //0
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    double height;
    double widthFactor;
    string text;
    map<string, double> position;
    double rotation;
    map<string, double> normal;
    string horizontalMode;
    string verticalMode;
};


struct MtextInfoDict : public BaseFieldsInJsonInfoDictAndElementInses { //72
    // TODO: 目前看来 mtext 是不方便继承 text 的 //这里没有lineNums吗
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    // TODO： 目前的.json文件中关于mtexts不存在horizontalMode字段和verticalMode字段，如果后续增加了，需要在这里添加这两个字段，需要在JsonTools中添加获取逻辑，需要在Elements中添加赋值逻辑，需要在Filter中添加转移逻辑，需要在Painter中采用这一信息，需要在DatabaseConverter中转换这一字段
    double actualHeight;
    double actualWidth;
    string contents;
    map<string, double> direction;
    map<string, double> location;
    map<string, double> normal;
    double rotation;
};


struct AttributeInfoDict : public BaseFieldsInJsonInfoDictAndElementInses { //0 10
    // TODO: attribute还是不继承text比较好
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    double height;
    double widthFactor;
    string text;
    map<string, double> position;
    double rotation;
    map<string, double> normal;
    string horizontalMode;
    string verticalMode;
    string tag;
};


struct SpatialFilterInfoDict {
    int pointsSize;
    map<string, double> normal;
    map<string, double> point;
    bool enable;
    bool isInverted;
    int clippingFront;
    int clippingBack;
    vector<map<string, double>> boundaryPoints;
    vector<map<string, double>> clipSpace;
    vector<map<string, double>> inverseBlockRefForm;
};


struct HpolylineInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    bool closed;
    vector<map<string, double>> vertexs;
};


struct BlockReferenceInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    string record;
    string blockTableRecord;
    map<string, double> position;
    double rotation;
    map<string, double> scaleFactor;
    map<string, double> normal;
    bool isDynBlock; //TODO: .json文件中具有这个字段，这就是Dyn与Hatch的不同 //TODO: 在json完善之后，可以考虑删除此初始值
    // bool isFromHatch; //.json文件中没有这个字段 //TODO 读取json时的容错机制，json中某些字段的临时缺失

    // TODO 完善这些结构
    vector<AttributeInfoDict> attributes;
    struct SpatialFilterInfoDict spatialFilter;
};


struct HatchInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    string hatchType;
    string patternName;
    double patternAngle;
    double patternScale;
    double patternSpace;
    bool patternDouble;
    string hatchStyle;
    string color;
    map<string, double> originPoint;
    vector<map<string, double>> hatchLoops;
    vector<HpolylineInfoDict> hpolylines;
    vector<PolylineInfoDict> polylineInfoDictsInHatch;
    vector<LineInfoDict> lineInfoDictsInHatch;
    vector<ArcInfoDict> arcInfoDictsInHatch;
    vector<CircleInfoDict> circleInfoDictsInHatch;
    vector<EllipseInfoDict> ellipseInfoDictsInHatch;
    vector<BlockReferenceInfoDict> blockReferenceInfoDictsInHatch;
    vector<PointInfoDict> pointInfoDictsInHatch;
    vector<SplineInfoDict> splineInfoDictsInHatch;
};


struct DimensionInfoDict : public BaseFieldsInJsonInfoDictAndElementInses {
    // TODO: 因为不知道其它Dimension的字段，不能对比抽象出共同字段，所以这里Dimension将AlignedDimension中的全部字段搬运过来；后续如果字段丰富了，应该需要改进；
    // struct BaseFieldsInJsonInfoDictAndElementInses base;
    string textAttachment;
    double textWidth;
    double textHeight;
    double textLineSpacingFactor;
    map<string, double> textPosition;
    double textRotation;
    bool isUsingDefaultTextPosition;
    string dimensionText;
    double measurement;
    string searchValue;
    map<string, double> dimLinePoint;
    map<string, double> xLine1Point;
    map<string, double> xLine2Point;
    double oblique;
};


struct AlignedDimensionInfoDict : public DimensionInfoDict {

};


struct BlockInfoDict {
    string id;
    string handle;
    string name;
    string blockScaling;
    map<string, double> origin;
    vector<string> entityInfoDicts;
    vector<LineInfoDict> lineInfoDicts;
    vector<PolylineInfoDict> polylineInfoDicts;
    vector<ArcInfoDict> arcInfoDicts;
    vector<CircleInfoDict> circleInfoDicts;
    vector<EllipseInfoDict> ellipseInfoDicts;
    vector<PointInfoDict> pointInfoDicts;
    vector<SplineInfoDict> splineInfoDicts;
    vector<TextInfoDict> textInfoDicts;
    vector<MtextInfoDict> mtextInfoDicts;
    vector<HatchInfoDict> hatchInfoDicts;
    vector<BlockReferenceInfoDict> blockReferenceInfoDicts;
    vector<AlignedDimensionInfoDict> alignedDimensionInfoDicts;

    // TODO 搜集这些字段的数据结构
    vector<map<string, double>> rotatedDimensionInfoDicts;
    vector<map<string, double>> lineAngularDimensionInfoDicts;
    vector<map<string, double>> arcDimensionInfoDicts;
    vector<map<string, double>> radialDimensionInfoDicts;
    vector<map<string, double>> diametricDimensionInfoDicts;
    vector<map<string, double>> radialDimensionLargeInfoDicts;
    vector<map<string, double>> ordinateDimensionInfoDicts;
    vector<map<string, double>> mLeaderInfoDicts;
};


struct LayoutInfoDict {
    string id;
    string handle;
    string name;
    string blockTableRecordHandle;
    double tabOrder;
    bool tabSelected;
    map<string, double> min;
    map<string, double> max;
};


struct Infos {
    vector<struct LayerInfoDict> layerInfoDicts;
    vector<struct BlockInfoDict> blockInfoDicts;
    vector<struct LayoutInfoDict> layoutInfoDicts;
    vector<string> groupInfoDicts;
};


#endif //ATUOCADDATAANALYSISPYCADCPP_STRUCTS_H
