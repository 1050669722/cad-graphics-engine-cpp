//
// Created by liuxingbo03 on 2021/11/18.
//

#ifndef ATUOCADDATAANALYSISPYCADCPP_BLOCKTOOLS_H
#define ATUOCADDATAANALYSISPYCADCPP_BLOCKTOOLS_H


#include "pycad/Structs.h"
#include "pycad/parse/BLock.h"


class BLockTools {
private:
    // 成员属性
    // Hatch中可能包含的图元类型，在json文件中的名称
    vector<string> eleTypeInHatch{"polylines", "lines", "arcs", "circles", "points", "splines", "blockReferences", "ellipses"};

    // handle2BlockInfoDcit
    map<string, BlockInfoDict*> handle2BlockInfoDict; //这个BlockInfoDict是拿来就用的，所以修改为指针类型是合适的

    // handle2ReferenceTable
    map<string, vector<string>> handle2ReferenceTable; //这个vector<string>是需要现场组装的，所以修改为指针类型不合适

    // handle2Block
    map<string, BLock> handle2Block; //这个BLock是需要现场组装的，所以修改为指针类型不合适

    // 成员方法
    // 读取json文件
    // void readJson(const wchar_t* jsonPath);
    void readJson(string& jsonPathFromPython);

    // 为来自hatchs的图元作标记
    template<class T>
    void imprintIsFromHatch(vector<T>& elementInfoDicts);

    // 搬运hatchs中的图元至block
    void moveHatchs();

    // 生成map: Handle -> BlockInfoDict //pycadDifference: 更名 generateHandle2BlockInfo -> generateMapHandle2BlockInfoDcit
    void generateMapHandle2BlockInfoDcit();

    // 生成map: Handle -> referenceTable //pycadDifference: 更名 generateReferenceTable -> generateMapHandle2ReferenceTable
    void generateMapHandle2ReferenceTable();

    // 递归遍历blcoks //pycadDifference 更名 traverseTree -> traverse
    void traverse(const char* root);

    // 获取根节点的handle [重载] //pycadDifference: 取消输出allLayoutsHandles；从utils搬运到TreeTools
    string whichLayoutWillBePlot(const char* layoutName="Model");

    // 生成map: Handle -> BLock //pycadDifference: 更名 generateHandle2BlockInfo -> generateMapHandle2Block
    void generateMapHandle2Block();


public:
    // 成员属性
    // infos
    Infos infos;
    // 根Block
    BLock* rootBlock{};

    // 成员方法
    // 默认构造函数
    BLockTools();
    // explicit BLockTools(const wchar_t* jsonPath);
    explicit BLockTools(string& jsonPathFromPython);
    // 生成rootBlock
    void generateRootBlock();
};


#endif //ATUOCADDATAANALYSISPYCADCPP_BLOCKTOOLS_H
