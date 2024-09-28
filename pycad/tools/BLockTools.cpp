//
// Created by liuxingbo03 on 2021/11/18.
//

#include <iostream>

// #include <boost/timer.hpp>
#include <ctime>

#include "pycad/tools/BLockTools.h"
#include "pycad/tools/JsonTools.h"


using namespace std;
// using namespace boost;


BLockTools::BLockTools() = default;


BLockTools::BLockTools(const wchar_t *jsonPath) {
    readJson(jsonPath); //生成 infos
    generateRootBlock(); //生成 rootBlock
}


BLockTools::BLockTools(string &jsonPathFromPython) {
    readJson(jsonPathFromPython); //生成 infos
    generateRootBlock(); //生成 rootBlock
}


void BLockTools::readJson(const wchar_t *jsonPath) {
    JsonTools::readJson(infos, jsonPath);
}


void BLockTools::readJson(string &jsonPathFromPython) {
    JsonTools::readJson(infos, jsonPathFromPython);
}


template<class T>
void BLockTools::imprintIsFromHatch(vector<T>& elementInfoDicts) {
    /*
     * 打一个标记，今后没准能用到
     */
    // // for-test
    // cout << "4" << endl;
    for (auto& elementInfoDict : elementInfoDicts) {
        // // for-test
        // cout << "5" << endl;
        elementInfoDict.isFromHatch = true;
    }
}


void BLockTools::moveHatchs() {
    /*
     * 搬运hatchs中的图元至同级blocks
     */
    // 遍历每一个block
    // // for-test
    // cout << "0" << endl;
    for (auto& blockInfoDict : infos.blockInfoDicts) {
        // 兼容性考虑
        // // for-test
        // cout << "1" << endl;
        if (blockInfoDict.hatchInfoDicts.empty()) continue;

        // 搬运
        // // for-test
        // cout << "2" << endl;
        for (auto& hatchInfoDict : blockInfoDict.hatchInfoDicts) {
            // // for-test
            // cout << "3" << endl;
            // polylines in this hatchInfoDict
            imprintIsFromHatch<PolylineInfoDict>(hatchInfoDict.polylineInfoDictsInHatch);
            blockInfoDict.polylineInfoDicts.insert(blockInfoDict.polylineInfoDicts.end(), hatchInfoDict.polylineInfoDictsInHatch.begin(), hatchInfoDict.polylineInfoDictsInHatch.end());

            // lines in this hatchInfoDict
            imprintIsFromHatch<LineInfoDict>(hatchInfoDict.lineInfoDictsInHatch);
            blockInfoDict.lineInfoDicts.insert(blockInfoDict.lineInfoDicts.end(), hatchInfoDict.lineInfoDictsInHatch.begin(), hatchInfoDict.lineInfoDictsInHatch.end());

            // arcs in this hatchInfoDict
            imprintIsFromHatch<ArcInfoDict>(hatchInfoDict.arcInfoDictsInHatch);
            blockInfoDict.arcInfoDicts.insert(blockInfoDict.arcInfoDicts.end(), hatchInfoDict.arcInfoDictsInHatch.begin(), hatchInfoDict.arcInfoDictsInHatch.end());

            // circles in this hatchInfoDict
            imprintIsFromHatch<CircleInfoDict>(hatchInfoDict.circleInfoDictsInHatch);
            blockInfoDict.circleInfoDicts.insert(blockInfoDict.circleInfoDicts.end(), hatchInfoDict.circleInfoDictsInHatch.begin(), hatchInfoDict.circleInfoDictsInHatch.end());

            // ellipses in this hatchInfoDict
            imprintIsFromHatch<EllipseInfoDict>(hatchInfoDict.ellipseInfoDictsInHatch);
            blockInfoDict.ellipseInfoDicts.insert(blockInfoDict.ellipseInfoDicts.end(), hatchInfoDict.ellipseInfoDictsInHatch.begin(), hatchInfoDict.ellipseInfoDictsInHatch.end());

            // blockReferences in this hatchInfoDict
            imprintIsFromHatch<BlockReferenceInfoDict>(hatchInfoDict.blockReferenceInfoDictsInHatch);
            blockInfoDict.blockReferenceInfoDicts.insert(blockInfoDict.blockReferenceInfoDicts.end(), hatchInfoDict.blockReferenceInfoDictsInHatch.begin(), hatchInfoDict.blockReferenceInfoDictsInHatch.end());

            // points in this hatchInfoDict
            imprintIsFromHatch<PointInfoDict>(hatchInfoDict.pointInfoDictsInHatch);
            blockInfoDict.pointInfoDicts.insert(blockInfoDict.pointInfoDicts.end(), hatchInfoDict.pointInfoDictsInHatch.begin(), hatchInfoDict.pointInfoDictsInHatch.end());

            // splines in this hatchInfoDict
            imprintIsFromHatch<SplineInfoDict>(hatchInfoDict.splineInfoDictsInHatch);
            blockInfoDict.splineInfoDicts.insert(blockInfoDict.splineInfoDicts.end(), hatchInfoDict.splineInfoDictsInHatch.begin(), hatchInfoDict.splineInfoDictsInHatch.end());
        }
    }
}


void BLockTools::generateMapHandle2BlockInfoDcit() {
    /*
     * 获得blocks字段对应的数据信息
     */
    // TODO 实现 moveHatchs
    moveHatchs();

    // 填充数据
    for (BlockInfoDict& blockInfoDict : infos.blockInfoDicts) {
        handle2BlockInfoDict.insert(pair<string, BlockInfoDict*>(blockInfoDict.handle, &blockInfoDict));
    }

    // 断言
    assert(!handle2BlockInfoDict.empty());
}


void BLockTools::generateMapHandle2ReferenceTable() {
    /*
     * 获得blocks字段对应的数据信息
     */

    // 填充数据
    for (BlockInfoDict& blockInfoDict : infos.blockInfoDicts) {
        // 这个block的handle
        string blockHandle = blockInfoDict.handle;
        // 这个block的referenceTable
        vector<string> referenceTable;
        // 遍历这个block的blockReferences
        for (BlockReferenceInfoDict& blockReferenceInfoDict : blockInfoDict.blockReferenceInfoDicts) {
            referenceTable.push_back(blockReferenceInfoDict.blockTableRecord);
        }
        // 建立本轮的键值对
        handle2ReferenceTable.insert(pair<string, vector<string>>(blockHandle, referenceTable));
    }

    // 断言
    assert(!handle2ReferenceTable.empty());
}


// map<string, BLock> TreeTools::generateMapHandle2Block() {
void BLockTools::generateMapHandle2Block() {
    /*
     * 实例化 每一个 BLock
     * 并 逐个 为 handle2Block 填充数据
     */
    // 遍历 每一个节点(每一个root)，实例化Block，并且实例化其子节点的Block，并且写入handle2Block
    for (auto& it : handle2BlockInfoDict) {
        string root = it.first;
        traverse(root.c_str());
    }
}


void BLockTools::traverse(const char *root) {
    /*
     * 深度优先遍历 blocks
     * 实例化 BLock
     * 并为 handle2Block 填充数据
     * 这两个职责是共生的
     */
    // 实例化Block
    BLock block = BLock(*(handle2BlockInfoDict[root]));

    // 如果这个block还引用了其它block //其它的block也需经过traverse
    // 遍历这个handle(这个root)引用了哪些handles
    for (string& referencedBlockHandle : handle2ReferenceTable[root]) {
        if (handle2Block.find(referencedBlockHandle) == handle2Block.end()) {
            traverse(referencedBlockHandle.c_str());
        }
    }
    // 为刚刚实例化的block添加referencedBlockHandle
    // 遍历这个handle(这个root)引用了哪些handles
    // timer t_ogm8;
    for (string& referencedBlockHandle : handle2ReferenceTable[root]) {
        block.appendReferenceBlock(handle2Block[referencedBlockHandle]); //block中的references中不再保留handle2Block中block的副本，而是以指针的方式一种这些handle2Block中的block，在Tree中的过程也是在修改这些handle2Block中的block
    }
    // cout << "t_ogm8: " <<  t_ogm8.elapsed() << endl;

    // 填充数据
    // timer t_ogm9;
    handle2Block.insert(pair<string, BLock>(root, block));
    // cout << "t_ogm9: " <<  t_ogm9.elapsed() << endl;
}


string BLockTools::whichLayoutWillBePlot(const char *layoutName) {
    // 用于记录输出的变量
    string handleOfLayoutWillBePlot; //自动初始化

    // 遍历layouts
    for (LayoutInfoDict& layoutInfoDict : infos.layoutInfoDicts) {
        if (layoutInfoDict.name == layoutName) {
            handleOfLayoutWillBePlot = layoutInfoDict.blockTableRecordHandle;
        }
    }

    // 断言
    assert(!handleOfLayoutWillBePlot.empty());

    // 返回
    return handleOfLayoutWillBePlot;
}


void BLockTools::generateRootBlock() {
    // 统计时间
    // timer t_ogm;
    clock_t start = clock();

    // 打印提示信息 //TODO 应该做成装饰模式
    // cout << "[INFO] RootBlock Initializing ..." << endl;
    cout << "[INFO] generateRootBlock ..." << endl;

    // 生成handle2BlockInfoDict
    // timer t_ogm0;
    generateMapHandle2BlockInfoDcit();
    // cout << t_ogm0.elapsed() << endl;

    // 生成handle2ReferenceTable
    // timer t_ogm1;
    generateMapHandle2ReferenceTable();
    // cout << t_ogm1.elapsed() << endl;

    // handleOfLayoutWillBePlot
    // timer t_ogm2;
    string handleOfLayoutWillBePlot = whichLayoutWillBePlot();
    // cout << t_ogm2.elapsed() << endl;

    // 生成map: Handle -> BLock
    // timer t_ogm3;
    this->generateMapHandle2Block();
    // cout << t_ogm3.elapsed() << endl;

    // 属性赋值
    // timer t_ogm4;
    rootBlock = &handle2Block[handleOfLayoutWillBePlot];
    // cout << t_ogm4.elapsed() << endl;

    // // 打印提示信息
    // cout << "[INFO] Done" << endl;

    // 统计时间
    // double time_cost = t_ogm.elapsed(); //秒
    double time_cost = (double)(clock() - start) / CLOCKS_PER_SEC; //秒
    cout << "[INFO] generateRootBlock done, " << time_cost << "s has elapsed" << endl; //TODO: 装饰器怎么写出来？
}


// int main() {
//     // jsonPath
//     string jsonPath = "../json/15.YJ160-11(T2-17X)_t3.json";
//
//     // infos
//     Infos infos = JsonTools().readJson(jsonPath.c_str());
//
//     // handleOfLayoutWillBePlot
//     string handleOfLayoutWillBePlot0 = TreeTools::whichLayoutWillBePlot(jsonPath.c_str());
//
//     // handleOfLayoutWillBePlot
//     string handleOfLayoutWillBePlot1 = TreeTools::whichLayoutWillBePlot(infos);
//
//     // 实例化TreeTools
//     TreeTools TT = TreeTools(jsonPath.c_str());
//
//     // 生成map: Handle -> BLock
//     map<string, BLock> handle2Block = TT.generateMapHandle2Block();
//
//     // for test
//     for (auto& h2b : handle2Block) {
//         if (h2b.first == "[2]") {
//             // for (auto & reference : h2b.second.references) {
//             //     // cout << reference << endl;
//             //     cout << h2b.second.references.size() << endl;
//             // }
//             // h2b.second.polylineInses[0];
//             cout << h2b.second.polylineInses[0].visible << endl;
//         }
//     }
//
//     // 返回
//     return 0;
// }
