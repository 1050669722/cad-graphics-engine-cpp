//
// Created by liuxingbo03 on 2021/10/22.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_BLOCK_H
#define ATUOCADDATAANALYSISPYCADCPP_BLOCK_H


#include <vector>


#include "pycad/Structs.h"
#include "pycad/parse/Elements.h"


using namespace std;


class BLock : public BlockInfoDict {
private:
    // 成员属性

    // 成员方法
    // 搜集引用变换方案 TransSolution
    void howToReference();
    // 实例化元素 //block自身包含的元素
    void instantiateElements();

public:
    // 成员属性
    /*
     * TODO: 这里遵循的规则是这样的，这些变量在计算过程中不会被修改，所以多个BLock公用一份值；而图元需要被修改，所以每个BLock各自保留一个副本
     * TODO: 最初的原则是，可以直接拿过来的就以指着接收地址，那些需要组装和本地生成的，则各自保留备份；
     * TODO: 不过这里产生了一个新的好处，那些本地产生的备份，恰是需要被改动的，所以这样做又更加合理了；
     */
    // 指针
    map<string, double>* ptr_origin{};
    vector<string>* ptr_entityInfoDicts{};
    vector<LineInfoDict>* ptr_lineInfoDicts{};
    vector<PolylineInfoDict>* ptr_polylineInfoDicts{};
    vector<ArcInfoDict>* ptr_arcInfoDicts{};
    vector<CircleInfoDict>* ptr_circleInfoDicts{};
    vector<EllipseInfoDict>* ptr_ellipseInfoDicts{};
    vector<PointInfoDict>* ptr_pointInfoDicts{};
    vector<SplineInfoDict>* ptr_splineInfoDicts{};
    vector<TextInfoDict>* ptr_textInfoDicts{};
    vector<MtextInfoDict>* ptr_mtextInfoDicts{};
    vector<HatchInfoDict>* ptr_hatchInfoDicts{};
    vector<BlockReferenceInfoDict>* ptr_blockReferenceInfoDicts{};
    vector<AlignedDimensionInfoDict>* ptr_alignedDimensionInfoDicts{};
    vector<map<string, double>>* ptr_rotatedDimensionInfoDicts{};
    vector<map<string, double>>* ptr_lineAngularDimensionInfoDicts{};
    vector<map<string, double>>* ptr_arcDimensionInfoDicts{};
    vector<map<string, double>>* ptr_radialDimensionInfoDicts{};
    vector<map<string, double>>* ptr_diametricDimensionInfoDicts{};
    vector<map<string, double>>* ptr_radialDimensionLargeInfoDicts{};
    vector<map<string, double>>* ptr_ordinateDimensionInfoDicts{};
    vector<map<string, double>>* ptr_mLeaderInfoDicts{};

    // 保存BLock实例的容器
    vector<BLock> references;

    // 引用变换参数
    vector<pair<string, BlockReferenceInfoDict>> referenceTransformSolutions;

    // 存放元素实例的容器 //区分例：line, lineIns //带有Ins的是Elements中的元素class；不到有Ins的是Structs中的结构体
    vector<LineIns> lineInses;
    vector<PolylineIns> polylineInses;
    vector<CircleIns> circleInses;
    vector<ArcIns> arcInses;
    vector<EllipseIns> ellipseInses;
    vector<PointIns> pointInses;
    vector<SplineIns> splineInses;
    vector<TextIns> textInses;
    vector<MtextIns> mtextInses;
    vector<AttributeIns> attributeInses;
    vector<AlignedDimensionIns> alignedDimensionInses;

    // 成员方法
    // 默认构造函数
    BLock();
    explicit BLock(BlockInfoDict &blockInfoDict);

    // 添加 Block实例 到 本Block实例的references容器
    void appendReferenceBlock(BLock& block);

    // 计数 countBlocks
    int countBlocks(int num);

    //

};


#endif //ATUOCADDATAANALYSISPYCADCPP_BLOCK_H
