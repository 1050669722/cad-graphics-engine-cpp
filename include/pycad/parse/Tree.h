//
// Created by liuxingbo03 on 2021/10/22.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_TREE_H
#define ATUOCADDATAANALYSISPYCADCPP_TREE_H


#include "pycad/Database.h"
#include "pycad/Configuration.h"
#include "pycad/parse/BLock.h"
#include "pycad/parse/ParseStructs.h"


// TODO: 考虑删除
// struct TransSolution {
//     /*
//      * 经过积累的 引用变换方案
//      */
//     Records records;
//     ScaleFactors scaleFactors;
//     Rotations rotations;
//     Positions positions;
//     Normals normals;
//     Layers layers;
//     IsDyns isDyns;
//     BLockReferenceHandles BRHandles;
//     Visibles visibles;
//     IsFromHatchs isFromHatchs;
//
//     Attributes attributes;
//     SpatialFilter spatialFilter;
// };


class TransSolution {
    /*
     * 经过积累的 引用变换方案
     */
public:
    // 成员属性
    Records records;
    ScaleFactors scaleFactors;
    Rotations rotations;
    Positions positions;
    Normals normals;
    Layers layers;
    IsDyns isDyns;
    BLockReferenceHandles BRHandles;
    Visibles visibles;
    IsFromHatchs isFromHatchs;

    Attributes attributes;
    SpatialFilter spatialFilter;

    // 成员方法
    TransSolution() {
        records.reserve(100);
        rotations.reserve(100);
        layers.reserve(100);
        BRHandles.reserve(100);

        attributes.reserve(100);
        spatialFilter.reserve(100);
    }
};


// TODO: 考虑删除
// struct EInsDataToBeWritten { //TODO 结合Elements.cpp中的筛选判据扩充
//     /*
//      * 单个元素实例的 将要被写入数据库的 数据
//      */
//     Coord coord_x;
//     Coord coord_y;
// };


// TODO: 考虑删除
// struct OutputOfSprout {
//     Database database;
//     int referSeq;
//     vector<TransSolution> solutionsPaths;
// };


class Tree {
private:
    // 成员属性
    Configuration* config = nullptr;
    BLock* block = nullptr;
    Pedigree* pedigree = nullptr;
    TransSolution* transSolution = nullptr;
    MatrixXd* affineMat = nullptr;
    int* referSeq = nullptr;
    int totalNum{};
    vector<TransSolution>* solutionsPaths = nullptr;

    // 成员方法
    // 装配引用变换方案 //in-place修改transST
    void assembleSolution(BlockReferenceInfoDict& solution);
    // 拆解引用变换方案
    void disassembleSolution();

    // 计算坐标属性
    static void collectCoordsValsAndCoordsPtrsForEIns(
                                                vector<double>& coords_vals_x,
                                                vector<double>& coords_vals_y,
                                                vector<double*>& coords_ptrs_x,
                                                vector<double*>& coords_ptrs_y,
                                                BaseIns& eIns
                                                );
    void calCoords(); //集中 计算坐标
    void calCoords(BaseIns& eIns); //分散 计算坐标

    // 计算判据属性
    void calParams(BaseIns& eIns);

    // 写数据库
    static void wirteDatabaseForOneFieldInDatabase(BaseFieldInDatabase& database_field, BaseIns& eIns);
    void writeDatabase(const char* eType, BaseIns& eIns);

    // 针对当前block的 某一类型的图元 计算坐标、计算判据、写数据库
    void enrichDC();
    void enrichDataCore(const char* eType);
    void enrichDC_LineInses();
    void enrichDC_PolylineInses();
    void enrichDC_CircleInses();
    void enrichDC_ArcInses();
    void enrichDC_EllipseInses();
    void enrichDC_PointInses();
    void enrichDC_SplineInses();
    void enrichDC_TextInses();
    void enrichDC_MtextInses();
    void enrichDC_AttributeInses();
    void enrichDC_AlignedDimensionInses();

    // 进度显示
    void displayProcess() const;

public:
    // 成员属性
    Database* database = nullptr;

    // 成员方法
    Tree();
    explicit Tree( //本类构造函数中所有参数都不要采用引用的方式传参，以免产生不必要的错误 //但是不采用引用的方式，会很慢
                Configuration* config,
                BLock* block,
                vector<string>* pedigree,
                TransSolution* transSolution,
                MatrixXd* affineMat,
                Database* database,
                vector<TransSolution>* solutionsPaths,
                int* referSeq,
                int totalNum=0
            );
    // 递归填充database
    void sprout();
    // 填充并输出database的逻辑 的封装
    static void bloom(Database* ptr_database, Configuration* ptr_config, BLock* ptr_rootBlock);

};


#endif //ATUOCADDATAANALYSISPYCADCPP_TREE_H
